#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"
#include "Menu/Menu.h"
#include "Osci/Osci_Settings.h"


#define TYPE_GRID                   (set.disp.typeGrid)
#define TYPE_GRID_1                 (TYPE_GRID == Display::TypeGrid::_1)
#define TYPE_GRID_2                 (TYPE_GRID == Display::TypeGrid::_2)
#define TYPE_GRID_3                 (TYPE_GRID == Display::TypeGrid::_3)
#define TYPE_GRID_4                 (TYPE_GRID == Display::TypeGrid::_4)

#define LINKING_RSHIFT              (set.disp.linkingRShift)

#define MENU_AUTO_HIDE              (set.disp.menuAutoHide)

#define BRIGHTNESS_DISPLAY          (set.disp.brightness)

#define BRIGHTNESS_GRID             (set.disp.brightnessGrid)

#define ENUM_MIN_MAX                (set.disp.ENumMinMax)
#define MIN_MAX_ENABLED             (ENUM_MIN_MAX != Display::ENumMinMax::_1)
#define NUM_MIN_MAX                 (1 << (int)ENUM_MIN_MAX)        /* Возвращает количество измерений сигналов для расчёта минимумов и максимумов. */

#define ENUM_SMOOTHING              (set.disp.ENumSmoothing)
#define SMOOTHING_ENABLED           (ENUM_SMOOTHING != Display::ENumSmoothing::Disable)

#define ACCUMULATION                (set.disp.modeAccumulation)
#define ACCUMULATION_IS_ENABLED     (ACCUMULATION == ::Display::ModeAccumulation::Reset)

#define NUM_ACCUM                   (1 << (int)ENUM_ACCUM)                   /* Возвращает число накоплений */

#define NUM_AVE_MAX                 256

#define ENUM_SIGNALS_IN_SEC         (set.disp.ENumSignalsInSec)

#define LAST_AFFECTED_CH            (set.disp.lastAffectedChannel)
#define LAST_AFFECTED_CH_IS_A       (LAST_AFFECTED_CH == Chan::A)


struct ModeDrawSignal
{
    enum E
    {
        Lines,
        Points
    } value;
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

namespace Display
{
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
}

struct SettingsDisplay
{
    ModeDrawSignal::E      modeDrawSignal;
    Background::E          background;                     ///< Цвет фона.
    ENumMinMax::E          ENumMinMax;                     ///< Перечисление количества измерений для определения минимумов и масимумов.
    uint8                  notUsing0;
    ENumAverage::E         ENumAverage;           ///< Число усреднений сигнала.
    ENumAccum::E           ENumAccum;             ///< Число накоплений сигнала на экране.
    Display::ModeAccumulation::E    modeAccumulation;      ///< Режим накопления сигналов.
    Display::ENumSmoothing          ENumSmoothing;         ///< Перечисление количества точек для скользящего фильтра.
    Display::ENumSignalsInSec       ENumSignalsInSec;      ///< Перечисление числа считываний сигнала в секунда.
    Display::TypeGrid::E            typeGrid;              ///< Тип сетки
    int                             brightnessGrid;        ///< Яркость сетки от 0 до 100.
    Display::LinkingRShift::E       linkingRShift;         ///< Тип привязки к смещению по вертикали.
    int16                           brightness;            ///< Яркость дисплея.
    Menu::AutoHide::E               menuAutoHide;          ///< Через сколько времени после последнего нажатия клавиши прятать меню.
    int16                           shiftInMemory;         ///< \brief Показывает смещение левого края стеки относительно нулевого байта памяти в 
                                                           ///< байтах. Т.е. для пикового детектора будет в два раза больше количества точек на экране.
    Chan::E                         lastAffectedChannel;   ///< \brief Последний управляемый канал. Используется для правильного вывода сигналов
                                                           ///< на экран с наложением один поверх другого
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
struct PageDisplay
{
    static void OnChanged_RefreshFPS(bool);

    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    class PageView
    {
    public:
        //static const Page * const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageAccumulation
    {
        static const Page * const self;

        static void OnPress_Accumulation_Clear();
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSettings
    {
        static const Page * const self;

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
