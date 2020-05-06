#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"


// Выводить автоматические измерения по каналу A
#define VIEW_MEASURES_A                 (S_CHANNEL_ENABLED_A && (S_MEAS_SOURCE_IS_A || S_MEAS_SOURCE_IS_A_B))
// Выводить автоматические измерения по каналу B
#define VIEW_MEASURES_B                 (S_CHANNEL_ENABLED_B && (S_MEAS_SOURCE_IS_B || S_MEAS_SOURCE_IS_A_B))


// Сколько автоматических измерений помещается на экран
struct MeasuresOnDisplay
{
    enum E
    {
        _1,      // 1 измерение слева внизу.
        _2,      // 2 измерения слева внизу.
        _1_5,    // 1 строка с 5 измерениями.
        _2_5,    // 2 строки по 5 измерений.
        _3_5,    // 3 строки по 5 измерений.
        _6_1,    // 6 строк по 1 измерению.
        _6_2     // 6 строк по 2 измерения.
    };

    // Возвращает true, если измерения расположены вертикально с левой стороны экрана
    static bool IsVertical();
};


struct MeasuresSource
{
#define S_MEAS_SOURCE           (set.meas.source)
#define S_MEAS_SOURCE_IS_A      (S_MEAS_SOURCE == MeasuresSource::A)
#define S_MEAS_SOURCE_IS_B      (S_MEAS_SOURCE == MeasuresSource::B)
#define S_MEAS_SOURCE_IS_A_B    (S_MEAS_SOURCE == MeasuresSource::A_B)

    enum E
    {
        A,
        B,
        A_B
    };
};


// Сжимать ли сигналы при выводе измерений.
struct MeasuresModeViewSignals
{
    enum E
    {
        AsIs,       // Показывать сигналы как есть.
        Compress    // Сжимать сетку с сигналами.
    };

    static bool IsCompress();
};


struct SettingsAutoMeasures
{
    bool                        show;              // Показывать ли измерения.
    MeasuresOnDisplay::E        number;            // Сколько измерений выводить.
    MeasuresSource::E           source;            // Для каких каналов выводить измерения.
    MeasuresModeViewSignals::E  modeViewSignals;   // Сжимать ли сигналы при выводе измерений.
    TypeMeasure::E              measures[15];      // Выбранные для индикации измерения.
    TypeMeasure::E              marked;            // Измерение, на которое нужно выводить маркеры.
};



// Режим слежения курсоров.
struct CursorsLookMode
{
#define S_CURS_LOOK_MODE(chan)              (set.curs._lookMode[chan])
#define S_CURS_LOOK_MODE_IS_VOLTAGE(chan)   (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Voltage)
#define S_CURS_LOOK_MODE_IS_TIME(chan)      (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Time)
#define S_CURS_LOOK_MODE_IS_BOTH(chan)      (S_CURS_LOOK_MODE(chan) == CursorsLookMode::Both)

    enum E
    {
        None,      // Курсоры не следят.
        Voltage,   // Курсоры следят за напряжением автоматически.
        Time,      // Курсоры следят за временем автоматически.
        Both       // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
    };
};

// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
struct CursorsActive
{
#define S_CURS_ACTIVE       (set.curs._active)
#define S_CURS_ACTIVE_IS_T  (S_CURS_ACTIVE == CursorsActive::T)
#define S_CURS_ACTIVE_IS_U  (S_CURS_ACTIVE == CursorsActive::U)

    enum E
    {
        U,
        T,
        None
    };
};


struct CursorsSource
{
    CursorsSource() {}
    operator Chan::E();
};


// Каким курсором управлять
struct CursorsControl
{
#define S_CURS_CONTROL_U(chan)  (set.curs._cntrlU[chan])
#define S_CURS_CONTROL_T(chan)  (set.curs._cntrlT[chan])

    enum E
    {
        _1,        // первым
        _2,        // вторым
        Both,      // обоими
        Disabled    // никаким
    };

    static CursorsControl::E &RefU(Chan::E ch);
    static CursorsControl::E &RefT(Chan::E ch);

    static bool IsDisabledT() { return RefT(CursorsSource()) == Disabled; }
    static bool IsEnabled1T() { return RefT(CursorsSource()) == _1 || RefT(CursorsSource()) == Both; }
    static bool IsEnabled2T() { return RefT(CursorsSource()) == _2 || RefT(CursorsSource()) == Both; }

    static bool IsDisabledU() { return RefU(CursorsSource()) == Disabled; }
    static bool IsEnabled1U() { return RefU(CursorsSource()) == _1 || RefU(CursorsSource()) == Both; }
    static bool IsEnabled2U() { return RefU(CursorsSource()) == _2 || RefU(CursorsSource()) == Both; }
};


// Дискретность перемещения курсоров.
struct CursorsMovement
{
#define S_CURS_MOVEMENT             (set.curs._movement)
#define S_CURS_MOVEMENT_IS_PIXELS   (S_CURS_MOVEMENT == CursorsMovement::Pixels)
#define S_CURS_MOVEMENT_IS_PERCENTS (S_CURS_MOVEMENT == CursorsMovement::Percents)

    enum E
    {
        Pixels,    // По пикселям экрана
        Percents   // По процентам
    };
};


#define S_CURS_SHOW                 (set.curs._showCursors)
#define S_CURS_SHOW_FREQ            (set.curs._showFreq)

