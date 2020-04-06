#pragma once
#include "FPGA/Calibrator.h"
#include "Menu/MenuItems.h"


struct SettingsService
{
    Calibrator::Mode::E calibratorMode;     // Режим работы калибратора.
    uint8               enumVolume;         // Громкость звука
    bool                showInfoVoltage;    // Показывать напряжение батареи на экране
    uint8               SoundVolume();      // Возвращает значение от 0 (выкл) до 100 (макс)
};



struct PageService
{
    static const Page * const self;

    static void OnPress_ResetSettings();

    static void DecodePassword(const KeyEvent &event);

    struct Battery
    {
        static const Page * const self; 
    };


    struct Calibrate
    {
        static const Page * const self;
    };


    struct Information
    {
        static const Page * const self;
    };
};



struct PageRTC
{
    static const Page * const self;
};
