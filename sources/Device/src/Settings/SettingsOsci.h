#pragma once


struct DataSettings;

struct Chan
{
    enum E
    {
        A,
        B,
        Count
    } value;
    Chan(E v) : value(v) { };
    bool IsA() { return value == A; };
    bool IsB() { return value == B; }
    int PointsInChannel() const;
    /// Возвращает количество памяти, требуемой для сохранения данных одного канала
    int RequestBytes(DataSettings *ds) const;
    pString Name() const;
    operator int() { return static_cast<int>(value); };
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
    } value;
    /// Этим констуктором можно установить новое значение
    explicit TBase(E v);
    /// Через этот конструктор можно узнать установленное значение
    TBase();

    operator TBase::E() { return value; }

    pString Name() const;

    pString ToString() const;

    static void Load();

    static void Change(int delta);

    /// С этого значения должен включаться режим поточечного вывода
    static const E MIN_P2P = _50ms;
    /// Минимальный масштаб по времени, при котором ещё возможно включение режима пикового детектора
    static const E MIN_PEAK_DET = _200ns;

    uint RandK();
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
    } value;
    /// Этот конструктор - чтобы узнать текущее знаечение Range
    Range(Chan::E ch);
    /// Этот конструктор устанавливает новоез значениеRange
    Range(Chan::E ch, E range);

    operator Range::E() { return value; }
    
    pString Name() const;

    pString ToString(int8 divider);

    static pString ToString(Range::E range, int8 divider);
    /// Возвращает напряжение, соответствующее верхней границе сетки
    static float MaxVoltageOnScreen(Range::E range);

    static void Change(Chan::E ch, int16 delta);
    /// Загружаться эта настройка может только для обоих каналов одновременно
    static void LoadBoth();
};


struct RShift
{
public:
    /// Этим конструктором можно узнать значение
    RShift(Chan::E ch);
    /// Этим конструктором можно установить значение
    RShift(Chan::E ch, int16 rShift);

    operator int16() { return shift; };

    /// Изменить на delta
    static void Change(Chan::E ch, int16 delta);
    /// Загрузить в аппаратуру
    static void Load(Chan::E ch);
    /// Отрисовать оба на экране
    static void DrawBoth();
    /// Преобразовать в строку
    static String ToString(int16 rShiftRel, Range::E range, int8 divider);

    static float ToAbs(int16 rShift, Range::E range);

    static int16 ToRel(float rShiftAbs, Range::E range);

    static bool ChangeMath(int delta);

private:
    /// Отрисовать маркер вертикального смещения на сетке
    static void Draw(Chan::E ch);

    static const float absStep[Range::Count];
    /// Это значение соответствует минимуму смещения
    static const int16 MIN = -480;
    /// Это значение соотвествует максимуму смещения
    static const int16 MAX = 480;
    /// Это знаение, которое нужно засылать в аппаратную часть, чтобы получить смещение "0"
    static const int16 HARDWARE_ZERO = 500;
    /// На столько единиц нужно изменить значение смещения, чтобы маркер смещения по напряжению передвинулся на одну точку.
    static const int16 STEP = (((MAX - MIN) / 24) / 20);

    int16 shift;
};

/// Режим канала по входу.
struct ModeCouple
{
    enum E
    {
        DC,      ///< Закрытый вход
        AC,      ///< Открытый вход
        GND,     ///< Вход заземлён.
        Count
    } value;
    /// С помощью этого можно узнать значение
    ModeCouple(Chan::E ch);
    /// С помощью этого можно установить новое значение
    ModeCouple(Chan::E ch, ModeCouple::E couple);

    operator ModeCouple::E() { return value; }
    
    pString UGO() const;
};

struct Bandwidth
{
    enum E
    {
        Full,     ///< Если это значение выбрано в меню КАНАЛ, то при этом положение устанавливается полоса из ОТЛАДКА-КАНАЛЫ-Полоса.
        _20MHz,
    } value;

    explicit Bandwidth(E v) : value(v) {};
    void Load();

private:
    /// Возвращает канал, для которого является действующей эта настройка
    Chan::E GetChannel() const;
};

struct TShift
{
    TShift();
    explicit TShift(int tShift);

    operator int() { return shift; }

    static int Min();
    static int Zero();
    static int Max();

    static void Load();

    static void Change(int delta);

    static String ToString(TBase::E tBase);

    static float ToAbs(int shift, TBase::E tBase);

    static void Reset();

private:
    static void LoadReal();
    static void LoadRandomize();
    static const float absStep[TBase::Count];

    int shift;
};

struct Trig
{
    /// Возвращает true в случае наличия синхроимпульса
    static bool SyncPulse();
    /// Вывести уровень синхронизации поверх сетки
    static void DrawOnGrid();
    /// После вызова этой функции уровень синхронизации будет отрисовываться поверх сигнала в течение timMS миллисекунд
    static void NeedForDraw();

    static bool pulse;
};

struct TrigLevel
{
    /// Загрузить уровень синхронизации в аппаратную часть
    static void Load();
    /// Изменить уровень синхронизации на delta единиц
    static void Change(int16 delta);
    /// Установить заданный уровень синхронизации
    static void Set(int16 level);
    /// Однократно найти уровень синхронизации
    static void Find();

    static void Draw();

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
    /// Если значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного вывода p2p, а также для tShift ранее считанного сигнала
    static const uint8 NONE = 0;

    static void PointsToVoltage(const uint8 *points, uint numPoints, Range::E range, int16 rShift, float *voltage);

    static void PointsFromVoltage(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    static uint8 FromVoltage(float voltage, Range::E range, int16 rShift);

    static float ToVoltage(uint8 value, Range::E range, int16 rShift);

private:
    static const float voltsInPoint[Range::Count];
};

