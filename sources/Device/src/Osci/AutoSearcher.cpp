#include "defines.h"
#include "FPGA/FPGA.h"
#include "FreqMeter/FreqMeter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Utils/Values.h"


// Попытка найти сигнал на канале ch. Если попытка удалась, то найденные параметры загружаются в прибор
static bool FindSignal(Chan::E ch);

// Находит частоту сигнала, поданного на вход ch. В случае неудачи возвращает значение Float::ERROR
static bool FindFrequency(Chan::E ch, float *outFreq);
static bool FindFrequency(Chan::E ch, Range::E range, float *outFreq);

// Находит масштаб по вертикаил
static Range::E FindRange(Chan::E ch);

// Рассчитывает TBase, необходимый для отображения задданой частоты
static TBase::E CalculateTBase(float frequency);

// Заслать соответствующие настройки в частотомер
static void TuneFreqMeter();

// Рассчитывает частоту, исходя из данных аппаратного частотомера
static bool CalculateFrequency(float *outFreq);


void Osci::RunAutoSearch()
{
    Settings old = set;

    if (!FindSignal(ChanA))
    {
        set = old;

        if (!FindSignal(ChanB))
        {
            set = old;

            DISPLAY_SHOW_WARNING("Сигнал не обнаружен");
        }
    }

    Osci::Init();
}


static bool FindSignal(Chan::E ch)
{
    float frequency = 0.0F;

    if (FindFrequency(ch, &frequency))
    {
        TBase::Set(CalculateTBase(frequency));

        //Range::Set(ch, FindRange(ch));

        return true;
    }

    return false;
}


static bool FindFrequency(Chan::E ch, float *outFreq)
{
    ModeCouple::Set(ch, ModeCouple::AC);
    TrigSource::Set(ch);
    TrigStartMode::Set(TrigStartMode::Wait);
    TrigLevel::Set(ch, 0);
    TBase::Set(TBase::_100ns);
    TShift::Set(0);
    RShift::Set(ch, 0);
    
    bool result = false;

    for (int range = static_cast<int>(Range::_20V); range >= 0 && !result; range--)
    {
        result = FindFrequency(ch, static_cast<Range::E>(range), outFreq);
    }

    return result;
}


static bool FindFrequency(Chan::E ch, Range::E range, float *outFreq)
{
    /*
        Находим сигнал с помощью частотомера
        1. Устанавливаем один периода в параметрах частотомера
        2. Читаем счётчики, тем самым обнуляя их.
        3. Даём запуск.
        4. Ждём, пока появится флаг частоты или периода
    */

    Settings old = set;

    Osci::Stop();
    Range::Set(ch, range);

    Timer::PauseOnTime(500);

    TuneFreqMeter();
    
    Osci::Start(false);

    static int counter = 0;
    
    do
    {
        counter++;
        
        FPGA::ReadFlag();

    } while (!FPGA::Flag::PeriodReady() || !FPGA::Flag::FreqReady());

    counter = counter;
    
    set = old;

    return CalculateFrequency(outFreq);
}


Range::E FindRange(Chan::E)
{
    return Range::Count;
}


static void TuneFreqMeter()
{
    S_FREQ_METER_ENABLED = true;
    S_FREQ_TIME_COUNTING = FreqMeter::TimeCounting::_1s;
    S_FREQ_FREQ_CLC = FreqMeter::FreqClc::_100MHz;
    S_FREQ_NUMBER_PERIODS = FreqMeter::NumberPeriods::_1;

    FreqMeter::FPGA::LoadSettings();
    FreqMeter::FPGA::ResetCounterFreq();
    FreqMeter::FPGA::ResetCounterPeriod();

    FPGA::Flag::Clear();
}


static bool CalculateFrequency(float *outFreq)
{
    if (!FPGA::Flag::FreqOverflow() && FPGA::Flag::FreqReady())
    {
        BitSet32 counter = FreqMeter::FPGA::ReadCounterFreq();
        uint freq = counter.word;
        freq = freq;

        return true;
    }

    if (!FPGA::Flag::PeriodOverflow() && FPGA::Flag::PeriodReady())
    {
        BitSet32 counter = FreqMeter::FPGA::ReadCounterPeriod();
        counter = counter;

        return true;
    }

    return false;
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