#define S_CURS_SOURCE               (set.curs._source)
#define S_CURS_SOURCE_IS_A          (S_CURS_SOURCE == ChanA)

#define S_CURS_DU_PERCENTS(chan)    (set.curs._deltaU100percents[chan])
#define S_CURS_DT_PERCENTS(chan)    (set.curs._deltaT100percents[chan])

#define S_CURS_POS_U(chan, num)     (set.curs._posCurU[chan][num])
#define S_CURS_POS_U0(chan)         (S_CURS_POS_U(chan, 0))
#define S_CURS_POS_U1(chan)         (S_CURS_POS_U(chan, 1))

#define S_CURS_POS_T(chan, num)     (set.curs._posCurT[chan][num])
#define S_CURS_POS_T0(chan)         (S_CURS_POS_T(chan, 0))
#define S_CURS_POS_T1(chan)         (S_CURS_POS_T(chan, 1))


struct SettingsCursorsMeasures
{
    bool               _showCursors;             // Показывать ли курсоры.
    CursorsLookMode::E _lookMode[Chan::Count];   // Режимы слежения за курсорами для двух пар курсоров.
    bool               _showFreq;                // Установленное в true значение, что нужно показывать на экране 1/dT между курсорами.
    CursorsActive::E   _active;                  // Какие курсоры сейас активны.
    Chan::E            _source;                  // Источник - к какому каналу относятся курсоры.
    CursorsControl::E  _cntrlU[Chan::Count];     // Активные курсоры напряжения.
    CursorsControl::E  _cntrlT[Chan::Count];     // Активные курсоры времени.
    CursorsMovement::E _movement;                // Как перемещаться курсорам - по точкам или процентам.
    float              _deltaU100percents[2];    // Расстояние между курсорами напряжения для 100%, для обоих каналов.
    float              _deltaT100percents[2];    // Расстояние между курсорами времени для 100%, для обоих каналов.
    float              _posCurU[Chan::Count][2]; // Текущие позиции курсоров напряжения обоих каналов.
    float              _posCurT[Chan::Count][2]; // Текущие позиции курсоров времени обоих каналов.
};


#define S_MATH_MODE_DRAW                (set.math.modeDraw)
#define S_MATH_MODE_DRAW_IS_SEPARATE    (S_MATH_MODE_DRAW == FuncModeDraw::Separate)
#define S_MATH_MODE_DRAW_IS_DISABLED    (S_MATH_MODE_DRAW == FuncModeDraw::Disabled)
struct FuncModeDraw
{
    enum E
    {
        Disabled,
        Separate,
        Together
    };
};


#define S_MATH_FUNCTION         (set.math.function)
#define S_MATH_FUNCTION_IS_SUM  (S_MATH_FUNCTION == MathFunction::Sum)
#define S_MATH_FUNCTION_IS_MUL  (S_MATH_FUNCTION == MathFunction::Mul)
struct MathFunction
{
    enum E
    {
        Sum,
        Mul
    };
};


#define S_MATH_MODE_REG_SET             (set.math.modeRegSet)
#define S_MATH_MODE_REG_SET_IS_RSHIFT   (S_MATH_MODE_REG_SET == ModeRegSet::RShift)
#define S_MATH_MODE_REG_SET_IS_RANGE    (S_MATH_MODE_REG_SET == ModeRegSet::Range)

struct ModeRegSet
{
    enum E
    {
        Range,
        RShift
    };
};

struct SettingsMath
{ //-V802
    FuncModeDraw::E modeDraw;       // Раздельный или общий дисплей в режиме математической функции.
    MathFunction::E function;
    ModeRegSet::E   modeRegSet;     // Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    Divider::E      divider;
    int16           rShift;
    Range::E        range;
};



struct PageMeasures
{
    static const Page * const self;
};



struct PageAutoMeasures
{
    static const Page * const self;


    class Tune
    {
    public:
        static const Page * const self;
    };
};



struct PageCursorsMeasures
{
    static const Page * const self;


    struct Set
    {
        static const Page * const self;

        static void Draw_Channel(int x, int y);

        static void OnPress_Channel();

        // Выбрать следующий курсор
        static void IncCursCntrlU(Chan::E ch);

        // Выбрать следующий курсор
        static void IncCursCntrlT(Chan::E ch);

        static void OnPress_U();

        static void OnPress_T();

        static void SetCursPos100(Chan::E ch);

        // Изменить значение позиции курсора напряжения на delta точек
        static void SetShiftCursPosU(Chan::E ch, int numCur, float delta);

        // Изменить значение позиции курсора времени на delta точек
        static void SetShiftCursPosT(Chan::E ch, int numCur, float delta);

        static bool HandlerKey(const KeyEvent &event);

        static void UpdateCursorsForLook();

        // Установить позицию курсора напряжения
        static void SetCursorU(Chan::E ch, int numCur, float pos);

        // Установить позицию курсора времени
        static void SetCursorT(Chan::E ch, int numCur, float pos);

        // Возвращает true, если вращение ручки УСТАНОВКА будет перемещать курсоры/курсоры
        static bool IsRegSetActiveOnCursors();

    private:
        static const float MAX_POS_U;
        static const float MAX_POS_T;

        // Установить источник курсорных измерений
        static void SetCursSource(Chan::E ch);
    };
};



struct PageMath
{
    static const Page * const self;
};
