#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define MOSI_HEIGHT             9
#define MOI_HEIGHT_TITLE        19
#define MOI_WIDTH_D_IP          34      /* Увеличение ширины открытого ip-адреса в случае наличия порта */
#define WIDTH_SB                19
#define GRID_DELTA              20  /* Количество точек в клетке */


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
