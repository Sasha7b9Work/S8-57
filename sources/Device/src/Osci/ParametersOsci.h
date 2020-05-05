#pragma once
#include "Utils/String.h"


#define ChanA Chan(Chan::A)
#define ChanB Chan(Chan::B)


struct DataSettings;
struct SettingsChannel;


struct Chan
{
    enum E
    {
        A,
        B,
        Count
    } ch;
    Chan(E v) : ch(v) { };
    pString Name() const;
    operator Chan::E() { return ch; };
    SettingsChannel &Ref();

    bool IsA() const { return ch == A; };
    bool IsB() const { return ch == B; }
    bool IsEnabled();
    bool IsInversed();
};


struct TBase
{
    enum E
    {
        _2ns,
        _5ns,
        _10ns,
        _20ns,
        _50ns,
        _100ns,
        _200ns,
        _500ns,
        _1us,
        _2us,
        _5us,
        _10us,
        _20us,
        _50us,
        _100us,
        _200us,
        _500us,
        _1ms,
        _2ms,
        _5ms,
        _10ms,
        _20ms,
        _50ms,
        _100ms,
        _200ms,
        _500ms,
        _1s,
        _2s,
        _5,
        _10s,
        Count
    };

    pString Name() const;

    static pString ToString();

    static void Set(E v = Count);
    static void Set200us() { Set(_200us); }
    static void Set500us() { Set(_500us); }
    static void Set100ms() { Set(_100ms); }

    static void Change(int delta);

    // С этого значения должен включаться режим поточечного вывода
    static const E MIN_P2P = _10ms;
    
    // Минимальный масштаб по времени, при котором ещё возможно включение режима пикового детектора
    static const E MIN_PEAK_DET = _5us;

    // Расстояние между точками в режиме рандомизатора
    static int DeltaPoint();

    // Время между точками
    static float TimePoint(TBase::E base);

private:
    TBase() {};
};


struct Range
{
    enum E
    {
        _2mV,
        _5mV,
        _10mV,
        _20mV,
        _50mV,
        _100mV,
        _200mV,
        _500mV,
        _1V,
        _2V,
        _5V,
        _10V,
        _20V,
        Count
    };

    static void Set(Chan::E ch, E range);
    static void Set500mV(Chan::E ch) { Set(ch, _500mV); }
    static void Set1V(Chan::E ch)    { Set(ch, _1V); }
    static void Set2V(Chan::E ch)    { Set(ch, _2V); }

    static pString Name(Chan::E ch);

    static pString ToString(Chan::E ch, int8 divider);

    static pString ToString(Range::E range, int8 divider);
    // Возвращает напряжение, соответствующее верхней границе сетки
    static float MaxVoltageOnScreen(Range::E range);

    static void Change(Chan::E ch, int16 delta);
    // Загружаться эта настройка может только для обоих каналов одновременно
    static void LoadBoth();
};


struct RShift
{
public:
    // Этим конструктором можно узнать значение
    RShift(Chan::E _ch) : ch(_ch) {};
    // Этим конструктором можно установить значение
    void Set(int16 rShift);

    operator int16();

    // Изменить на delta
    void Change(int16 delta);
    // Загрузить в аппаратуру. Если force, то загружать нужно, не проверяя никакие условия
    void Load(bool force = false);
    // Отрисовать оба на экране
    static void DrawBoth();
    // Преобразовать в строку
    static String ToString(int16 rShiftRel, Range::E range, int8 divider);

    static float ToAbs(int16 rShift, Range::E range);

    static int16 ToRel(float rShiftAbs, Range::E range);

    static bool ChangeMath(int delta);

private:
    // Отрисовать маркер вертикального смещения на сетке
    void Draw();

