#pragma once
#include "Tester/Tester.h"
#include "Osci/FrequencyCounter.h"
#include "Menu/Menu.h"
#include "Display/Colors.h"
#include "Osci/Measurements/Measures.h"
#include "Multimeter/Multimeter.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/DebugPage.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "Menu/Pages/Include/PageFreqMeter.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageMultimeter.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTester.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"



#pragma pack(push, 1)
class Settings
{
public:
    static void Load(bool _default = false);
    static void Save();
    static void Reset();

    void SetData(uint8 data[1024]);
    /// Сохраняет значение перечислимого типа. Возвращает адрес первой свободной ячейки (туда надо записывать следующее значение)
    void SaveEnum16(uint8 name, uint8 value);

    /// Настройки по умолчанию
    static const Settings defaultSettings;

    uint                size;                               ///< Размер данной структуры в байтах
    uint                crc32;                              ///< \brief Контрольная сумма данной структуры с хранящимися в ней настройками

    SettingsDisplay     disp;
    SettingsChannel     ch[Chan::Size];
    SettingsTrig        trig;
    SettingsTime        time;
    SettingsCursors     curs;
    SettingsMemory      mem;
    SettingsMath        math;
    SettingsFFT         fft;
    SettingsMeasures    meas;
    SettingsService     serv;
    SettingsFreqMeter   freq;
    SettingsTester      test;
    SettingsMenu        menu;
    SettingsMultimeter  mult;
    SettingsRecorder    rec;
    SettingsDebug       dbg;
};
#pragma pack(pop)

extern Settings set;
