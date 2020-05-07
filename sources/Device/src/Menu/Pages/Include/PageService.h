#pragma once
#include "FPGA/Calibrator.h"
#include "Menu/MenuItems.h"


#define S_SERV_ENUM_VOLUME          (set.serv._enumVolume)
#define S_SERV_SHOW_INFO_VOLTAGE    (set.serv._showInfoVoltage)
#define S_SERV_TIME_DISABLE_DISPLAY (set.serv._timeDisableDisplay)
#define S_SERV_TIME_DISABLE_POWER   (set.serv._timeDisablePower)


struct SettingsService
{
    Calibrator::Mode::E _calibratorMode;        // Режим работы калибратора.
    uint8               _enumVolume;            // Громкость звука
    bool                _showInfoVoltage;       // Показывать напряжение батареи на экране
    int16               _timeDisableDisplay;    // Время отключения дисплея в минутах
    int16               _timeDisablePower;      // Время отключения питания в минутах
    uint8               SoundVolume();          // Возвращает значение от 0 (выкл) до 100 (макс)
};



struct PageService
{
    static const Page * const self;

    static void OnPress_ResetSettings();

    static void DecodePassword(const KeyEvent &event);

    struct Battery
    {
        static const Page *const self;
    };


    struct Calibrate
    {
        static const Page *const self;
    };


    struct Information
    {
        static const Page *const self;
    };
};


struct PageRTC
{
    static const Page *const self;
};
