#pragma once



/// Чем будем управлять в тестер-компоненте - напряжением или током
struct TesterControl
{
    enum E
    {
        Voltage,
        Current
    };

    operator TesterControl::E();

    static bool IsVoltage();
};

struct TesterPolarity
{
    enum E
    {
        Positive,
        Negative
    };

    operator TesterPolarity::E();
    static bool IsPositive();
};

struct TesterStepU
{
    enum E
    {
        _100mV,
        _500mV
    };

    TesterStepU() {};
    operator TesterStepU::E();
    static bool Is500mV();
};

struct TesterStepI
{
    enum E
    {
        _4mA,
        _20mA
    };

    TesterStepI() {};
    operator TesterStepI::E();
    static bool Is20mA();
};

struct TesterViewMode
{
    enum E
    {
        Lines,
        Points
    };

    operator TesterViewMode::E();
};

struct SettingsTester
{
    TesterControl::E    control;
    TesterPolarity::E   polarity;
    TesterStepU::E      stepU;
    TesterStepI::E      stepI;
    ENumAverage::E      ave;                   ///< Количество усреднений
    TesterViewMode::E   viewMode;
};



struct PageTester
{
    static void Init();

    static void OnChanged_Control(bool);

    static const Page *const self;
};
