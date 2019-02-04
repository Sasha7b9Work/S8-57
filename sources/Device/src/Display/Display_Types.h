#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MOSI_HEIGHT             9
#define MOI_HEIGHT_TITLE        19
#define MOI_WIDTH_D_IP          34      /* Увеличение ширины открытого ip-адреса в случае наличия порта */
#define WIDTH_SB                19
#define GRID_DELTA              20      /* Количество точек в клетке */

#define BACKGROUND                  (set.disp_background)
#define BACKGROUND_BLACK            (BACKGROUND == Display::Background::Black)

#define SHOW_STRING_NAVI            (set.disp_showStringNavigation)
#define SHOW_STRING_NAVI_TEMP       (SHOW_STRING_NAVI == Display::ShowStrNavi::Temp)
#define SHOW_STRING_NAVI_ALL        (SHOW_STRING_NAVI == Display::ShowStrNavi::All)

#define MODE_DRAW_SIGNAL            (set.disp_modeDrawSignal)
#define MODE_DRAW_SIGNAL_IS_LINES   (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Lines)
#define MODE_DRAW_SIGNAL_IS_POINTS  (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Points)

//#define THICKNESS_SIGNAL            (set.disp_thickness)
//#define THICKNESS_SIGNAL_IS_3       (THICKNESS_SIGNAL == Display::ThicknessSignal::_3)

#define ENUM_MIN_MAX                (set.disp_ENumMinMax)
#define MIN_MAX_ENABLED             (ENUM_MIN_MAX != Display::ENumMinMax::_1)
#define NUM_MIN_MAX                 (1 << (int)ENUM_MIN_MAX)        /* Возвращает количество измерений сигналов для расчёта минимумов и максимумов. */

#define ENUM_SMOOTHING              (set.disp_ENumSmoothing)
#define NUM_SMOOTHING               (Display::NumPointSmoothing())
#define SMOOTHING_ENABLED           (ENUM_SMOOTHING != Display::ENumSmoothing::Disable)

#define MODE_ACCUM                  (set.disp_modeAccumulation)
#define MODE_ACCUM_RESET            (MODE_ACCUM == Display::ModeAccumulation::Reset)
#define MODE_ACCUM_NO_RESET         (MODE_ACCUM == Display::ModeAccumulation::NoReset)

#define ENUM_ACCUM                  (set.disp_ENumAccum)
#define ENUM_ACCUM_INF              (ENUM_ACCUM == ENumAccum_Infinity)
#define NUM_ACCUM                   (1 << (int)ENUM_ACCUM)                   /* Возвращает число накоплений */
#define IN_ACCUM_MODE               (ENUM_ACCUM > ENumAccum_1)

#define MODE_AVE                    (set.disp_modeAveraging)
#define ENUM_AVE                    (set.disp_ENumAverage)
#define NUM_AVE                     (1 << (int)ENUM_AVE)
#define IN_AVERAGING_MODE           (ENUM_AVE > Display::ENumAverage::_1 && (!IN_P2P_MODE))

#define SHIFT_IN_MEMORY             (set.disp_shiftInMemory)
#define TIME_MESSAGES               (set.disp_timeMessages)
#define ENUM_SIGNALS_IN_SEC         (set.disp_ENumSignalsInSec)
#define LAST_AFFECTED_CH            (set.disp_lastAffectedChannel)
#define LAST_AFFECTED_CH_IS_A       (LAST_AFFECTED_CH == Chan::A)
#define DISPLAY_ORIENTATION         (set.dbg_Orientation)

#define ALT_MARKERS                 (set.disp_altMarkers)
#define ALT_MARKERS_SHOW            (ALT_MARKERS == AltMarkers::Show)
#define ALT_MARKERS_HIDE            (ALT_MARKERS == AltMarkers::Hide)
#define ALT_MARKERS_AUTO            (ALT_MARKERS == AltMarkers::AutoHide)

#define LINKING_RSHIFT              (set.disp_linkingRShift)

#define MENU_AUTO_HIDE              (set.disp_menuAutoHide)

#define BRIGHTNESS_DISPLAY          (set.disp_brightness)

#define BRIGHTNESS_GRID             (set.disp_brightnessGrid)

#define SET_NUM_BYTES_ON_DISPLAY    (SET_PEAKDET_EN ? 281 * 2 : 281)

#define SHIFT_IN_MEMORY_IN_POINTS   Display::ShiftInMemoryInPoints()


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    struct DrawMode
    {
        enum E
        {
            Auto,
            Hand
        } value;
    };

    /// Выбор цвета фона.
    struct Background
    {
        enum E
        {
            Black,
            White
        } value;
    };

    struct ModeDrawSignal
    {
        enum E
        {
            Lines,
            Points
        } value;
    };

    /// Толщина рисуемого сигнала
    struct ThicknessSignal
    {
        enum E
        {
            _1,         ///< Сигнал рисуется линией толщиной одна точка
            _3          ///< Сигнал рисуется линией толщиной три точки
        } value;
    };

    /// Режим показа строки навигации
    struct ShowStrNavi
    {
        enum E
        {
            Temp,   ///< Показывать на несколько секунд
            All,    ///< Всегда показывать
            None    ///< Никогда не показывать
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
    };

    /// Тип усреднений по измерениям
    struct ModeAveraging
    {
        enum E
        {
            Accurately,   ///< Усреднять точно.
            Around        ///< Усреднять приблизительно.
        } value;
    };

    struct Orientation
    {
        enum E
        {
            Direct,
            Back
        } value;
    };
    /// Режим отображения дополнительных боковых маркеров смещения по напряжению.
    struct AltMarkers
    {
        enum E
        {
            Hide,        /// Никода не выводить.
            Show,        /// Всегда выводить.
            AutoHide     /// Выводить и прятать через timeShowLevels.
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
        /// Возвращает количество кадров в секунду
        int ToFPS() const;
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
    };

    struct ModeAccumulation
    {
        enum E
        {
            NoReset,   /// В этом режиме показываются строго N последних.
            Reset      /// В этом режиме набираются N последних и потом сбрасываются.
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
            _512
        } value;
    };
}
