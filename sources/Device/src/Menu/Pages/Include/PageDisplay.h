#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/ParametersOsci.h"


#define NUM_AVE_MAX     256


#define S_DISP_MAPPING          (set.disp.mapping)
#define S_DISP_MAPPING_IS_LINES (S_DISP_MAPPING == DisplayMapping::Lines)
#define S_DISP_MAPPING_IS_DOTS  (S_DISP_MAPPING == DisplayMapping::Dots)
struct DisplayMapping
{
    enum E
    {
        Lines,
        Dots,
        Count
    };
};

// Количество усреднений по измерениям.
#define S_DISP_ENUM_AVERAGE         (set.disp.enumAverage)
#define S_DISP_AVERAGING_IS_ENABLED (S_DISP_ENUM_AVERAGE != ENumAverage::_1)
#define S_DISP_ENUM_AVERAGE_U16     (static_cast<uint16>(S_DISP_ENUM_AVERAGE))
#define S_DISP_NUM_AVERAGE          (1 << S_DISP_ENUM_AVERAGE)
struct ENumAverage
{
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
#define S_DISP_ENUM_ACCUM       (set.disp.enumAccum)
#define S_DISP_NUM_ACCUM        (1 << S_DISP_ENUM_ACCUM)
struct ENumAccum
{
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
        _Infinity
    };
};


#define S_DISP_MODE_ACCUM           (set.disp.modeAccumulation)
#define S_DISP_MODE_ACCUM_IS_RESET  (S_DISP_MODE_ACCUM == ModeAccumulation::Reset)
struct ModeAccumulation
{
    enum E
    {
        NoReset,   // В этом режиме показываются строго N последних.
        Reset      // В этом режиме набираются N последних и потом сбрасываются.
    };
};

// Количество точек для расчёта сглаживания.
#define S_DISP_ENUM_SMOOTH  (set.disp.enumSmoothing)
#define S_DISP_NUM_SMOOTH   (S_DISP_ENUM_SMOOTH + 1)
struct ENumSmoothing
{
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
#define S_DISP_ENUM_FPS (set.disp.enumSignalsInSec)
struct ENumSignalsInSec
{
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
#define S_DISP_TYPE_GRID        (set.disp.typeGrid)
#define S_DISP_TYPE_GRID_IS_1   (S_DISP_TYPE_GRID == TypeGrid::_1)
#define S_DISP_TYPE_GRID_IS_2   (S_DISP_TYPE_GRID == TypeGrid::_2)
#define S_DISP_TYPE_GRID_IS_3   (S_DISP_TYPE_GRID == TypeGrid::_3)
struct TypeGrid
{
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


struct SettingsDisplay
{ //-V802
    DisplayMapping::E      mapping;
    ENumAverage::E         enumAverage;           // Число усреднений сигнала.
    ENumAccum::E           enumAccum;             // Число накоплений сигнала на экране.
    ModeAccumulation::E    modeAccumulation;      // Режим накопления сигналов.
    ENumSmoothing::E       enumSmoothing;         // Перечисление количества точек для скользящего фильтра.
    ENumSignalsInSec::E    enumSignalsInSec;      // Перечисление числа считываний сигнала в секунда.
    TypeGrid::E            typeGrid;              // Тип сетки
    uint8                  brightness;            // Яркость дисплея.
    MenuAutoHide::E        menuAutoHide;          // Через сколько времени после последнего нажатия клавиши прятать меню.
    int16                  shiftInMemory;         // \brief Показывает смещение левого края стеки относительно нулевого байта памяти в 
                                                  // байтах. Т.е. для пикового детектора будет в два раза больше количества точек на экране.
    Chan::E                lastAffectedChannel;   // \brief Последний управляемый канал. Используется для правильного вывода сигналов
                                                  // на экран с наложением один поверх другого
    void SetLastAffectedChannel(Chan::E ch)
    {
        lastAffectedChannel = ch;
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
