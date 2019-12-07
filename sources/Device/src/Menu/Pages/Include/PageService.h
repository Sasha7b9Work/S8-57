#pragma once
#include "FPGA/Calibrator.h"
#include "Menu/MenuItems.h"


struct SettingsService
{
    Calibrator::Mode::E calibratorMode;   ///< Режим работы калибратора.
    uint8               soundVolume;      ///< Громкость звука
    bool                showInfoVoltage;  ///< Показывать напряжение батареи на экране
};



struct PageService
{
    static const Page * const self;

    static void OnPress_ResetSettings();

    struct PageBattery
    {
        static const Page * const self; 
    };


    struct PageCalibrate
    {
        static const Page * const self;
    };


    struct PageInformation
    {
        static const Page * const self;
    };
};



struct PageRTC
{
    static const Page * const self;
};
