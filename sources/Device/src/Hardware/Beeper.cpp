#include "defines.h"
#include "Beeper.h"
#include "Timer.h"
#include "Utils/Math.h"
#include "Hardware/HAL/HAL.h"
#include <cmath>



#define POINTS_IN_PERIOD_SOUND 10
static uint8 points[POINTS_IN_PERIOD_SOUND] = {0};
static float frequency = 0.0F;
static float amplitude = 0.0F;
static TypeWave::E typeWave = TypeWave::Sine;
static bool soundWarnIsBeep = false;
static bool buttonIsPressed = false;    ///< \brief Когда запускается звук нажатой кнопки, устанавливается этот флаг, чтобы знать, проигрывать ли знак 
                                        ///< отпускания
static volatile bool isBeep = false;

static bool bellIsEnabled = false;



static void Beep(const TypeWave::E newTypeWave, const float newFreq, const float newAmpl, const int newDuration);

static void Stop();

static void SetWave();

static void CalculateMeandr();

static uint16 CalculatePeriodForTIM();

static void CalculateSine();

static void CalculateTriangle();


void Beeper::Init()
{
    HAL_DAC1::Init();
}


static void Stop()
{
    HAL_DAC1::StopDMA();
    isBeep = false;
    soundWarnIsBeep = false;
}


static uint16 CalculatePeriodForTIM()
{
#define MULTIPLIER_CALCPERFORTIM 30e6F

    return (uint16)(MULTIPLIER_CALCPERFORTIM / frequency / POINTS_IN_PERIOD_SOUND);
}


static void CalculateSine()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        float step = 2.0F * Math::PI_F / (POINTS_IN_PERIOD_SOUND - 1);
        float value = (std::sinf(i * step) + 1.0F) / 2.0F;
        float v = value * amplitude * 255.0F;
        points[i] = static_cast<uint8>(v);
    }

    for (int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        if (i < POINTS_IN_PERIOD_SOUND / 2)
        {
            points[i] = 0;
        }
        else
        {
            points[i] = static_cast<uint8>(255.0F * amplitude);
        }
    }
}



static void CalculateMeandr()
{
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND / 2; i++)
    {
        points[i] = static_cast<uint8>(255.0F * amplitude);
    }
    for(int i = POINTS_IN_PERIOD_SOUND / 2; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = 0;
    }
}



static void CalculateTriangle()
{
    float k = 255.0F / POINTS_IN_PERIOD_SOUND;
    for(int i = 0; i < POINTS_IN_PERIOD_SOUND; i++)
    {
        points[i] = static_cast<uint8>(k * static_cast<float>(i) * amplitude);
    }
}



static void SetWave()
{
    HAL_DAC1::ConfigTIM7(0, CalculatePeriodForTIM());

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



static void Beep(const TypeWave::E newTypeWave, const float newFreq, const float newAmpl, const int newDuration)
{
    if (bellIsEnabled || soundWarnIsBeep || !(set.serv.soundVolume == 0))
    {
        return;
    }

    if (frequency != newFreq || amplitude != newAmpl || typeWave != newTypeWave) //-V550 //-V2550
    {
        frequency = newFreq;
        amplitude = newAmpl * set.serv.soundVolume / 100.0F;
        typeWave = newTypeWave;
        
        Stop();
        SetWave();
    }

    Stop();
    
    isBeep = true;

    HAL_DAC1::StartDMA(points, POINTS_IN_PERIOD_SOUND);

    Timer::SetAndStartOnce(TypeTimer::StopSound, Stop, static_cast<uint>(newDuration));
}


void Beeper::Bell::On()
{
    if (!bellIsEnabled)
    {
        Beeper::WaitForCompletion();


        frequency = 2000.0F;
        amplitude = 1.0F;
        typeWave = TypeWave::Sine;

        Stop();

        SetWave();

        HAL_DAC1::StartDMA(points, POINTS_IN_PERIOD_SOUND);

        Timer::SetAndStartOnce(TypeTimer::StopSound, Stop, 1000000U);

        bellIsEnabled = true;
    }
}


void Beeper::Bell::Off()
{
    Stop();

    bellIsEnabled = false;
}


void Beeper::WaitForCompletion()
{
    while (isBeep)
    {
    };
}


void Beeper::ButtonPress()
{
    ::Beep(TypeWave::Sine, 2000.0F, 0.75F, 50);
    buttonIsPressed = true;
}


void Beeper::ButtonRelease()
{
    if (buttonIsPressed)
    {
        ::Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
        buttonIsPressed = false;
    }
}



void Beeper::GovernorChangedValue()
{
    ::Beep(TypeWave::Sine, 1000.0F, 0.5F, 50);
    buttonIsPressed = false;
}



void Beeper::RegulatorShiftRotate()
{
    ::Beep(TypeWave::Sine, 1.0F, 0.2F, 3);
    buttonIsPressed = false;
}



void Beeper::RegulatorSwitchRotate()
{
    ::Beep(TypeWave::Sine, 500.0F, 0.5F, 75);
    buttonIsPressed = false;
}



void Beeper::WarnBeepBad()
{
    ::Beep(TypeWave::Meandr, 500.0F, 1.0F, 500);
    soundWarnIsBeep = true;
    buttonIsPressed = false;
}
 

void Beeper::WarnBeepGood()
{
    ::Beep(TypeWave::Triangle, 1000.0F, 1.0F, 500);
    buttonIsPressed = false;
}


static void EmptyFunc()
{
}


void Beeper::Beep(TypePress::E type)
{
    static const pFuncVV func[TypePress::Count] =
    {
        Beeper::ButtonPress,
        EmptyFunc,
        Beeper::ButtonRelease,
        EmptyFunc,
        EmptyFunc
    };

    if (type < TypePress::Count)
    {
        func[type]();
    }
}


void Beeper::Test()
{
    ButtonPress();
    ButtonRelease();
    GovernorChangedValue();
    RegulatorShiftRotate();
    RegulatorSwitchRotate();
    WarnBeepBad();
    WarnBeepGood();
}
