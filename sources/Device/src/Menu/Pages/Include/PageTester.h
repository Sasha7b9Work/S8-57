#pragma once


struct SettingsTester
{
    Tester::Control::E  _control;
    Tester::Polarity::E _polarity;       // Полярность управляющего воздействия
    Tester::StepU::E    _stepU;          // Выбранный шаг изменения напряжения
    Tester::StepI::E    _stepI;          // Выбранный шаг изменения тока
    ENumAverage::E      enumAverage;    // Количество усреднений
    Tester::ViewMode::E viewMode;       // Режим отображения кривых
};



struct PageTester
{
    static void Init();

    static const Page *const self;
};
