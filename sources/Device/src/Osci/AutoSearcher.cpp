#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "FreqMeter/FreqMeter.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Utils/Values.h"


// Попытка найти сигнал на канале ch. Если попытка удалась, то найденные параметры загружаются в прибор
static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *range);

// Находит частоту сигнала, поданного на вход ch
static bool FindFrequency(Chan::E ch, float *outFreq);
//static bool FindFrequency(Chan::E ch, Range::E range, float *outFreq);

// Находит одну синхронизацию и возвращает true в случае успеха и range, при котором она найдена
//static bool FindOneSync(Chan::E ch, Range::E range, uint timeWait);

// Ищет две синхронизации во временном промежутке timeWait.
// Если не найдено не одной, возращает -1
// Если найдена только одна возрващает 0
// Если найдены две, возвращает время в микросекундах между ними (но не менее 1)
static uint FindTwoSync(Chan::E ch, Range::E range, uint timeWait);

// Находит масштаб по вертикаил
//static Range::E FindRange(Chan::E ch);

// Рассчитывает TBase, необходимый для отображения задданой частоты
static TBase::E CalculateTBase(float frequency);

// Заслать соответствующие настройки в частотомер
//static void TuneFreqMeter();


static void DisplayUpdate();


void Osci::_RunAutoSearch()
{
    Settings old = set;

    TBase::E tBase = S_TIME_BASE;;
    Range::E rangeA = S_RANGE_A;
    Range::E rangeB = S_RANGE_B;

    Chan::E source = Chan::Count;

    if (FindSignal(ChanA, &tBase, &rangeA))
    {
        source = ChanA;
    }
    else if (FindSignal(ChanB, &tBase, &rangeB))
    {
        source = ChanB;
    }
    else
    {
        DISPLAY_SHOW_WARNING("Сигнал не обнаружен");
    }

    set = old;

    S_TIME_BASE = tBase;
    S_RANGE_A = rangeA;
    S_RANGE_B = rangeB;
    
    if (source != Chan::Count)
    {
        S_TRIG_SOURCE = source;
    }

    Osci::Init();

    FreqMeter::Init();
}


static bool FindSignal(Chan::E ch, TBase::E *tBase, Range::E *)
{
    float frequency = 0.0F;

    if (FindFrequency(ch, &frequency))
    {
        *tBase = CalculateTBase(frequency);

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

    for (int range = static_cast<int>(Range::_2mV); range < Range::Count; range++)
    {
        DisplayUpdate();

        uint timeSync = FindTwoSync(ch, static_cast<Range::E>(range), 150);

        if (timeSync == static_cast<uint>(-1))                                  // Не найдено синхронизации - переходим к следующему
        {
            continue;
        }
        else if (timeSync == 0)
        {
            timeSync = FindTwoSync(ch, static_cast<Range::E>(range), 1500);
        }
        else
        {
            FreqMeter::ClearMeasure();

            Osci::Start(false);

            do 
            {
                FPGA::Flag::Read();
            } while (!FreqMeter::FrequencyIsFound());

            *outFreq = FreqMeter::GetFrequency();
        }
    }

    return false;
}


//static bool FindFrequency(Chan::E ch, Range::E range, float *outFreq)
//{
//    /*
//        Находим сигнал с помощью частотомера
//        1. Устанавливаем один периода в параметрах частотомера
//        2. Читаем счётчики, тем самым обнуляя их.
//        3. Даём запуск.
//        4. Ждём, пока появится флаг частоты или периода
//    */
//
//    Settings old = set;
//
//    Osci::Stop();
//    Range::Set(ch, range);
//
//    Timer::PauseOnTime(500);
//
//    TuneFreqMeter();
//    
//    FreqMeter::ClearMeasure();
//
//    Osci::Start(false);
//
//    uint timeStart = TIME_MS;
//
//    do
//    {
//        FPGA::ReadFlag();
//        DisplayUpdate();
//    } while (!FreqMeter::FrequencyIsFound() && (TIME_MS - timeStart < 2000));
//
//    *outFreq = FreqMeter::GetFrequency();
//
//    set = old;
//
//    return FreqMeter::FrequencyIsFound();
//}


//Range::E FindRange(Chan::E)
//{
//    return Range::Count;
//}


//static void TuneFreqMeter()
//{
//    S_FREQ_METER_ENABLED = true;
//    S_FREQ_TIME_COUNTING = FreqMeter::TimeCounting::_1s;
//    S_FREQ_FREQ_CLC = FreqMeter::FreqClc::_100MHz;
//    S_FREQ_NUMBER_PERIODS = FreqMeter::NumberPeriods::_1;
//
//    FreqMeter::FPGA::LoadSettings();
//    FreqMeter::FPGA::ResetCounterFreq();
//    FreqMeter::FPGA::ResetCounterPeriod();
//
//    FPGA::Flag::Clear();
//}


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


static uint FindTwoSync(Chan::E /*ch*/, Range::E /*range*/, uint /*timeWait*/)
{
    return 0;
}
