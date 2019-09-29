#pragma once
#include "Menu/MenuItems.h"


#define TRIG_POLARITY           (set.trig.polarity)
#define TRIG_POLARITY_FALLING   (TRIG_POLARITY == TrigPolarity::Falling)
#define TRIG_POLARITY_FRONT     (TRIG_POLARITY == TrigPolarity::Rising)

#define TRIG_INPUT              (set.trig.input)
#define TRIG_INPUT_AC           (TRIG_INPUT == TrigInput_AC)
#define TRIG_INPUT_LPF          (TRIG_INPUT == TrigInput_LPF)
#define TRIG_INPUT_FULL         (TRIG_INPUT == TrigInput_Full)

#define TRIG_TIME_DELAY         (set.trig.TimeDelay)


/// Источник синхронизации
struct TrigSource
{
    enum E
    {
        A,    /// Канал 1
        B     /// Канал 2
    } value;

    static void Load();
};

/// Режим поиска синхронизции
struct TrigModeFind
{
    enum E
    {
        Hand,      ///< Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки.
        Auto       ///< Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала.
    } value;
    explicit TrigModeFind(E v) : value(v) {};
};

struct TrigInput
{
    enum E
    {
        Full,
        HF,
        LF
    } value;

    /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
    static void Load();
};

struct TrigPolarity
{
    enum E
    {
        Rising,
        Falling
    } value;
    explicit TrigPolarity(E v) : value(v) {};

    static void Load();
};

/// Режим запуска.
struct TrigStartMode
{
    enum E
    {
        Auto,     ///< Автоматический.
        Wait,     ///< Ждущий.
        Single    ///< Однократный.
    } value;
    explicit TrigStartMode(E v) : value(v) {};
};


struct SettingsTrig
{
    TrigSource::E    source;
    TrigInput::E     input;
    TrigPolarity::E  polarity;
    uint16                             lev[Chan::Size];
    TrigStartMode::E startMode;           ///< Режим запуска.
    TrigModeFind::E  modeFind;            ///< Поиск синхронизации - вручную или автоматически.
                                            /// \todo разрядности trig_holdOff недостаточно
    int16                              holdOff;             ///< Удержание синхронизации
    bool                               holdOffEnabled;      ///< Включение удержания синхронизации
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageTrig
{
    static void OnChanged_Mode(bool active);

    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageHoldOff
    {
        static const Page * const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageFind
    {
        static const Page * const self;
    };
};
