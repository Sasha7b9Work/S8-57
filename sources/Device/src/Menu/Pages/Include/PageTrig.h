#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


/// Источник синхронизации
struct TrigSource
{
    TrigSource() {};
    static void Set(Chan::E ch = Chan::Count);
    static Chan::E &Ref();
    operator Chan::E() { return Ref(); };

};

/// Режим поиска синхронизции
struct TrigModeFind
{
    enum E
    {
        Hand,      ///< Уровень синхронизации устанавливается вручную или автоматически - однократным нажажтием кнопки.
        Auto       ///< Подстройки уровня синхронизации производится автоматически после каждого нового считанного сигнала.
    };

    static TrigModeFind::E &Ref();
    static bool IsAuto() { return Ref() == Auto; }
    static bool IsHand() { return Ref() == Hand; }
};

struct TrigInput
{
    enum E
    {
        Full,
        HF,
        LF
    };
    TrigInput() {};
    static TrigInput::E &Ref();
    operator TrigInput::E() { return Ref(); }
    /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
    static void Load();
};

struct TrigPolarity
{
    enum E
    {
        Rising,
        Falling
    };
    TrigPolarity() {}
    static TrigPolarity::E &Ref();
    void Load();
    operator TrigPolarity::E() { return Ref(); }
};

/// Режим запуска.
struct TrigStartMode
{
    enum E
    {
        Auto,     ///< Автоматический.
        Wait,     ///< Ждущий.
        Single    ///< Однократный.
    };
    TrigStartMode() {}
    static TrigStartMode::E &Ref();
    operator TrigStartMode::E() { return Ref(); }
    static bool IsSingle()      { return Ref() == Single; }
    static bool IsWait()        { return Ref() == Wait; }
    static bool IsAuto()        { return Ref() == Auto; }
};


struct SettingsTrig
{
    Chan::E          source;
    TrigInput::E     input;
    TrigPolarity::E  polarity;
    int16            level[Chan::Count];
    TrigStartMode::E startMode;         ///< Режим запуска.
    TrigModeFind::E  modeFind;          ///< Поиск синхронизации - вручную или автоматически.
                                        /// \todo разрядности trig_holdOff недостаточно
    int16            holdOff;           ///< Удержание синхронизации
    bool             holdOffEnabled;    ///< Включение удержания синхронизации
};



struct PageTrig
{
    static void OnChanged_Mode(bool active);

    static const Page * const self;


    struct PageHoldOff
    {
        static const Page * const self;
    };


    struct PageFind
    {
        static const Page * const self;
    };
};
