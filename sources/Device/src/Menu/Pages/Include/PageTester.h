#pragma once


struct SettingsTester
{
    Tester::Control::E  control;
    Tester::Polarity::E polarity;
    Tester::StepU::E    stepU;          // Выбранный шаг изменения напряжения
    Tester::StepI::E    stepI;          // Выбранный шаг изменения тока
    ENumAverage::E      enumAverage;    // Количество усреднений
    Tester::ViewMode::E viewMode;       // Режим отображения кривых
};



struct PageTester
{
    static void Init();

    static void OnChanged_Control(bool);

    static const Page *const self;
};
