// 2023/11/22 13:46:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Utils/Math.h"


static void DisplayUpdate();

// Нахоит сигнал на канале ch. Возвращает true, если сигнал найден и параметры сигнала в tBase, range
static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *range, ModeCouple::E *couple);
static bool FullSearchSignal(Chan::E ch, const Settings *old);

// Отмасштабировать сигнал. Если onlyReduce - только сжать
static void ScaleChannel(Chan::E ch, bool onlyReduce);

// Находит частоту на канале ch
static bool FindFrequency(Chan::E ch, float *outFreq, Range::E *outRange, ModeCouple::E *couple);
static bool FindFrequencyForRanges(Chan::E ch, uint timeWaitMS, float *outFreq, Range::E *outRange);
static bool FindFrequencyForRange(Chan::E ch, Range::E range, uint timeWaitMS, float *outFreq);

// Ожидает импульса синхронизации в течение timeWaitMS миллисекунд и возвращает true, если синхронизация пришла
static bool WaitSync(uint timeWaitMS);

// Рассчитывает TBase, необходимый для отображения задданой частоты
static TBase::E CalculateTBase(float frequency);


namespace AutoFPGA
{
    static const int SIZE = 300;
    void Start();
    void ReadData(Chan::E ch, uint8 *data);

    // Прочитать данные и проверить, что они не выходят за пределы минимального и максимального. Если false - выход за экран
    float ReadAndCalculateMinMax(Chan::E ch);

    void Wait(uint timeMS);
}


namespace FrequencyMeter
{
    // Найти частоту по счётчику частоты
    void TuneForFind();
    
    namespace State
    {
        // Сохранить состояние частотомера перед нашими измерениями
        void Store();

        // Восстановить состояние частотомера после наших измерений
        void Restore();

        bool enabled = S_FREQ_METER_ENABLED;
        FreqMeter::TimeCounting::E counting = S_FREQ_TIME_COUNTING;
        FreqMeter::FreqClc::E clc = S_FREQ_FREQ_CLC;
        FreqMeter::NumberPeriods::E periods = S_FREQ_NUMBER_PERIODS;
    }
}


void Osci::RunAutoSearch()
{
    Settings old = set;

    if (!FullSearchSignal(ChanA, &old))
    {
        if (!FullSearchSignal(ChanB, &old))
        {
            DISPLAY_SHOW_WARNING("Сигнал не обнаружен");

            set = old;
            Osci::Init();
        }
    }

    FreqMeter::Init();
}


static bool FullSearchSignal(Chan::E ch, const Settings *old)
{
    TBase::E tBase = TBase::Count;
    Range::E range = Range::Count;
    ModeCouple::E couple = ModeCouple::Count;

    if (FindSignal(ch, &tBase, &range, &couple))
    {
        set = *old;
        S_TIME_BASE = tBase;
        S_RANGE(ch) = range;
        S_MODE_COUPLE(ch) = couple;
        TrigSource::Set(ch);
        RShift::Set(ch, 0);
        TrigLevel::Set(ch, 0);
        TrigStartMode::Set(TrigStartMode::Wait);

        Osci::Init();

        ScaleChannel(ChanA, ch == Chan::B);
        ScaleChannel(ChanB, ch == Chan::A);

        Osci::Init();

        return true;
    }

    return false;
}


static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *outRange, ModeCouple::E *couple)
{
    float frequency = 0.0F;

    if (FindFrequency(ch, &frequency, outRange, couple))
    {
        *tBase = CalculateTBase(frequency);

        return true;
    }

    return false;
}


static bool FindFrequency(Chan::E ch, float *outFreq, Range::E *outRange, ModeCouple::E *couple)
{
    Osci::Stop();
    ModeCouple::Set(ch, ModeCouple::AC);
    TrigSource::Set(ch);
    TrigStartMode::Set(TrigStartMode::Wait);
    TrigLevel::Set(ch, 0);
    TBase::Set(TBase::_100ns);
    TShift::Set(0);
    RShift::Set(ch, 0);
    S_TRIG_INPUT = TrigInput::Full;
    TrigInput::Load();

    if (FindFrequencyForRanges(ch, 150, outFreq, outRange))
    {
        *couple = ModeCouple::AC;
        return true;
    }

    ModeCouple::Set(ch, ModeCouple::DC);

    if (FindFrequencyForRanges(ch, 1200, outFreq, outRange))
    {
        *couple = ModeCouple::DC;
        return true;
    }

    return false;
}


