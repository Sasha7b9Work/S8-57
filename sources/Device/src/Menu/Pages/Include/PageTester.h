#pragma once



/// Чем будем управлять в тестер-компоненте - напряжением или током
struct TesterControl
{
    enum E
    {
        Voltage,
        Current
    };
};

struct TesterPolarity
{
    enum E
    {
        Positive,
        Negative
    };
};

struct TesterStepU
{
    enum E
    {
        _100mV,
        _500mV
    };
};

struct TesterStepI
{
    enum E
    {
        _4mA,
        _20mA
    };
};

struct TesterViewMode
{
    enum E
    {
        Lines,
        Points
    };
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
