#pragma once


struct SettingsTester
{
    Tester::Control::E  control;
    Tester::Polarity::E   polarity;
    Tester::StepU::E      stepU;
    Tester::StepI::E      stepI;
    ENumAverage::E      ave;                   ///< Количество усреднений
    Tester::ViewMode::E   viewMode;
};



struct PageTester
{
    static void Init();

    static void OnChanged_Control(bool);

    static const Page *const self;
};
