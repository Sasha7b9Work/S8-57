#pragma once
#include "Menu/MenuItems.h"


struct CalibratorMode
{
    enum E
    {
        Freq,
        DC,
        GND
    } value;
    explicit CalibratorMode(E v) : value(v) {};
};

struct ColorScheme
{
    enum E
    {
        WhiteLetters,   ///< В этом случае заголовки элементов меню пишутся белым - не очень хорошо видно снизу
        BlackLetters    ///< В этом случае заголовки элементов меню пишутся чёрным - не очень красиво выглядит
    } value;
    explicit ColorScheme(E v) : value(v) {}
};


struct SettingsService
{
    ColorScheme::E      colorScheme;
    CalibratorMode::E   calibratorMode;            ///< Режим работы калибратора.
    uint8               soundVolume;               ///< Громкость звука
    bool                showInfoVoltage;           ///< Показывать напряжение батареи на экране
};



struct PageService
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageBattery
    {
        static const Page * const self; 
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageCalibrate
    {
        static const Page * const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageInformation
    {
        static const Page * const self;
    };
};



struct PageRTC
{
    static const Page * const self;
};
