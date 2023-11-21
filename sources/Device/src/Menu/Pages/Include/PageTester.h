#pragma once
#include "Tester/Tester.h"


struct SettingsTester
{
    Tester::Control::E  _control;
    Tester::Polarity::E _polarity;       // Полярность управляющего воздействия
    Tester::StepU::E    _stepU;          // Выбранный шаг изменения напряжения
    Tester::StepI::E    _stepI;          // Выбранный шаг изменения тока
    ENumAverage::E      _enumAverage;    // Количество усреднений
    Tester::ViewMode::E _viewMode;       // Режим отображения кривых
};



namespace PageTester
{
    void Init();

    void OnChanged_Control(bool = true);

    extern const Page *const self;
};
