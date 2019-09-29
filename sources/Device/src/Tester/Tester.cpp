#include "defines.h"
#include "Transceiver.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include <stm32f4xx_hal.h>
#include "Hardware/Timer.h"
#include "Data/Reader.h"
#include "Utils/Math.h"
#include "FPGA/FPGA_Math.h"
#include "Utils/Values.h"


using namespace Osci::Settings;


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

    HAL::PIO::Init(HAL::PIO::Port::_A, HAL::PIO::Pin::_5, HAL::PIO::Mode::Analog, HAL::PIO::Pull::No);    // Настраиваем выходной порт

    //                         TEST_ON               PNP               U
    uint pins = (uint)(Tester::Pin_TEST_ON | Tester::Pin_PNP | Tester::Pin_U);
    HAL::PIO::Init(Port_TEST_ON, pins, HAL::PIO::Mode::Output_PP, HAL::PIO::Pull::Down);

    //                               I
    HAL::PIO::Init(Port_I, Tester::Pin_I, HAL::PIO::Mode::Output_PP, HAL::PIO::Pull::Down);

    //              TEST_STR - EXTI9
    HAL::PIO::Init(Port_TEST_STR, Tester::Pin_TEST_STR, HAL::PIO::Mode::RisingIT, HAL::PIO::Pull::No);

    HAL::PIO::Set(Port_TEST_ON, Tester::Pin_TEST_ON);         // Отключаем тестер-компонет

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

    set.ch[Chan::A].couple = set.ch[Chan::B].couple = ModeCouple::GND;

    set.ch[Chan::A].range = Range::_2V;
    set.ch[Chan::B].range = Range::_2V;

    Range::LoadBoth();

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

    oldSet.test.control = set.test.control;
    oldSet.test.polarity = set.test.polarity;
    oldSet.test.stepU = set.test.stepU;
    oldSet.test.stepI = set.test.stepI;

    set = oldSet;
    //set.Save();

    Osci::Init();

    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::StartStop()
{
    if (enabled)
    {
        HAL::NVIC_::DisableIRQ(HAL::NVIC_::irqEXTI9_5);      // Выключаем прерывания от тактовых импульсов
        enabled = false;
    }
    else
    {
        HAL::NVIC_::EnableIRQ(HAL::NVIC_::irqEXTI9_5);    // Включаем прерывания от тактовых импульсов
        enabled = true;
    }
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
        if (!FPGA::ForTester::Start())
        {
            return;
        }
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
static void ReadData()
{
    int halfStep = step / 2;

    uint16 *x = &dataX[halfStep][0];
    uint8 *y = &dataY[halfStep][0];

    if(FPGA::ForTester::Read(x, y))
    {
        RecountPoints(x, y);

        Tester::Display::SetPoints(halfStep, x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RecountPoints(uint16 *x, uint8 *y)
{
    static const float scaleX = 332.0F / 240.0F;
    static const float scaleY = 249.0F / 255.0F;

    static const int dX = 20;
    static const int dY = -2;

    static const int x0 = 160;
    static const int y0 = 120;

    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        int X = 255 - x[i] + dX;
        X = (int)(x0 + (X - x0) * scaleX);
        LIMITATION(X, 0, 319); //-V2516
        x[i] = (uint16)X;

        int Y = y[i] + dY;
        Y = (uint8)(y0 + (Y - y0) * scaleY);
        LIMITATION(Y, 0, 239); //-V2516
        y[i] = (uint8)Y;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadPolarity()
{
    // Устанавливаем полярность
    HAL::PIO::Write(Port_PNP, Pin_PNP, (set.test.polarity == TesterPolarity::Positive) ? State::Enabled : State::Disabled);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tester::LoadStep()
{
    // Устанавливаем управление напряжением или током
    HAL::PIO::Write(Port_U, Pin_U, (set.test.control == TesterControl::Voltage) ? State::Enabled : State::Disabled);

    HAL::PIO::Write(Port_I, Pin_I, (set.test.control == TesterControl::Voltage) ? State::Disabled : State::Enabled);

    if (set.test.control == TesterControl::Voltage)
    {
        stepU =  255.0F / 3 * ((set.test.stepU == TesterStepU::_500mV) ? 2 : 0.4F) / 5;
    }
    else
    {
        stepU = 255.0F / 3 * ((set.test.stepI == TesterStepI::_20mA) ? 2 : 0.4F) / 5;
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Tester::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Range((Range::E)value).ToString(Divider::_1);
    }

    static const pString names[] =
    {
        "2мкА",
        "5мкА",
        "10мкА",
        "20мкА",
        "50мкА",
        "100мкА",
        "200мкА",
        "500мкА",
        "1мА",
        "2мА",
        "5мА",
        "10мА",
        "20мА"
    };

    pString name = names[value];

    if (name)
    {
        return names[value];
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

    float shiftAbs = FPGA::Math::RShift2Abs(shift, (Range::E)scale) * 1e-3F;

    return Current(shiftAbs).ToString();
}