    static const float absStep[Range::Count];
    // Это значение соответствует минимуму смещения
    static const int16 MIN = -480;
    // Это значение соотвествует максимуму смещения
    static const int16 MAX = 480;
    // Это знаение, которое нужно засылать в аппаратную часть, чтобы получить смещение "0"
    static const int16 HARDWARE_ZERO = 500;
    // На столько единиц нужно изменить значение смещения, чтобы маркер смещения по напряжению передвинулся на одну точку.
    static const int16 STEP = (((MAX - MIN) / 24) / 20);

    Chan::E ch;
};

// Режим канала по входу.
struct ModeCouple
{
    enum E
    {
        DC,      // Закрытый вход
        AC,      // Открытый вход
        GND,     // Вход заземлён.
        Count
    };
    // С помощью этого можно узнать значение
    ModeCouple(Chan::E _ch) : ch(_ch) {};

    void Set(ModeCouple::E couple) { Ref(ch) = couple; Range::LoadBoth(); }

    void SetGND() { Set(GND); }

    void SetAC() { Set(AC); }

    static ModeCouple::E &Ref(Chan::E);

    operator ModeCouple::E() { return Ref(ch);};
    
    pString UGO() const;

private:
    Chan::E ch;
};

struct Bandwidth
{
    enum E
    {
        Full,     // Если это значение выбрано в меню КАНАЛ, то при этом положение устанавливается полоса из ОТЛАДКА-КАНАЛЫ-Полоса.
        _20MHz,
    };

    Bandwidth(Chan::E c) : ch(c) {}
    static Bandwidth::E &Ref(Chan::E);
    void Load();
    bool Is20MHz() { return Ref(ch) == _20MHz; };

private:
    Chan::E ch;
};

struct TShift
{
    static void Set(int);

    static int Min();
    static int Max();

    static void Load();

    static void Change(const int delta);

    static String ToString(const TBase::E tBase = TBase::Count);

    static float ToAbs(const int shift, const TBase::E tBase);

    static void Reset();

    static void Draw();

private:
    static void LoadReal();

    static void LoadRandomize();
    // Нарисовать "нормальное" изображение маркера смещения, когда позиция смещения находится на экране
    static void DrawNormal(const int x, const int y);
    // Нарисовать маркер смещения, когда позиция смещения находится за левой границей экрана
    static void DrawLeft();
    // Нарисовать маркер смещения, когда позиция смещения находится за правой границей экрана
    static void DrawRight();

    static const float absStep[TBase::Count];

    TShift() { };
};

struct Trig
{
    // Возвращает true в случае наличия синхроимпульса
    static bool SyncPulse();
    // Вывести уровень синхронизации поверх сетки
    static void DrawOnGrid();
    // После вызова этой функции уровень синхронизации будет отрисовываться поверх сигнала в течение timMS миллисекунд
    static void NeedForDraw();

    static bool pulse;
};

struct TrigLevel
{
    static void Set(Chan::E ch, int16 level);

    // Загрузить уровень синхронизации в аппаратную часть
    static void Load();

    // Изменить уровень синхронизации текущего иточника синхронизации на delta единиц
    static void Change(int16 delta);

    // Однократно найти уровень синхронизации
    static void Find(Chan::E ch);

    static void Draw(Chan::E ch);

private:
    static const int16 MIN = -480;
    static const int16 MAX = 480;
    static const int16 HARDWARE_ZERO = 500;
    static const int16 STEP = (((MAX - MIN) / 24) / 20);
};


struct VALUE
{
    static const uint8 AVE = 127;
    static const uint8 MIN = AVE - 125;
    static const uint8 MAX = AVE + 125;
    // Если значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного вывода p2p, а также для tShift ранее считанного сигнала
    static const uint8 NONE = 0;

    static void PointsToVoltage(const uint8 *points, uint numPoints, Range::E range, int16 rShift, float *voltage);

    static void PointsFromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    static uint8 FromVoltage(float voltage, Range::E range, int16 rShift);

    static float ToVoltage(uint8 value, Range::E range, int16 rShift);

private:
    static const float voltsInPoint[Range::Count];
};

