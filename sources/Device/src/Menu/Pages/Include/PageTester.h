#pragma once


struct SettingsTester
{
    Tester::Control::E  control;
    Tester::Polarity::E polarity;       // Полярность управляющего воздействия
    Tester::StepU::E    stepU;          // Выбранный шаг изменения напряжения
    Tester::StepI::E    stepI;          // Выбранный шаг изменения тока
    ENumAverage::E      enumAverage;    // Количество усреднений
    Tester::ViewMode::E viewMode;       // Режим отображения кривых
};



struct PageTester
{
    static void Init();

    static const Page *const self;
};
