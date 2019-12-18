#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Settings/SettingsOsci.h"


#define NUM_AVE_MAX     256


struct DisplayMapping
{
    enum E
    {
        Lines,
        Dots,
        Count
    };

    DisplayMapping() {};
    static DisplayMapping::E &Ref();
    DisplayMapping(DisplayMapping::E v) { Ref() = v; }
    operator DisplayMapping::E()        { return Ref(); }
    static bool IsLines()               { return Ref() == Lines; }
    static bool IsDots()                { return Ref() == Dots; }
};

/// Количество измерений для расчёта минимального и максимального значений.
struct ENumMinMax
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
        Count
    };

    ENumMinMax::E &Ref();
    int Number() { return 1 << Ref(); }
};

/// Количество усреднений по измерениям.
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
    ENumAverage() {};
    operator ENumAverage::E() { return Ref(); };
    int Number()              { return (1 << Ref()); };
    static ENumAverage::E &Ref();
    static void Set(ENumAverage::E v);
};

/// Количество накоплений.
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
    static ENumAccum::E &Ref();
    operator ENumAccum::E() { return Ref(); }
    int Number()            { return (1 << Ref()); };
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   /// В этом режиме показываются строго N последних.
        Reset      /// В этом режиме набираются N последних и потом сбрасываются.
    };

    static ModeAccumulation::E &Ref();
    static bool IsReset() { return (Ref() == Reset); }
};

/// Количество точек для расчёта сглаживания.
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
    static ENumSmoothing::E &Ref();
    static uint ToNumber() { return static_cast<uint>(Ref() + 1); };
};

/// Ограничение FPS.
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
    ENumSignalsInSec() {}
    static ENumSignalsInSec::E &Ref();
    /// Возвращает количество миллисекунда между кадрами
    static uint TimeBetweenFramesMS();
    operator ENumSignalsInSec::E() { return Ref(); }
    static bool Is25()             { return Ref() == _25; }
};

/// Тип сетки на экране.
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

    static TypeGrid::E &Ref();
    static bool Is1() { return (Ref() == _1); };
    static bool Is2() { return (Ref() == _2); };
    static bool Is3() { return (Ref() == _3); };
};

/// Тип привязки к смещению по вертикали
struct LinkingRShift
{
    enum E
    {
        Voltage,      ///< Смещение привязано к напряжению
        Position      ///< Смещение привязано к позиции
    };

    static LinkingRShift::E &Ref();
};

/// Через какое время после последнего нажатия кнопки скрывать меню.
struct MenuAutoHide
{
    enum E
    {
        None = 0,   ///< Никогда.
        _5 = 5,   ///< Через 5 секунд.
        _10 = 10,  ///< Через 10 секунд.
        _15 = 15,  ///< Через 15 секунд.
        _30 = 30,  ///< Через 30 секунд.
        _60 = 60   ///< Через 60 секунд.
    };

    MenuAutoHide() {};
    static MenuAutoHide::E &Ref();
    operator MenuAutoHide::E() { return Ref(); };
};


struct SettingsDisplay
{ //-V802
    DisplayMapping::E      mapping;
    ENumMinMax::E          enumMinMax;            ///< Перечисление количества измерений для определения минимумов и масимумов.
    uint8                  _;
    ENumAverage::E         enumAverage;           ///< Число усреднений сигнала.
    ENumAccum::E           enumAccum;             ///< Число накоплений сигнала на экране.
    ModeAccumulation::E    modeAccumulation;      ///< Режим накопления сигналов.
    ENumSmoothing::E       enumSmoothing;         ///< Перечисление количества точек для скользящего фильтра.
    ENumSignalsInSec::E    enumSignalsInSec;      ///< Перечисление числа считываний сигнала в секунда.
    TypeGrid::E            typeGrid;              ///< Тип сетки
    LinkingRShift::E       linkingRShift;         ///< Тип привязки к смещению по вертикали.
    uint8                  brightness;            ///< Яркость дисплея.
    MenuAutoHide::E        menuAutoHide;          ///< Через сколько времени после последнего нажатия клавиши прятать меню.
    int16                  shiftInMemory;         ///< \brief Показывает смещение левого края стеки относительно нулевого байта памяти в 
                                                  ///< байтах. Т.е. для пикового детектора будет в два раза больше количества точек на экране.
    Chan::E                lastAffectedChannel;   ///< \brief Последний управляемый канал. Используется для правильного вывода сигналов
                                                  ///< на экран с наложением один поверх другого
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
            /// Для сетки
            static ColorType colorTypeGrid;
            /// Для первого канала
            static ColorType colorTypeA;
            /// Для второго канала
            static ColorType colorTypeB;
        };
    };
};
