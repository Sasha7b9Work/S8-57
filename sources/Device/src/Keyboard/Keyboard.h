#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Key
{
    enum E
    {
        None,
        Function,
        Measure,
        Memory,
        Service,
        ChannelA,
        ChannelB,
        Time,
        Start,
        Trig,
        Display,
        RangeMoreA,
        RangeLessA,
        RShiftMoreA,
        RShiftLessA,
        RangeMoreB,
        RangeLessB,
        RShiftMoreB,
        RShiftLessB,
        TBaseMore,
        TBaseLess,
        TShiftMore,
        TShiftLess,
        TrigLevMore,
        TrigLevLess,
        Left,
        Right,
        Up,
        Down,
        Enter,
        F1,
        F2,
        F3,
        F4,
        F5,
        Number
    } value;

    explicit Key(E v = None) : value(v) {};
    bool IsFunctional() const { return value >= F1 && value <= F5; };
    bool IsArrow() const { return value >= Left && value <= Down; };
    /// Возвращает true, если кнопка управляет параметром сигнала - развёрткой там, смещением
    bool IsControlSignal() const;
    pString Name();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct TypePress
{
    enum E
    {
        Press,      ///< Нажатие кнопки
        Repeat,     ///< Автоматический повтор
        Release,    ///< Отпускание кнопки
        Long,       ///< "Длинное" нажатие кнопки
        None,
        Number
    } value;

    explicit TypePress(E v) : value(v)  { };
    pString ToString();
    bool IsLong() const { return value == Long; };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct KeyEvent
{
    explicit KeyEvent(Key::E k = Key::None, TypePress::E t = TypePress::None) : key(k), type(t) { };
    Key::E key;
    TypePress::E type;
    /// Возвращает true, если кнопка означает увеличение
    bool IsAboveZero() const { return key == Key::Up || key == Key::Right; }
    /// Возвращает 1, если кнопка означает увеличение, и -1 в обратном случае
    int Delta() const { return IsAboveZero() ? 1 : -1; }
    
    bool IsLeft() const { return key == Key::Left; };
    bool IsRight() const { return key == Key::Right; };
    bool IsUp() const { return key == Key::Up; };
    bool IsDown() const { return key == Key::Down; };
    
    bool IsPress() const { return type == TypePress::Press; };
    bool IsRepeat() const { return type == TypePress::Repeat; };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Keyboard
{
public:
    /// Отлючить панель управления
    /** Когда панель отключена, то происходит лишь обновление состояния переменной releasedButton */
    static void Disable();
    /// Ожидание нажатия клавиши
    static Key::E WaitPressingButton();

    static void Enable();
};
