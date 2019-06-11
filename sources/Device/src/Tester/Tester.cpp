#include "defines.h"
#include "Transceiver.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>
#include "Hardware/Timer.h"
#include "Data/Reader.h"


using namespace FPGA::Settings;
using namespace Osci::Settings;
using namespace Tester::Settings;


using HAL::FSMC;
using HAL::PIO::State;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static Settings oldSet = Settings::defaultSettings;

uint16 Tester::Pin_TEST_ON = HAL::PIO::Pin::_13;
uint16 Tester::Pin_PNP = HAL::PIO::Pin::_14;
uint16 Tester::Pin_U = HAL::PIO::Pin::_15;
uint16 Tester::Pin_I = HAL::PIO::Pin::_0;
uint16 Tester::Pin_TEST_STR = HAL::PIO::Pin::_9;

/// Загрузить FPGA в соответствии с установленными настройками
static void LoadFPGA();
/// Считать данные очередной ступеньки
static void ReadData();
/// Пересчитать точки для засылки отрисовки
static void RecountPoints(uint16 *x, uint8 *y);
/// Произвести смещение данных для отрисовки строго по центру
static void ShiftData(uint16 *x, uint8 *y);
/// Текущий шаг
static int step = 0;
/// Шаг изменения напряжения
static float stepU = 0.0F;
/// Установленное в true значение означает, что вклюён режим тестера
static bool enabled = false;

static uint16 dataX[Tester::NUM_STEPS][TESTER_NUM_POINTS];  /// \todo Сделать так, чтобы при включении тестер-компонента необходимая память бралась из Heap.cpp
static uint8  dataY[Tester::NUM_STEPS][TESTER_NUM_POINTS];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Tester
{
    class DAC2_
    {
    public:
        static void Init()
        {
            HAL::DAC2_::Init();
        }
        static void SetValue(uint value)
        {
            HAL::DAC2_::SetValue(value);
        }
    };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Tester::Init()
{
    for (int i = 0; i < Chan::Size; i++)
    {
        for (int j = 0; j < NUM_STEPS; j++)
        {
            for (int k = 0; k < TESTER_NUM_POINTS; k++)
            {
                dataX[j][k] = 0;
                dataY[j][k] = 0;
            }
        }
    }

    DAC2_::Init();

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

    HAL::PIO::Reset(Port_TEST_ON, Pin_TEST_ON);       // Включаем тестер-компонент

    LoadFPGA();

    Osci::Stop();

    step = 0;                               // Первый пришедший импульс будет означать нулевой шаг

    LoadPolarity();

    LoadStep();

    HAL::NVIC_::EnableIRQ(HAL::NVIC_::irqEXTI9_5);    // Включаем прерывания от тактовых импульсов
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

    HAL::NVIC_::DisableIRQ(HAL::NVIC_::irqEXTI9_5);      // Выключаем прерывания от тактовых импульсов

    HAL::PIO::Set(Port_TEST_ON, Pin_TEST_ON);

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
static void LoadFPGA()
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

    if (Transceiver::InInteraction() || 
        Timer::IsBusy())
    {
        return;
    }

    if ((step % 2) == 0)        // Если шаг кратен двум, то нужно устанавливать напряжение
    {
        DAC2_::SetValue((uint)(stepU * step / 2));
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
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//static void AverageData(const uint8 *dataNew, uint16 *dataAve)
//{
//    uint8 *_new = (uint8 *)dataNew;
//    uint16 *av = dataAve;
//
//    uint16 numAve = 1;
//
//    for (int i = 0; i < TESTER_NUM_POINTS; i++)
//    {
//        av[i] = (uint16)(av[i] - (av[i] >> numAve));
//
//        av[i] += *_new;
//
//        *_new = (uint8)(av[i] >> numAve);
//
//        _new++;
//    }
//}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadData()
{
    int halfStep = step / 2;

    uint16 *x = &dataX[halfStep][0];
    uint8 *y = &dataY[halfStep][0];

//    uint16 *av[2][5] =
//    {
//        {AVE_1, AVE_1 + 240, AVE_1 + 240 * 2, AVE_1 + 240 * 3, AVE_1 + 240 * 4},
//        {AVE_2, AVE_2 + 240, AVE_2 + 240 * 2, AVE_2 + 240 * 3, AVE_2 + 240 * 4}
//    };

    if(FPGA::ForTester::Read(x, y))
    {
        //AverageData(x, av[0][halfStep]);
        //AverageData(y, av[1][halfStep]);

        RecountPoints(x, y);

        Tester::Display::SetPoints(halfStep, x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RecountPoints(uint16 *x, uint8 *y)
{
    ShiftData(x, y);

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        x[i] = (uint16)(x[i] * 320.0F / 240.0F);
        y[i] = (uint8)(y[i] * 240.0F / 255.0F);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ShiftData(uint16* x, uint8* y)
{
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        int X = x[i] + 7;
        if (X > 255)
        {
            X = 255;
        }
        x[i] = (uint8)X;

        int Y = y[i] + 6;
        if (Y > 255)
        {
            Y = 255;
        }
        y[i] = (uint8)Y;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadPolarity()
{
    // Устанавливаем полярность
    HAL::PIO::Write(Port_PNP, Pin_PNP, TESTER_POLARITY_IS_POSITITVE ? State::Enabled : State::Disabled);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadStep()
{
    // Устанавливаем управление напряжением или током
    HAL::PIO::Write(Port_U, Pin_U, TESTER_CONTROL_IS_U ? State::Enabled : State::Disabled);

    HAL::PIO::Write(Port_I, Pin_I, TESTER_CONTROL_IS_U ? State::Disabled : State::Enabled);

    if (TESTER_CONTROL_IS_U)
    {
        stepU =  255.0F / 3 * ((TESTER_STEP_U == StepU::_500mV) ? 2 : 0.4F) / 5;
    }
    else
    {
        stepU = 255.0F / 3 * ((TESTER_STEP_I == StepI::_20mA) ? 2 : 0.4F) / 5;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    if (pin == HAL::PIO::Pin::_9)      // Прерывание от тестер-компонента
    {
        Tester::ProcessStep();
    }
}
