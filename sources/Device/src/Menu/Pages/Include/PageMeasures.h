#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Measurements/Measures.h"


// Выводить автоматические измерения по каналу A
#define VIEW_MEASURES_A                 (setA.enabled && ((set.meas.source == MeasuresSource::A) || (set.meas.source == MeasuresSource::A_B)))
// Выводить автоматические измерения по каналу B
#define VIEW_MEASURES_B                 (setB.enabled && ((set.meas.source == MeasuresSource::B) || (set.meas.source == MeasuresSource::A_B)))


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
    enum E
    {
        None,      // Курсоры не следят.
        Voltage,   // Курсоры следят за напряжением автоматически.
        Time,      // Курсоры следят за временем автоматически.
        Both       // Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
    };

    static CursorsLookMode::E &Ref(Chan::E);
    static bool IsVoltage(Chan::E ch) { return Ref(ch) == Voltage; }
};

// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
struct CursorsActive
{
    enum E
    {
        U,
        T,
        None
    };

    static CursorsActive::E &Ref();
    static bool IsT()    { return Ref() == T; }
    static bool IsU()    { return Ref() == U; }
    static void Set(E v) { Ref() = v; }
};


struct CursorsSource
{
    CursorsSource() {}
    static Chan::E &Ref();
    operator Chan::E() { return Ref(); }
    static bool IsA() { return Ref() == ChanA; }
};


// Каким курсором управлять
struct CursorsControl
{
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
    enum E
    {
        Pixels,    // По пикселям экрана
        Percents   // По процентам
    };

    static CursorsMovement::E &Ref();
    static bool IsPercents() { return Ref() == Percents; }
    static bool IsPixels() { return Ref() == Pixels; }
};


struct SettingsCursorsMeasures
{
    bool               showCursors;             // Показывать ли курсоры.
    CursorsLookMode::E lookMode[Chan::Count];   // Режимы слежения за курсорами для двух пар курсоров.
    bool               showFreq;                // Установленное в true значение, что нужно показывать на экране 1/dT между курсорами.
    CursorsActive::E   active;                  // Какие курсоры сейас активны.
    Chan::E            source;                  // Источник - к какому каналу относятся курсоры.
    CursorsControl::E  cntrlU[Chan::Count];     // Активные курсоры напряжения.
    CursorsControl::E  cntrlT[Chan::Count];     // Активные курсоры времени.
    CursorsMovement::E movement;                // Как перемещаться курсорам - по точкам или процентам.
    float              deltaU100percents[2];    // Расстояние между курсорами напряжения для 100%, для обоих каналов.
    float              deltaT100percents[2];    // Расстояние между курсорами времени для 100%, для обоих каналов.
    float              posCurU[Chan::Count][2]; // Текущие позиции курсоров напряжения обоих каналов.
    float              posCurT[Chan::Count][2]; // Текущие позиции курсоров времени обоих каналов.
};



struct FuncModeDraw
{
    enum E
    {
        Disabled,
        Separate,
        Together
    };
    FuncModeDraw() {}
    static FuncModeDraw::E &Ref();
    operator FuncModeDraw::E() { return Ref(); }
    static bool IsDisabled()   { return Ref() == Disabled; }
    static bool IsSeparate()   { return Ref() == Separate; }
};

struct MathFunction
{
    enum E
    {
        Sum,
        Mul
    };
    MathFunction() {}
    static MathFunction::E &Ref();
    operator MathFunction::E() { return Ref(); }
    static bool IsSum() { return Ref() == Sum; }
    static bool IsMul() { return Ref() == Mul; }
};

struct ModeRegSet
{
    enum E
    {
        Range,
        RShift
    };
    ModeRegSet() {}
    static ModeRegSet::E &Ref();
    operator ModeRegSet::E() { return Ref(); }
    static bool IsRange()    { return Ref() == Range; }
    static bool IsRShift()   { return Ref() == RShift; }
};

struct SettingsMath
{ //-V802
    FuncModeDraw::E modeDraw;       // Раздельный или общий дисплей в режиме математической функции.
    MathFunction::E function;
    ModeRegSet::E   modeRegSet;     // Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    int8            divider;
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
