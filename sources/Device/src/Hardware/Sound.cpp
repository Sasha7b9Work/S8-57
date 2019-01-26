#include <stm32f4xx_hal.h>
#include "defines.h"
#include "Sound.h"
#include "Timer.h"
#include "Hardware.h"
#include "Utils/Math.h"
#include <cmath>


using Hardware::Timer;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DAC_HandleTypeDef Sound::handleDAC = {DAC};

#define POINTS_IN_PERIOD_SOUND 10
static uint8 points[POINTS_IN_PERIOD_SOUND] = {0};
static float frequency = 0.0F;
static float amplitude = 0.0F;
static TypeWave::E typeWave = TypeWave::Sine;
static bool soundWarnIsBeep = false;
static bool buttonIsPressed = false;    ///< \brief Когда запускается звук нажатой кнопки, устанавливается этот флаг, чтобы знать, проигрывать ли знак 
                                        ///< отпускания
static volatile bool isBeep = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Sound::Init()
{
    __DMA1_CLK_ENABLE();
    __TIM7_CLK_ENABLE();
    __DAC_CLK_ENABLE();

    GPIO_InitTypeDef structGPIO =
    {
        GPIO_PIN_4,
        GPIO_MODE_ANALOG,
        GPIO_NOPULL,
        0, 0
    };

     HAL_GPIO_Init(GPIOA, &structGPIO);

    static DMA_HandleTypeDef hdmaDAC1 =
    {
        DMA1_Stream5,
        {
            DMA_CHANNEL_7,
            DMA_MEMORY_TO_PERIPH,
            DMA_PINC_DISABLE,
            DMA_MINC_ENABLE,
            DMA_PDATAALIGN_BYTE,
            DMA_MDATAALIGN_BYTE,
            DMA_CIRCULAR,
            DMA_PRIORITY_HIGH,
            DMA_FIFOMODE_DISABLE,
            DMA_FIFO_THRESHOLD_HALFFULL,
            DMA_MBURST_SINGLE,
            DMA_PBURST_SINGLE
        },
        HAL_UNLOCKED, HAL_DMA_STATE_RESET, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0
    };

    HAL_DMA_Init(&hdmaDAC1);

    __HAL_LINKDMA(&handleDAC, DMA_Handle1, hdmaDAC1);

    HAL_NVIC_SetPriority(DMA1_Stream5_IRQn, PRIORITY_SOUND_DMA1_STREAM5);
    HAL_NVIC_EnableIRQ(DMA1_Stream5_IRQn);


    DAC_ChannelConfTypeDef config =
    {
        DAC_TRIGGER_T7_TRGO,
        DAC_OUTPUTBUFFER_ENABLE
    };

    HAL_DAC_DeInit(&handleDAC);

    HAL_DAC_Init(&handleDAC);

    HAL_DAC_ConfigChannel(&handleDAC, &config, DAC_CHANNEL_1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::Stop()
{
    HAL_DAC_Stop_DMA(&handleDAC, DAC_CHANNEL_1);
    isBeep = false;
    soundWarnIsBeep = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::ConfigTIM7(uint16 prescaler, uint16 period)
{
    static TIM_HandleTypeDef htim =
    {
        TIM7, {}, HAL_TIM_ACTIVE_CHANNEL_1, {}, HAL_UNLOCKED, HAL_TIM_STATE_RESET
    };

    htim.Init.Prescaler = prescaler;
    htim.Init.Period = period;
    htim.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;

    HAL_TIM_Base_Init(&htim);

    TIM_MasterConfigTypeDef masterConfig =
    {
        TIM_TRGO_UPDATE,
        TIM_MASTERSLAVEMODE_DISABLE
    };

    HAL_TIMEx_MasterConfigSynchronization(&htim, &masterConfig);

    HAL_TIM_Base_Stop(&htim);
    HAL_TIM_Base_Start(&htim);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 Sound::CalculatePeriodForTIM()
{
#define MULTIPLIER_CALCPERFORTIM 30e6F

    return (uint16)(MULTIPLIER_CALCPERFORTIM / frequency / POINTS_IN_PERIOD_SOUND);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::CalculateSine()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        float step = 2.0F * PI / (POINTS_IN_PERIOD_SOUND - 1);
        float value = (std::sinf(i * step) + 1.0F) / 2.0F;
        float v = value * amplitude * 255.0F;
        points[i] = (uint8)v;
    }

    for (int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        if (i < POINTS_IN_PERIOD_SOUND / 2)
        {
            points[i] = 0;
        }
        else
        {
            points[i] = (uint8)(255.0F * amplitude);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::CalculateMeandr()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND / 2; i++)
    {
        points[i] = (uint8)(255.0F * amplitude);
    }
    for(int i = POINTS_IN_PERIOD_SOUND / 2; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = 0;
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::CalculateTriangle()
{
    float k = 255.0F / POINTS_IN_PERIOD_SOUND;
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = (uint8)(k * (float)i * amplitude);
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::SetWave()
{
    ConfigTIM7(0, CalculatePeriodForTIM());

    if(typeWave == TypeWave::Sine)
    {
        CalculateSine();
    }
    else if(typeWave == TypeWave::Meandr)
    {
        CalculateMeandr();
    }
    else if(typeWave == TypeWave::Triangle) //-V547
    {
        CalculateTriangle();
    }
    else
    {
        // больше нет видов волн
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::Beep(const TypeWave::E newTypeWave, const float newFreq, const float newAmpl, const int newDuration)
{
    if (soundWarnIsBeep)
    {
        return;
    }
    if (!SOUND_ENABLED)
    {
        //return;
    }
    if (frequency != newFreq || amplitude != newAmpl || typeWave != newTypeWave) //-V550
    {
        frequency = newFreq;
        amplitude = newAmpl * SOUND_VOLUME / 100.0F;
        typeWave = newTypeWave;
        
        Stop();
        SetWave();
    }

    Stop();
    
    isBeep = true;
    
    HAL_DAC_Start_DMA(&handleDAC, DAC_CHANNEL_1, (uint32_t*)points, POINTS_IN_PERIOD_SOUND, DAC_ALIGN_8B_R); //-V1032 //-V641

    Timer::SetAndStartOnce(Timer::Type::StopSound, Stop, (uint)newDuration);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WaitForCompletion()
{
    while (isBeep)
    {
    };
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::ButtonPress()
{
    Beep(TypeWave::Sine, 2000.0F, 0.75F, 50);
    buttonIsPressed = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::ButtonRelease()
{
    if (buttonIsPressed)
    {
        Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
        buttonIsPressed = false;
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::GovernorChangedValue()
{
    Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
    buttonIsPressed = false;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::RegulatorShiftRotate()
{
    Beep(TypeWave::Sine, 1.0F, 0.2F, 3);
    buttonIsPressed = false;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::RegulatorSwitchRotate()
{
    Beep(TypeWave::Sine, 500.0F, 0.5F, 75);
    buttonIsPressed = false;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WarnBeepBad()
{
    Beep(TypeWave::Meandr, 500.0F, 1.0F, 500);
    soundWarnIsBeep = true;
    buttonIsPressed = false;
}
 
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::WarnBeepGood()
{
    Beep(TypeWave::Triangle, 1000.0F, 1.0F, 500);
    buttonIsPressed = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::Beep(TypePress::E type)
{
    static const pFuncVV func[4] =
    {
        //Sound::ButtonPress,
        //Sound::ButtonPress,
        //Sound::ButtonRelease,
        EmptyFuncVV,
        EmptyFuncVV,
        EmptyFuncVV,
        EmptyFuncVV
    };

    func[type]();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Sound::Test()
{
    ButtonPress();
    ButtonRelease();
    GovernorChangedValue();
    RegulatorShiftRotate();
    RegulatorSwitchRotate();
    WarnBeepBad();
    WarnBeepGood();
}
