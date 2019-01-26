#include <stm32f4xx_hal.h>
#include "defines.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Hardware.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"

#include "Hardware/HAL/HAL.h"


using namespace FPGA::Settings;
using namespace Osci::Settings;

using HAL::FSMC;
using HAL::PORTS::State;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static DAC_HandleTypeDef hDAC = {DAC};

uint8 Tester::data[Chan::Size][NUM_STEPS][TESTER_NUM_POINTS];

static Settings oldSet = Settings::defaultSettings;

int   Tester::step = 0;
float Tester::stepU = 0.0F;
bool  Tester::enabled = false;

uint16 Tester::Pin_TEST_ON = HAL::PORTS::Pin::_13;
uint16 Tester::Pin_PNP = HAL::PORTS::Pin::_14;
uint16 Tester::Pin_U = HAL::PORTS::Pin::_15;
uint16 Tester::Pin_I = HAL::PORTS::Pin::_0;
uint16 Tester::Pin_TEST_STR = HAL::PORTS::Pin::_9;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Init()
{
    for (int i = 0; i < Chan::Size; i++)
    {
        for (int j = 0; j < NUM_STEPS; j++)
        {
            for (int k = 0; k < TESTER_NUM_POINTS; k++)
            {
                Tester::data[i][j][k] = 0;
            }
        }
    }

    HAL::DAC2_::Init();

    Disable();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Update()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Enable() // -V2506
{
    if(enabled)
    {
        return;
    }
    
    enabled = true;

    oldSet = set;

    SET_COUPLE_A = SET_COUPLE_B = ModeCouple::GND;

    SET_RANGE(Chan::A) = Osci::Settings::Range::_2V;
    SET_RANGE(Chan::B) = Osci::Settings::Range::_2V;

    Osci::Settings::Range::LoadBoth();

    RShift::Set(Chan::A, RShift::ZERO);
    RShift::Set(Chan::B, RShift::ZERO);

    HAL::PORTS::Reset(Port_TEST_ON, Pin_TEST_ON);       // Включаем тестер-компонент

    LoadFPGA();

    Osci::Stop();

    step = 0;                               // Первый пришедший импульс будет означать нулевой шаг

    LoadPolarity();

    LoadStep();

    HAL_NVIC_EnableIRQ(EXTI9_5_IRQn);       // Включаем прерывания от тактовых импульсов
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Tester::DeltaRShiftA()
{
    return 18;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::Disable() // -V2506
{
    if(!enabled)
    {
        return;
    }

    enabled = false;

    HAL_NVIC_DisableIRQ(EXTI9_5_IRQn);      // Выключаем прерывания от тактовых импульсов

    HAL::PORTS::Set(Port_TEST_ON, Pin_TEST_ON);

    oldSet.test_control = TESTER_CONTROL;
    oldSet.test_polarity = TESTER_POLARITY;
    oldSet.test_stepU = TESTER_STEP_U;
    oldSet.test_stepI = TESTER_STEP_I;

    set = oldSet;
    //set.Save();

    Osci::Init();

    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadFPGA()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::ProcessStep()
{
                                                                                                                                                  /*
       |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |---
       |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
    ___|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|

       | step == 0 |  2.5 мс   | step == 2 |  2.5 мс   | step == 4 |  2.5 мс   | step == 6 |  2.5 мс   | step == 8 |  2.5 мс   |
       |     0V    |  чтение   |   1 * dU  |  чтение   |   2 * dU  |  чтение   |   3 * dU  |  чтение   |  4 * dU   |  чтение   |
       |<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|                  */


    static int success = 0;
    static int fail = 0;

    if (!FSMC::InterchangeWithPanel())   // Если идёт процесс обмена с панелью, отложим на следующий так во избежание артефактов и зависаний.
    {
        if ((step % 2) == 0)        // Если шаг кратен двум, то нужно устанавливать напряжение
        {
            HAL_DAC_SetValue(&hDAC, DAC1_CHANNEL_2, DAC_ALIGN_8B_R, (uint)(stepU * step / 2));  // -V2004
            // Запускаем ПЛИС для записи необходимого количества точек. Набор будет производиться в течение 2.5 мс (длительсность одного такта)
            FPGA::ForTester::Start();
        }
        else
        {
            ReadData();
        }

        ++step;

        if (step == 10)
        {
            step = 0;
        }
        ++success;
    }
    else
    {
        ++fail;
        FSMC::RunFunctionAfterInteractionWitchPanel(&Tester::ProcessStep);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::ReadData()
{
    uint8 *x = &data[Chan::A][step / 2][0];
    uint8 *y = &data[Chan::B][step / 2][0];

    if(FPGA::ForTester::Read(x, y))
    {
        Display::SetPoints(step / 2, x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16_t pin)
{
    if (pin == GPIO_PIN_9)      // Прерывание от тестер-компонента
    {
        Tester::ProcessStep();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadPolarity()
{
    // Устанавливаем полярность
    HAL::PORTS::Write(Port_PNP, Pin_PNP, TESTER_POLARITY_IS_POSITITVE ? State::Enabled : State::Disabled);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadStep()
{
    // Устанавливаем управление напряжением или током
    HAL::PORTS::Write(Port_U, Pin_U, TESTER_CONTROL_IS_U ? State::Enabled : State::Disabled);

    HAL::PORTS::Write(Port_I, Pin_I, TESTER_CONTROL_IS_U ? State::Disabled : State::Enabled);

    if (TESTER_CONTROL_IS_U)
    {
        stepU =  255.0F / 3 * ((TESTER_STEP_U == Tester::StepU::_500mV) ? 2 : 0.4F) / 5;
    }
    else
    {
        stepU = 255.0F / 3 * ((TESTER_STEP_I == Tester::StepI::_20mA) ? 2 : 0.4F) / 5;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Tester::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Range((Osci::Settings::Range::E)value).ToString(Divider::_1);
    }

    static const pString names[][Language::Size] =
    {
        "2мкА",     "2uA",
        "5мкА",     "5uА",
        "10мкА",    "10uА",
        "20мкА",    "20uА",
        "50мкА",    "50uА",
        "100мкА",   "100uА",
        "200мкА",   "200uА",
        "500мкА",   "500uА",
        "1мА",      "1mА",
        "2мА",      "2mА",
        "5мА",      "5mА",
        "10мА",     "10mА",
        "20мА",     "20mА"
    };

    pString name = names[value][LANG];

    if (name)
    {
        return names[value][LANG];
    }
    else
    {
        LOG_ERROR("Массив не заполнен");
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Tester::Shift::ToString(Scale::E scale) // -V2506
{
    if (ch == Chan::A)
    {
        return RShift::ToString(shift, (Range::E)scale, Divider::_1);
    }

    float shiftAbs = FPGA::Math::RShift2Abs(shift,  (Range::E)scale) * 1e-3F;

    return Current(shiftAbs).ToString();
}
