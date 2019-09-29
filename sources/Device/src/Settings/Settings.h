#pragma once
#include "SettingsDebug.h"
#include "SettingsNRST.h"
#include "Tester/Tester.h"
#include "Osci/FrequencyCounter.h"
#include "Osci/Measurements/Cursors.h"
#include "Menu/Menu.h"
#include "Display/Colors.h"
#include "Osci/Measurements/Measures.h"
#include "Multimeter/Multimeter.h"
#include "Recorder/Recorder_Settings.h"
#include "Osci/Measurements/Cursors_Settings.h"
#include "Tester/Tester_Settings.h"
#include "Multimeter/Multimeter_Settings.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMemory.h"
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

/// Возвращает позицию активного пункта на странице Page::Name.
#define MENU_POS_ACT_ITEM(name)     (set.menu_posActItem[name])

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


    // Меню ОТЛАДКА

    int8                         dbg_showConsole;               ///< Показывать ли консоль
    int16                        dbg_numStrings;                ///< Число строк в консоли.
    int8                         dbg_sizeFont;                  ///< Размер шрифта консоли - 0 - 5, 1 - 8.
    bool                         dbg_modePauseConsole;          ///< Если true, то вывод в консоль останавливается нажатием кнопки ПУСК/СТОП.
    bool                         dbg_showAll;
    bool                         dbg_showFlag;
    bool                         dbg_showRShift[2];
    bool                         dbg_showTrigLev;
    bool                         dbg_showRange[2];
    bool                         dbg_showChanParam[2];
    bool                         dbg_showTrigParam;
    bool                         dbg_showTShift;
    bool                         dbg_showTBase;
    int16                        dbg_numMeasuresForGates;       ///< Число измерений для ворот.
    bool                         dbg_showRandInfo;              ///< Выводить информацию по рандомизатору - ворота и считанное значение.
    bool                         dbg_showRandStat;              ///< Выводить график статистики.
    int16                        dbg_timeCompensation;          ///< Дополнительное смещение по времени для данной развёртки режима рандомизатора.
    int16                        dbg_pretriggered;              ///< Регулируемая величина предзапуска для исследования рандомизатора.
    FPGA::Settings::Bandwidth::E dbg_bandwidth[2];              ///< \brief Здесь задаётся полоса, которая будет включаться при выборе в "КАНАЛ-Полоса" 
                                                                ///< значения "Полная".
    bool                         dbg_ShowStats;                 ///< Показывать статистику на экране (fps, например).
    Display::Orientation::E      dbg_Orientation;

    // Настройки тестер-компонента

    Tester::Settings::Control::E    test_control;
    Tester::Settings::Polarity::E   test_polarity;
    Tester::Settings::StepU::E      test_stepU;
    Tester::Settings::StepI::E      test_stepI;
    Display::ENumAverage::E         test_ave;                   ///< Количество усреднений
    Tester::Settings::ViewMode::E   test_viewMode;

    // Настройки меню

    bool                menu_show;                                                  ///< Если true, то нужно показывать текущую страницу
    int8                menu_posActItem[PageName::Number];                          ///< \brief Позиция активного пункта. bit7 == 1 - item is opened, 0x7f - нет активного пункта.
    int8                menu_currentSubPage[PageName::Number];                      ///< Номер текущей подстраницы.

    // Несбрасываемые настройки калибровки

    int16               nrst_balanceADC[Chan::Size];                                ///< Значение дополнительного смещения АЦП для ручной балансировки.
    BalanceADC::E       nrst_balanceADCtype;                                        ///< Тип балансировки.
    StretchADC::E       nrst_stretchADCtype;                                        ///< Тип растяжки канала.
    int16               nrst_StretchADC[Chan::Size][3];                             ///< \brief Поправочный коэффициент для ручного, калибровочного и
                        ///< отключенного режимов. Здесь хранится в целом виде, чтобы получить реальный коэффициент, нужно разделить на 1000 и
                        ///< прибавить единицу.
    int16               nrst_rShiftAddStable[Chan::Size][3];                        ///< Добавочное смещение для трёх самых чувствительных диапазонов. Задаётся единожды при настройке
    int16               nrst_numAveForRand;                                         ///< По скольким измерениям усреднять сигнал в режиме рандомизатора.
    int16               nrst_numSmoothForRand;                                      ///< Число точек для скользящего фильта в рандомизаторе.
    int16               nrst_rShiftAdd[Chan::Size][Osci::Settings::Range::Size][2]; ///< Добавочное смещение, которое пишется сюда при калибровке и балансировке
    int16               nrst_correctionTime;                                        ///< Коэффициент коррекции времени.

    // Настройки мультиметра
    Multimeter::Settings::AVP::E              multi_avp;
    Multimeter::Measure::E                    multi_meas;
    Multimeter::Settings::RangeDC::E          multi_rangeDC;
    Multimeter::Settings::RangeAC::E          multi_rangeAC;
    Multimeter::Settings::RangeCurrent::E     multi_rangeCurrentAC;     ///< Предел измерения переменного тока
    Multimeter::Settings::RangeCurrent::E     multi_rangeCurrentDC;     ///< Предел измерения постоянного тока
    Multimeter::Settings::RangeResistance::E  multi_rangeResist;

    // Настройки регистратора
    bool                               rec_sourceChanA;      ///< Включение записи канала 1
    bool                               rec_sourceChanB;      ///< Включение записи канала 2
    bool                               rec_sourceSensor;     ///< Включение записи показаний датчика
    Recorder::Settings::TypeStorage::E rec_storageRecord;    ///< Куда сохранять данные
    Recorder::Settings::TypeStorage::E rec_storagePlay;      ///< Откуда брать сохранённые данные
    Recorder::Settings::Axis::E        rec_axisMove;         ///< Текущая ось перемещения
    Recorder::Settings::Axis::E        rec_axisZoom;         ///< Текущая ось масштабирования
    Recorder::Settings::ScaleX         rec_scaleX;           ///< Установленный масштаб по времени
    Osci::Settings::Range::E           unused[2];            ///< Масштаб по вертикали в режиме регистратора


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
