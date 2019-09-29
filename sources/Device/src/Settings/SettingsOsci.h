#pragma once


#define SET_RSHIFT(ch)  (RShift::Value(ch))
#define SET_RSHIFT_A    (SET_RSHIFT(Chan::A))
#define SET_RSHIFT_B    (SET_RSHIFT(Chan::B))

/// Ќа столько единиц нужно изменить значение смещени€, чтобы маркер смещени€ по напр€жению передвинулс€ на одну точку.
#define STEP_RSHIFT     (((RShift::MAX - RShift::MIN) / 24) / 20)
#define STEP_TRIGLEV    STEP_RSHIFT



struct DataSettings;


struct Chan
{
    enum E
    {
        A,
        B,
        Math,
        Size
    } value;
    explicit Chan(E v) : value(v) { };
    bool IsA() { return value == A; };
    bool IsB() { return value == B; }
    int PointsInChannel() const;
    /// ¬озвращает количество пам€ти, требуемой дл€ сохранени€ данных одного канала
    int RequestBytes(DataSettings *ds) const;
    pString Name() const;
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
        Size
    } value;
    explicit TBase(E v) : value(v) {};

    pString Name() const;

    pString ToString() const;

    static void Load();

    static void Change(int delta);

    /// — этого значени€ должен включатьс€ режим поточечного вывода
    static const E MIN_P2P = _50ms;
    /// ћинимальный масштаб по времени, при котором ещЄ возможно включение режима пикового детектора
    static const E MIN_PEAK_DET = _200ns;
};


struct Range
{
    static void Change(Chan::E ch, int delta);
    /// «агружатьс€ эта настройка может только дл€ обоих каналов одновременно
    static void LoadBoth();

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
        Size
    } value;
    explicit Range(E v) : value(v) {};
    pString Name() const;

    static void Set(Chan::E ch, E range);

    pString ToString(int8 divider);

};


struct RShift
{
    /// Ёто значение соответствует минимуму смещени€
    static const int MIN = 20;
    /// Ёто значение соотвествует максимуму смещени€
    static const int MAX = 980;
    /// Ёто значение соответствует середине экрана
    static const int ZERO = 500;
    /// »зменить на delta
    static void Change(Chan::E ch, int delta);
    /// ”становить значение
    static void Set(Chan::E ch, uint16 rShift);
    /// «агрузить в аппаратуру
    static void Load(Chan::E ch);
    /// ќтрисовать оба на экране
    static void DrawBoth();
    /// ѕреобразовать в строку
    static String ToString(uint16 rShiftRel, Range::E range, int8 divider);
    /// ¬озвращает ссылку на значение
    static uint16 &Value(Chan::E ch);
private:
    /// ќтрисовать маркер вертикального смещени€ на сетке
    static void Draw(Chan::E ch);
};