static bool FindFrequencyForRanges(Chan::E ch, uint timeWaitMS, float *outFreq, Range::E *outRange)
{
    bool result = false;

    FrequencyMeter::State::Store();

    FrequencyMeter::TuneForFind();

    for (int range = static_cast<int>(Range::_20V); range >= 0; range -= 2)
    {
        float frequency1 = 0.0F;

        if (FindFrequencyForRange(ch, static_cast<Range::E>(range), timeWaitMS, &frequency1))
        {
            float frequency2 = 0.0F;

            if (FindFrequencyForRange(ch, static_cast<Range::E>(range), timeWaitMS, &frequency2))
            {
                if (Math::FloatsIsEquals(frequency1, frequency2, 0.1F))
                {
                    *outFreq = (frequency1 + frequency2) / 2.0F;
                    *outRange = static_cast<Range::E>(range > 0 ? (range - 1) : range);
                    result = true;
                    break;
                }
            }
        }
    }

    FrequencyMeter::State::Restore();

    return result;
}


static bool FindFrequencyForRange(Chan::E ch, Range::E range, uint timeWaitMS, float *outFreq)
{
    DisplayUpdate();

    Range::Set(ch, range);

    AutoFPGA::Wait(100);

    FPGA::Flag::Clear();

    Osci::Start(false);

    if (WaitSync(timeWaitMS))
    {
        do
        {
            DisplayUpdate();
            FPGA::Flag::Read(false);
        } while (!FPGA::Flag::FreqReady());

        BitSet32 counterFreq = FreqMeter::FPGA::ReadCounterFreq();

        if (counterFreq.word > 10)
        {
            *outFreq = counterFreq.word * 10.0F;
        }
        else
        {
            while (!FPGA::Flag::PeriodReady())
            {
                DisplayUpdate();
                FPGA::Flag::Read(false);
            }

            BitSet32 counterPeriod = FreqMeter::FPGA::ReadCounterPeriod();

            *outFreq = 1.0F / (counterPeriod.word * 10e-9F);
        }

        return true;
    }

    return false;
}


void FrequencyMeter::TuneForFind()
{
    S_FREQ_METER_ENABLED = true;
    S_FREQ_TIME_COUNTING = FreqMeter::TimeCounting::_100ms;
    S_FREQ_FREQ_CLC = FreqMeter::FreqClc::_100MHz;
    S_FREQ_NUMBER_PERIODS = FreqMeter::NumberPeriods::_1;

    FreqMeter::FPGA::_LoadSettings();
}


static bool WaitSync(uint timeWaitMS)
{
    uint start = TIME_MS;           // Время начала ожидания

    while (!FPGA::Flag::TrigReady())
    {
        if (TIME_MS - start > timeWaitMS)
        {
            return false;
        }

        FPGA::Flag::Read();

        DisplayUpdate();
    }

    return true;
}


static void DisplayUpdate()
{
    Painter::BeginScene(Color::BACK);

    Text("Поиск сигнала").DrawInCenterRect(0, 0, 320, 200, Color::FILL);

    int length = static_cast<int>((TIME_MS / 250) % 20);

    int dX = 5;

    int width = dX * length;

    for (int i = 0; i < length; i++)
    {
        Text(".").Draw(320 / 2 - width / 2 + i * dX + 2, 120);
    }

    Painter::EndScene();
}


