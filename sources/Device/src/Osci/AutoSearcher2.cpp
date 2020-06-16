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
static float FindFrequency(Chan::E ch);

// Находит время между двумя синхронизациями при заданном range. Возвращаемые значения:
// -1 - Не найдено ни одной синхронизации
// 0 - Найдена одна синхронизация
// 1 и более - время в миллисекундах между синхронизациями
static uint FindTwoSync(uint timeWaitMS);

// Рассчитывает TBase, необходимый для отображения задданой частоты
static TBase::E CalculateTBase(float frequency);


namespace FrequencyMeter
{
    // Найти частоту по счётчику частоты
    float FindFromCounerFrequency();

    
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
    float frequency = FindFrequency(ch);

    if (frequency != 0.0F)
    {
        *tBase = CalculateTBase(frequency);

        *range = Range::_500mV;

        return true;
    }

    return false;
}


static float FindFrequency(Chan::E ch)
{
    Osci::Stop();
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

        Range::Set(ch, static_cast<Range::E>(range));

        uint timeSync = FindTwoSync(150);

        if (timeSync > 0)
        {
            return FrequencyMeter::FindFromCounerFrequency();
        }
    }
}


static uint FindTwoSync(uint timeWaitMS)
{
    FPGA::Flag::Clear();

    Osci::Start(false);

    uint start = TIME_MS;           // Время начала ожидания

    while (!FPGA::Flag::TrigReady())
    {
        if (TIME_MS - start > timeWaitMS)
        {
            return static_cast<uint>(-1);
        }

        FPGA::ReadFlag();
    }

    uint timeFirst = TIME_MS;

    FPGA::Flag::Clear();

    Osci::Start(false);

    while (!FPGA::Flag::TrigReady())
    {
        if (TIME_MS - timeFirst > timeWaitMS)
        {
            return 0;
        }

        FPGA::ReadFlag();
    }

    uint result = TIME_MS - timeFirst;

    if (result == 0)
    {
        result++;
    }

    return result;
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


float FrequencyMeter::FindFromCounerFrequency()
{
    State::Store();



    State::Restore();
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
}
