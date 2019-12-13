#include "defines.h"
#include "common/Transceiver.h"
#include "FPGA/ContextTester.h"
#include "FPGA/MathFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Values.h"


static Settings oldSet = Settings::defaultSettings;

#ifdef OLD_VERSION
uint16 Tester::Pin_PNP     = HPin::_14;
uint16 Tester::Pin_U       = HPin::_15;
#else
uint16 Tester::Pin_PNP     = HPin::_6;
uint16 Tester::Pin_U       = HPin::_0;
#endif

int Tester::step = 0;
float Tester::stepU = 0.0F;
bool Tester::enabled = false;

static uint16 dataX[Tester::NUM_STEPS][TESTER_NUM_POINTS];  /// \todo Сделать так, чтобы при включении тестер-компонента необходимая память бралась из Heap.cpp
static uint8  dataY[Tester::NUM_STEPS][TESTER_NUM_POINTS];


void Tester::Init()
{
    for (int i = 0; i < Chan::Count; i++)
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

    HAL_PIO::Init(HPort::_A, HPin::_5, HMode::Analog, HPull::No);    // Настраиваем выходной порт

    //                                      PNP               U
    //uint pins = static_cast<uint>(Tester::Pin_PNP | Tester::Pin_U);
    //HAL_PIO::Init(Port_TEST_ON, pins, HMode::Output_PP, HPull::Down);

    HAL_PIO::Init(PIN_TESTER_ON, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_I, HMode::Output_PP, HPull::Down);
    HAL_PIO::Init(PIN_TESTER_STR, HMode::RisingIT, HPull::No);

    HAL_PIO::Set(PIN_TESTER_ON);         // Отключаем тестер-компонет

    HAL_DAC2::Init();

    Disable();
}


void Tester::DeInit()
{

}


void Tester::Update()
{
}


void Tester::Enable() // -V2506
{
    if(enabled)
    {
        return;
    }
    
    enabled = true;

    oldSet = set;

    ModeCouple(Chan::A).SetGND();
    ModeCouple(Chan::B).SetGND();

    Range(Chan::A).Set2V();
    Range(Chan::B).Set2V();

    RShift(Chan::A).Set(0);
    RShift(Chan::B).Set(0);

    HAL_PIO::Reset(PIN_TESTER_ON);       // Включаем тестер-компонент

    Osci::Stop();

    step = 0;                               // Первый пришедший импульс будет означать нулевой шаг

    LoadPolarity();

    LoadStep();

    HAL_NVIC::EnableIRQ(HAL_NVIC::irqEXTI9_5);    // Включаем прерывания от тактовых импульсов
}


int16 Tester::DeltaRShiftA()
{
    return 18;
}


void Tester::Disable() // -V2506
{
    if(!enabled)
    {
        return;
    }

    enabled = false;

    HAL_NVIC::DisableIRQ(HAL_NVIC::irqEXTI9_5);      // Выключаем прерывания от тактовых импульсов

    HAL_PIO::Set(PIN_TESTER_ON);

    oldSet.test.control = Control();
    oldSet.test.polarity = Polarity();
    oldSet.test.stepU = StepU();
    oldSet.test.stepI = StepI();

    set = oldSet;
    //set.Save();

    Osci::Init();

    ContextTester::OnPressStart();
}


void Tester::StartStop()
{
    if (enabled)
    {
        HAL_NVIC::DisableIRQ(HAL_NVIC::irqEXTI9_5);      // Выключаем прерывания от тактовых импульсов
        enabled = false;
    }
    else
    {
        HAL_NVIC::EnableIRQ(HAL_NVIC::irqEXTI9_5);    // Включаем прерывания от тактовых импульсов
        enabled = true;
    }
}


void Tester::ProcessStep()
{
                                                                                                                                                  /*
       |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |-----|     |---
       |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |     |
    ___|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|     |_____|

       | step == 0 |  2.5 мс   | step == 2 |  2.5 мс   | step == 4 |  2.5 мс   | step == 6 |  2.5 мс   | step == 8 |  2.5 мс   |
       |     0V    |  чтение   |   1 * dU  |  чтение   |   2 * dU  |  чтение   |   3 * dU  |  чтение   |  4 * dU   |  чтение   |
       |<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|<--------->|                  */


    if (Transceiver::InInteraction() || Timer::IsBusy())
    {
        return;
    }

    if ((step % 2) == 0)        // Если шаг кратен двум, то нужно устанавливать напряжение
    {
        HAL_DAC2::SetValue(static_cast<uint>(stepU * step / 2));
        // Запускаем ПЛИС для записи необходимого количества точек. Набор будет производиться в течение 2.5 мс (длительсность одного такта)
        if (!ContextTester::Start())
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


void Tester::ReadData()
{
    int halfStep = step / 2;

    uint16 *x = &dataX[halfStep][0];
    uint8 *y = &dataY[halfStep][0];

    if(ContextTester::Read(x, y))
    {
        RecountPoints(x, y);

        DisplayTester::SetPoints(halfStep, x, y);
    }
}


void Tester::RecountPoints(uint16 *x, uint8 *y)
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
        X = static_cast<int>(x0 + (X - x0) * scaleX);
        LIMITATION(X, 0, 319);
        x[i] = static_cast<uint16>(X);

        int Y = y[i] + dY;
        Y = static_cast<uint8>(y0 + (Y - y0) * scaleY);
        LIMITATION(Y, 0, 239);
        y[i] = static_cast<uint8>(Y);
    }
}


void Tester::LoadPolarity()
{
    // Устанавливаем полярность
    HAL_PIO::Write(Port_PNP, Pin_PNP, Polarity::IsPositive() ? HState::Enabled : HState::Disabled);
}


void Tester::LoadStep()
{
    // Устанавливаем управление напряжением или током
    HAL_PIO::Write(Port_U, Pin_U, Control::IsVoltage() ? HState::Enabled : HState::Disabled);

    HAL_PIO::Write(PIN_TESTER_I, Control::IsVoltage() ? HState::Disabled : HState::Enabled);

    if (Control::IsVoltage())
    {
        stepU =  255.0F / 3 * (StepU::Is500mV() ? 2 : 0.4F) / 5;
    }
    else
    {
        stepU = 255.0F / 3 * (StepI::Is20mA() ? 2 : 0.4F) / 5;
    }
}

INTERRUPT_BEGIN

void HAL_GPIO_EXTI_Callback(uint16 pin)
{
    if (pin == HPin::_9)      // Прерывание от тестер-компонента
    {
        Tester::ProcessStep();
    }
}

INTERRUPT_END

pString Tester::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Range::ToString(static_cast<Range::E>(value), Divider::_1);
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


String Tester::Shift::ToString(Scale::E scale) // -V2506
{
    if (ch == Chan::A)
    {
        return RShift::ToString(shift, static_cast<Range::E>(scale), Divider::_1);
    }

    float shiftAbs = RShift::ToAbs(shift, static_cast<Range::E>(scale)) * 1e-3F;

    return Current(shiftAbs).ToString();
}


Tester::Control::E &Tester::Control::Ref()
{
    return set.test.control;
}


Tester::Polarity::E &Tester::Polarity::Ref()
{
    return set.test.polarity;
}


Tester::StepU::E &Tester::StepU::Ref()
{
    return set.test.stepU;
}


Tester::StepI::E &Tester::StepI::Ref()
{
    return set.test.stepI;
}


Tester::ViewMode::E &Tester::ViewMode::Ref()
{
    return set.test.viewMode;
}


ENumAverage::E &Tester::ENUMAverage::Ref()
{
    return set.test.enumAverage;
}
