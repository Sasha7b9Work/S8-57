#pragma once
#include "Menu/MenuItems.h"
#include "FPGA/FPGA_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


#define CALIBRATOR_MODE                 (set.serv.calibratorMode)
#define BAT_SHOW_ON_DISPLAY             (set.serv.showInfoVoltage)

#define SOUND_VOLUME                    (set.serv.soundVolume)
#define SOUND_ENABLED                   (SOUND_VOLUME == 0)

#define COLOR_SCHEME                    (set.serv.colorScheme)


struct SettingsService
{
    Color::Scheme::E    colorScheme;
    CalibratorMode::E   calibratorMode;            ///< Режим работы калибратора.
    uint8               soundVolume;               ///< Громкость звука
    bool                showInfoVoltage;           ///< Показывать напряжение батареи на экране
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageRTC
{
    static const Page * const self;
};
