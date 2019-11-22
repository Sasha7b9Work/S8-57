#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Settings/SettingsOsci.h"


#define NUM_AVE_MAX     256
#define NUM_AVE         (1 << static_cast<int>(set.disp.ENumAverage))
#define NUM_ACCUM       (1 << static_cast<int>(set.disp.ENumAccum))
#define NUM_MIN_MAX     (1 << static_cast<int>(set.disp.ENumMinMax))

struct DisplayMapping
{
    enum E
    {
        Lines,
        Points,
        Count
    } value;

    DisplayMapping();
    DisplayMapping(DisplayMapping::E v);
    operator int() { return value; }
};

/// Цвет фона
struct Background
{
    enum E
    {
        Black,
        White
    } value;
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
        _128
    } value;
    explicit ENumMinMax(E v) : value(v) {};
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
        _256
    } value;
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
    } value;
};

struct ModeAccumulation
{
    enum E
    {
        NoReset,   /// В этом режиме показываются строго N последних.
        Reset      /// В этом режиме набираются N последних и потом сбрасываются.
    } value;
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
    } value;
    explicit ENumSmoothing(E v) : value(v) { };
    uint ToNumber() const;
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
    } value;
    explicit ENumSignalsInSec(E v) : value(v) { };
    /// Возвращает количество миллисекунда между кадрами
    uint TimeBetweenFramesMS() const;
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
        Size
    } value;
};

/// Тип привязки к смещению по вертикали
struct LinkingRShift
{
    enum E
    {
        Voltage,      ///< Смещение привязано к напряжению
        Position      ///< Смещение привязано к позиции
    } value;
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
    } value;
};


struct SettingsDisplay
{ //-V802
    DisplayMapping::E      mapping;
    Background::E          background;            ///< Цвет фона.
    ENumMinMax::E          ENumMinMax;            ///< Перечисление количества измерений для определения минимумов и масимумов.
    uint8                  notUsing0;
    ENumAverage::E         ENumAverage;           ///< Число усреднений сигнала.
    ENumAccum::E           ENumAccum;             ///< Число накоплений сигнала на экране.
    ModeAccumulation::E    modeAccumulation;      ///< Режим накопления сигналов.
    ENumSmoothing          ENumSmoothing;         ///< Перечисление количества точек для скользящего фильтра.
    ENumSignalsInSec       ENumSignalsInSec;      ///< Перечисление числа считываний сигнала в секунда.
    TypeGrid::E            typeGrid;              ///< Тип сетки
    int                    brightnessGrid;        ///< Яркость сетки от 0 до 100.
    LinkingRShift::E       linkingRShift;         ///< Тип привязки к смещению по вертикали.
    int16                  brightness;            ///< Яркость дисплея.
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


    struct PageAccumulation
    {
        static const Page * const self;

        static void OnPress_Accumulation_Clear();
    };


    struct PageSettings
    {
        static const Page * const self;


        struct PageColors
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
