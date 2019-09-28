#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MOSI_HEIGHT         9
#define MOI_HEIGHT_TITLE    19
#define MOI_WIDTH_D_IP      34      /* Увеличение ширины открытого ip-адреса в случае наличия порта */
#define GRID_DELTA          20      /* Количество точек в клетке */

#define BACKGROUND                  (set.disp.background)
#define BACKGROUND_BLACK            (BACKGROUND == Display::Background::Black)

#define MODE_DRAW_SIGNAL            (set.disp.modeDrawSignal)
#define MODE_DRAW_SIGNAL_IS_LINES   (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Lines)
#define MODE_DRAW_SIGNAL_IS_POINTS  (MODE_DRAW_SIGNAL == Display::ModeDrawSignal::Points)

//#define THICKNESS_SIGNAL            (set.disp_thickness)
//#define THICKNESS_SIGNAL_IS_3       (THICKNESS_SIGNAL == Display::ThicknessSignal::_3)

#define ENUM_MIN_MAX                (set.disp.ENumMinMax)
#define MIN_MAX_ENABLED             (ENUM_MIN_MAX != Display::ENumMinMax::_1)
#define NUM_MIN_MAX                 (1 << (int)ENUM_MIN_MAX)        /* Возвращает количество измерений сигналов для расчёта минимумов и максимумов. */

#define ENUM_SMOOTHING              (set.disp.ENumSmoothing)
#define SMOOTHING_ENABLED           (ENUM_SMOOTHING != Display::ENumSmoothing::Disable)

#define ACCUMULATION                (set.disp.modeAccumulation)
#define ACCUMULATION_IS_ENABLED     (ACCUMULATION == ::Display::ModeAccumulation::Reset)


#define ENUM_ACCUM                  (set.disp.ENumAccum)
#define NUM_ACCUM                   (1 << (int)ENUM_ACCUM)                   /* Возвращает число накоплений */

#define MODE_AVE                    (set.disp.modeAveraging)
#define ENUM_AVE                    (set.disp.ENumAverage)
#define NUM_AVE                     (1 << (int)ENUM_AVE)
#define NUM_AVE_MAX                 256
#define IN_AVERAGING_MODE           (ENUM_AVE > Display::ENumAverage::_1 && (!IN_P2P_MODE))

#define TIME_MESSAGES               5

#define ENUM_SIGNALS_IN_SEC         (set.disp.ENumSignalsInSec)

#define LAST_AFFECTED_CH            (set.disp.lastAffectedChannel)
#define LAST_AFFECTED_CH_IS_A       (LAST_AFFECTED_CH == Chan::A)
#define DISPLAY_ORIENTATION         (set.dbg_Orientation)
#define DISPLAY_ORIENTATION_IS_NORMAL (DISPLAY_ORIENTATION == Display::Orientation::Normal)

#define LINKING_RSHIFT              (set.disp.linkingRShift)

#define MENU_AUTO_HIDE              (set.disp.menuAutoHide)

#define BRIGHTNESS_DISPLAY          (set.disp.brightness)

#define BRIGHTNESS_GRID             (set.disp.brightnessGrid)

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

    /// Толщина рисуемого сигнала
    struct ThicknessSignal
    {
        enum E
        {
            _1,         ///< Сигнал рисуется линией толщиной одна точка
            _3          ///< Сигнал рисуется линией толщиной три точки
        } value;
    };

    struct Orientation
    {
        enum E
        {
            Normal,
            Reverse
        } value;
        explicit Orientation(E v) : value(v) {};
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
}
