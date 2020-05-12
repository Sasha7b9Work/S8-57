#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/ParametersOsci.h"


#define NUM_AVE_MAX     256


struct DisplayMapping
{
#define S_DISP_MAPPING          (set.disp._mapping)
#define S_DISP_MAPPING_IS_LINES (S_DISP_MAPPING == DisplayMapping::Lines)
#define S_DISP_MAPPING_IS_DOTS  (S_DISP_MAPPING == DisplayMapping::Dots)

    enum E
    {
        Lines,
        Dots,
        Count
    };
};


// Количество усреднений по измерениям.
struct ENumAverage
{
#define S_OSCI_ENUM_AVERAGE         (set.disp._enumAverage)
#define S_OSCI_AVERAGING_IS_ENABLED (S_OSCI_ENUM_AVERAGE != ENumAverage::_1)
#define S_OSCI_ENUM_AVERAGE_U16     (static_cast<uint16>(S_OSCI_ENUM_AVERAGE))
#define S_OSCI_NUM_AVERAGE          (1 << S_OSCI_ENUM_AVERAGE)

#define S_TEST_ENUM_AVERAGE         (set.test._enumAverage)

    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _256,
        Count
    };
    static void Set(ENumAverage::E v);
};


// Количество накоплений.
struct ENumAccum
{
#define S_DISP_ENUM_ACCUM               (set.disp._enumAccum)
#define S_DISP_NUM_ACCUM                (1 << S_DISP_ENUM_ACCUM)
#define S_DISP_ENUM_ACCUM_IS_INFINITY   (set.disp._enumAccum == ENumAccum::_Infinity)

    enum E
    {
        _1,
        _2,
        _4,
        _8,
        _16,
        _32,
        _64,
        _128,
        _Infinity,
        Count
    };

    static pString ToString(ENumAccum::E v);
};


// Количество точек для расчёта сглаживания.
struct ENumSmoothing
{
#define S_DISP_ENUM_SMOOTH  (set.disp._enumSmoothing)
#define S_DISP_NUM_SMOOTH   (S_DISP_ENUM_SMOOTH + 1)

    enum E
    {
        Disable,
        _2points,
        _3points,
        _4points,
        _5points,
        _6points,
        _7points,
        _8points,
        _9points,
        _10points
    };
};


// Ограничение FPS.
struct ENumSignalsInSec
{
#define S_DISP_ENUM_FPS (set.disp._enumSignalsInSec)

    enum E
    {
        _25,
        _10,
        _5,
        _2,
        _1
    };
    // Возвращает количество миллисекунда между кадрами
    static uint TimeBetweenFramesMS();
};


// Тип сетки на экране.
struct TypeGrid
{
#define S_DISP_TYPE_GRID        (set.disp._typeGrid)
#define S_DISP_TYPE_GRID_IS_1   (S_DISP_TYPE_GRID == TypeGrid::_1)
#define S_DISP_TYPE_GRID_IS_2   (S_DISP_TYPE_GRID == TypeGrid::_2)
#define S_DISP_TYPE_GRID_IS_3   (S_DISP_TYPE_GRID == TypeGrid::_3)

    enum E
    {
        _1,
        _2,
        _3,
        _4,
        Count
    };
};

// Через какое время после последнего нажатия кнопки скрывать меню.
struct MenuAutoHide
{
#define S_MENU_AUTOHIDE (set.disp._menuAutoHide)

    enum E
    {
        None,   // Никогда.
        _5,     // Через 5 секунд.
        _10,    // Через 10 секунд.
        _15,    // Через 15 секунд.
        _30,    // Через 30 секунд.
        _60     // Через 60 секунд.
    };
};


#define S_DISP_BRIGHTNESS                   (set.disp._brightness)
#define S_DISP_SHIFT_IN_MEMORY              (set.disp._shiftInMemory)
#define S_DISP_LAST_AFFECTED_CHANNEL        (set.disp._lastAffectedChannel)
#define S_DISP_LAST_AFFECTED_CHANNEL_IS_A   (S_DISP_LAST_AFFECTED_CHANNEL == Chan::A)

struct SettingsDisplay
{ //-V802
    DisplayMapping::E      _mapping;
    ENumAverage::E         _enumAverage;           // Число усреднений сигнала.
    ENumAccum::E           _enumAccum;             // Число накоплений сигнала на экране.
    ENumSmoothing::E       _enumSmoothing;         // Перечисление количества точек для скользящего фильтра.
    ENumSignalsInSec::E    _enumSignalsInSec;      // Перечисление числа считываний сигнала в секунда.
    TypeGrid::E            _typeGrid;              // Тип сетки
    uint8                  _brightness;            // Яркость дисплея.
    MenuAutoHide::E        _menuAutoHide;          // Через сколько времени после последнего нажатия клавиши прятать меню.
    int16                  _shiftInMemory;         // \brief Показывает смещение левого края стеки относительно нулевого байта памяти в 
                                                  // байтах. Т.е. для пикового детектора будет в два раза больше количества точек на экране.
    Chan::E                _lastAffectedChannel;   // \brief Последний управляемый канал. Используется для правильного вывода сигналов
                                                  // на экран с наложением один поверх другого
    void SetLastAffectedChannel(Chan::E ch)
    {
        _lastAffectedChannel = ch;
    }
};


struct PageDisplay
{
    static const Page * const self;


    struct Accumulation
    {
        static const Page * const self;

        static void OnPress_Accumulation_Clear();
    };


    struct Settings
    {
        static const Page * const self;


        struct Colors
        {
            static const Page * const self;
            // Для сетки
            static ColorType colorTypeGrid;
            // Для первого канала
            static ColorType colorTypeA;
            // Для второго канала
            static ColorType colorTypeB;
        };
    };
};
