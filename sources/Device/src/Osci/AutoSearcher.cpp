#include "defines.h"
#include "Osci/Osci.h"


// Попытка найти сигнал на канале ch. Если попытка удалась, то найденные параметры загружаются в прибор
static bool FindSignal(Chan::E ch);

// Находит частоту сигнала, поданного на вход ch. В случае неудачи возвращает значение 0.0F
static float FindFrequency(Chan::E ch);

// Находит масштаб по вертикаил
static Range::E FindRange(Chan::E ch);

// Рассчитывает TBase, необходимый для отображения задданой частоты
static TBase::E CalculateTBase(float frequency);


void Osci::RunAutoSearch()
{
    Settings old;
    Settings::Store(&old);

    if (!FindSignal(ChanA))
    {
        if (!FindSignal(ChanB))
        {
            DISPLAY_SHOW_WARNING("Сигнал не обнаружен");
            Settings::Restore(&old);
        }
    }

    Osci::Init();
}


static bool FindSignal(Chan::E ch)
{
    float frequency = FindFrequency(ch);

    if (frequency != 0.0F)
    {
        TBase::Set(CalculateTBase(frequency));

        Range::Set(ch, FindRange(ch));
    }

    return (frequency != 0.0F);
}


static float FindFrequency(Chan::E ch)
{
    return 0.0F;
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

    int i = 0;

    while (times[i].tBase != TBase::Count)
    {
        if (frequency < times[i].frequency)
        {
            return times[i].tBase;
        }
    }

    return TBase::_2ns;
}


Range::E FindRange(Chan::E ch)
{
    return Range::Count;
}
