#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Osci/Osci.h"


static void DisplayUpdate();

// Нахоит сигнал на канале ch. Возвращает true, если сигнал найден и параметры сигнала в tBase, range
static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *range);

// Сжать сигнал, если не влазит в экран по вертикали
static void ToScaleChannel(Chan::E ch);

// Находит частоту на канале ch
static bool FindFrequency(Chan::E ch, float *outFreq);
static bool FindFrequencyForRanges(Chan::E ch, float *outFreq, uint timeWaitMS);

// Ожидает импульса синхронизации в течение timeWaitMS миллисекунд и возвращает true, если синхронизация пришла
static bool WaitSync(uint timeWaitMS);

// Рассчитывает TBase, необходимый для отображения задданой частоты
static TBase::E CalculateTBase(float frequency);


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

    TBase::E tBase = S_TIME_BASE;
    Range::E rangeA = S_RANGE_A;
    Range::E rangeB = S_RANGE_B;

    if (FindSignal(ChanA, &tBase, &rangeA))
    {
        set = old;
        S_TIME_BASE = tBase;
        S_RANGE_A = rangeA;
        TrigSource::Set(ChanA);

        Osci::Init();

        ToScaleChannel(ChanA);
        ToScaleChannel(ChanB);
    }
    else if (FindSignal(ChanA, &tBase, &rangeB))
    {
        set = old;
        S_TIME_BASE = tBase;
        S_RANGE_B = rangeB;
        TrigSource::Set(ChanB);

        Osci::Init();

        ToScaleChannel(ChanA);
        ToScaleChannel(ChanB);
    }
    else
    {
        DISPLAY_SHOW_WARNING("Сигнал не обнаружен");

        set = old;
        Osci::Init();
    }

    FreqMeter::Init();
}


static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *range)
{
    float frequency = 0.0F;

    if (FindFrequency(ch, &frequency))
    {
        *tBase = CalculateTBase(frequency);

        *range = Range::_500mV;

        return true;
    }

    return false;
}


static bool FindFrequency(Chan::E ch, float *outFreq)
{
    Osci::Stop();
    ModeCouple::Set(ch, ModeCouple::AC);
    TrigSource::Set(ch);
    TrigStartMode::Set(TrigStartMode::Wait);
    TrigLevel::Set(ch, 0);
    TBase::Set(TBase::_100ns);
    TShift::Set(0);
    RShift::Set(ch, 0);

    if (FindFrequencyForRanges(ch, outFreq, 150))
    {
        return true;
    }

    if (FindFrequencyForRanges(ch, outFreq, 1200))
    {
        return true;
    }

    return false;
}


static bool FindFrequencyForRanges(Chan::E ch, float *outFreq, uint timeWaitMS)
{
    FrequencyMeter::State::Store();

    FrequencyMeter::TuneForFind();

    for (int range = static_cast<int>(Range::_2mV); range < Range::Count; range++)
    {
        DisplayUpdate();

        Range::Set(ch, static_cast<Range::E>(range));

        if (WaitSync(timeWaitMS))
        {
            FreqMeter::FPGA::ReadCounterFreq();

            FPGA::Flag::Clear();

            *outFreq = 0.0F;

            return true;
        }
    }

    FrequencyMeter::State::Restore();

    return false;
}


void FrequencyMeter::TuneForFind()
{
    S_FREQ_METER_ENABLED = true;
    S_FREQ_TIME_COUNTING = FreqMeter::TimeCounting::_100ms;
    S_FREQ_FREQ_CLC = FreqMeter::FreqClc::_100MHz;
    S_FREQ_NUMBER_PERIODS = FreqMeter::NumberPeriods::_1;

    FreqMeter::FPGA::LoadSettings();

    FreqMeter::FPGA::ResetCounterFreq();

    FPGA::Flag::Clear();

    Osci::Start(false);

    do
    {
        FPGA::ReadFlag();
    } while (!FPGA::Flag::FreqReady());

    BitSet32 counter = FreqMeter::FPGA::ReadCounterFreq();

//    return counter.word * 10.0F;
}


static bool WaitSync(uint timeWaitMS)
{
    FPGA::Flag::Clear();

    Osci::Start(false);

    uint start = TIME_MS;           // Время начала ожидания

    while (!FPGA::Flag::TrigReady())
    {
        if (TIME_MS - start > timeWaitMS)
        {
            return false;
        }

        FPGA::ReadFlag();
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
        Text(".").Draw(320 / 2 - width / 2 + i * dX, 120);
    }

    Painter::EndScene();
}


static void ToScaleChannel(Chan::E)
{

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
        {10.0F,  TBase::_10ms},
        {30.0F,  TBase::_5ms},
        {80.0F,  TBase::_2ms},

        {100.0F, TBase::_1ms},
        {300.0F, TBase::_500us},
        {800.0F, TBase::_200us},

        {1e3F,   TBase::_100us},
        {3e3F,   TBase::_50us},
        {8e3F,   TBase::_20us},

        {10e3F,  TBase::_10us},
        {30e3F,  TBase::_5us},
        {80e3F,  TBase::_2us},

        {100e3F, TBase::_1us},
        {300e3F, TBase::_500ns},
        {800e3F, TBase::_200ns},

        {1e6F,   TBase::_100ns},
        {3e6F,   TBase::_50ns},
        {8e6F,   TBase::_20ns},

        {10e6F,  TBase::_10ns},
        {30e6F,  TBase::_5ns},
        {250e6F, TBase::_2ns},
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
