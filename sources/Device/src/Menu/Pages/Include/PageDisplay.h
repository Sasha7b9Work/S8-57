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
    static int Number();
    static void Set(ENumAverage::E v);
};

// Количество накоплений.
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
    static int Number();
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   // В этом режиме показываются строго N последних.
        Reset      // В этом режиме набираются N последних и потом сбрасываются.
    };
};

// Количество точек для расчёта сглаживания.
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
    static int Number();
};

// Ограничение FPS.
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
