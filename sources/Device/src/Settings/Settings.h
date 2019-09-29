#pragma once
#include "SettingsNRST.h"
#include "Tester/Tester.h"
#include "Osci/FrequencyCounter.h"
#include "Osci/Measurements/Cursors.h"
#include "Menu/Menu.h"
#include "Display/Colors.h"
#include "Osci/Measurements/Measures.h"
#include "Multimeter/Multimeter.h"
#include "Osci/Measurements/Cursors_Settings.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_SIZE                    (set.size)
#define SET_CRC32                   (set.crc32)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TRIG_MODE                   (set.trig.mode)
#define TRIG_HOLDOFF                (set.trig.holdOff)
#define TRIG_HOLDOFF_ENABLED        (set.trig.holdOffEnabled)

#define SET_DIVIDER(ch)             (divider[ch])
#define SET_DIVIDER_IS_1(ch)        (SET_DIVIDER(ch) == 0)
#define SET_DIVIDER_IS_10(ch)       (SET_DIVIDER(ch) == 1)

#define SET_DIVIDER_ABS(ch)         ((divider[ch] == 0) ? 1 : 10)


#pragma pack(push, 1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Settings
{
public:
    static void Load(bool _default = false);
    static void Save();
    static void Reset();

    void SetData(uint8 data[1024]);
    /// Сохраняет значение перечислимого типа. Возвращает адрес первой свободной ячейки (туда надо записывать следующее значение)
    void SaveEnum16(uint8 name, uint8 value);

    uint                size;                               ///< Размер данной структуры в байтах
    uint                crc32;                              ///< \brief Контрольная сумма данной структуры с хранящимися в ней настройками

    SettingsDisplay     disp;
    SettingsChannel     ch;
    SettingsTrig        trig;
    SettingsTime        time;
    SettingsCursors     curs;
    SettingsMemory      mem;
    SettingsMath        math;
    SettingsMeasures    meas;
    SettingsService     serv;
    SettingsFreqMeter   freq;
    SettingsDebug       dbg;
    SettingsTester      test;
    SettingsMenu        menu;
    SettingsNRST        nrst;
    SettingsMultimeter  mult;
    SettingsRecorder    rec;

    /*  !!!!!!!!!!!!!!!!!!!!!!                                    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!!!!!!!!!!!!!!!!!!!!! Эти настройки задаются при наладке !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!!!!!!!!!!!!!!!!!!!!!                                    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */

    int16   dbg_enum_gate_max;                              ///< Ограничение ворот в рандомизаторе сверху
    int16   dbg_enum_gate_min;                              ///< Ограничение ворот в рандомизаторе снизу

    int8    addRShift[2][Osci::Settings::Range::Size];      ///< Добавочные смещения для 

    /// Настройки по умолчанию
    static const Settings defaultSettings;
};

#pragma pack(pop)

extern Settings set;

extern int8 divider[2]; // Делитель канала
