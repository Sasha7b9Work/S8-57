#pragma once
#include "Menu/MenuItems.h"
#include "Osci/ParametersOsci.h"


// Источник синхронизации
struct TrigSource
{
    static void Set(Chan::E ch = Chan::Count);
};


// Режим поиска синхронизции
struct TrigModeFind
{
    enum E
    {
        Hand,      // Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки.
        Auto       // Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала.
    };
};


#define S_TRIG_INPUT    (set.trig.input)
struct TrigInput
{
    enum E
    {
        Full,
        HF,
        LF
    };
    // Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
    static void Load();
};


struct TrigPolarity
{
    enum E
    {
        Rising,
        Falling
    };
    static void Load();
};


// Режим запуска.
struct TrigStartMode
{
    enum E
    {
        Auto,     // Автоматический.
        Wait,     // Ждущий.
        Single,   // Однократный.
        Count
    };
    static void Set(TrigStartMode::E);
};


#define S_TRIG_SOURCE           (set.trig.source)
#define S_TRIG_LEVEL(channel)   (set.trig.level[channel])
#define S_TRIG_LEVEL_SOURCE     S_TRIG_LEVEL(S_TRIG_SOURCE)

struct SettingsTrig
{
    Chan::E          source;
    TrigInput::E     input;
    TrigPolarity::E  polarity;
    int16            level[Chan::Count];
    TrigStartMode::E startMode;         // Режим запуска.
    TrigModeFind::E  modeFind;          // Поиск синхронизации - вручную или автоматически.
                                        // \todo разрядности trig_holdOff недостаточно
    int16            holdOff;           // Удержание синхронизации
    bool             holdOffEnabled;    // Включение удержания синхронизации
};


struct PageTrig
{
    static const Page * const self;


    struct HoldOff
    {
        static const Page * const self;
    };


    struct Find
    {
        static const Page * const self;
    };
};
