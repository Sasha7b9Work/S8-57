#pragma once
#include "Menu/MenuItems.h"


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


struct SettingsTrig
{
    TrigSource::E    source;
    Osci::Settings::Trig::Input::E     input;
    Osci::Settings::Trig::Polarity::E  polarity;
    uint16                             lev[Chan::Size];
    Osci::Settings::Trig::StartMode::E startMode;           ///< Режим запуска.
    Osci::Settings::Trig::ModeFind::E  modeFind;            ///< Поиск синхронизации - вручную или автоматически.
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