static TBase::E CalculateTBase(float frequency)
{
    struct TimeStruct
    {
        float       frequency;  // Частота, ниже которой нужно устанавливать tBase
        TBase::E    tBase;
    };

    static const TimeStruct times[] =
    {
        {2.0F,   TBase::_200ms},
        {3.0F,   TBase::_100ms},
        {5.0F,   TBase::_50ms},
        {30.0F,  TBase::_20ms},
        {60.0F,  TBase::_5ms},
        {80.0F,  TBase::_2ms},

        {300.0F, TBase::_1ms},
        {600.0F, TBase::_500us},
        {800.0F, TBase::_200us},

        {3e3F,   TBase::_100us},
        {6e3F,   TBase::_50us},
        {8e3F,   TBase::_20us},

        {30e3F,  TBase::_10us},
        {60e3F,  TBase::_5us},
        {80e3F,  TBase::_2us},

        {300e3F, TBase::_1us},
        {600e3F, TBase::_500ns},
        {800e3F, TBase::_200ns},

        {3e6F,   TBase::_100ns},
        {6e6F,   TBase::_50ns},
        {15e6F,  TBase::_20ns},

        {30e6F,  TBase::_10ns},
        {60e6F,  TBase::_5ns},
        {0.0F,   TBase::Count}
    };

    for (int i = 0; times[i].tBase != TBase::Count; i++)
    {
        if (frequency < times[i].frequency)
        {
            return times[i].tBase;
        }
    }

    return TBase::_2ns;
}


void FrequencyMeter::State::Store()
{
    enabled = S_FREQ_METER_ENABLED;
    counting = S_FREQ_TIME_COUNTING;
    clc = S_FREQ_FREQ_CLC;
    periods = S_FREQ_NUMBER_PERIODS;
}


void FrequencyMeter::State::Restore()
{
    S_FREQ_METER_ENABLED = enabled;
    S_FREQ_TIME_COUNTING = counting;
    S_FREQ_FREQ_CLC = clc;
    S_FREQ_NUMBER_PERIODS = periods;

    FreqMeter::Init();
}


static void ScaleChannel(Chan::E ch, bool onlyReduce)
{
    float delta = AutoFPGA::ReadAndCalculateMinMax(ch);

    while (delta > 0.8F && S_RANGE(ch) < Range::_20V)
    {
        Range::Change(ch, 1);
        delta = AutoFPGA::ReadAndCalculateMinMax(ch);
    }

    if (!onlyReduce)
    {
        while (delta < 0.4F && S_RANGE(ch) > Range::_2mV)
        {
            Range::Change(ch, -1);           
            delta = AutoFPGA::ReadAndCalculateMinMax(ch);
        }
    }
}


float AutoFPGA::ReadAndCalculateMinMax(Chan::E ch)
{
    Buffer buffer(AutoFPGA::SIZE);

    Osci::Init();

    AutoFPGA::Wait(100);

    AutoFPGA::Start();

    do
    {
        DisplayUpdate();
        FPGA::Flag::Read();

    } while (!FPGA::Flag::DataReady());

    AutoFPGA::ReadData(ch, buffer.data);

    uint8 max = Math::MaxFromArray(buffer.data, 10, 290);
    uint8 min = Math::MinFromArray(buffer.data, 10, 290);

    float delta = static_cast<float>(max - min) / static_cast<float>(VALUE::MAX - VALUE::MIN);

    return delta;
}


void AutoFPGA::Wait(uint timeMS)
{
    uint start = TIME_MS;

    while (TIME_MS - start < timeMS)
    {
        DisplayUpdate();
    }
}


void AutoFPGA::Start()
{
    FPGA::GiveStart(static_cast<uint16>(~(1)), static_cast<uint16>(~(SIZE + 5)));

    for (int i = 0; i < 50; i++)
    {
        FPGA::Flag::Read();
        if (FPGA::Flag::Pred())
        {
            break;
        }
    }

    FPGA::ForcedStart();
}


void AutoFPGA::ReadData(Chan::E ch, uint8 *data)
{
    uint16 addrRead = static_cast<uint16>(Osci::ReadLastRecord(ch) - SIZE);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, addrRead);
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);

    uint8 *a0 = (ch == ChanA) ? RD::DATA_A : RD::DATA_B;
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    uint8 *p = data;
    *p = HAL_BUS::FPGA::ReadA1();

    for (int i = 0; i < SIZE; i++)
    {
        *p = HAL_BUS::FPGA::ReadA1();
        p++;
    }
}
