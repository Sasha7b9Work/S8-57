#pragma once
#include "Tester/Tester.h"


struct SettingsTester
{
    Tester::Control::E  _control;
    Tester::Polarity::E _polarity;       // ���������� ������������ �����������
    Tester::StepU::E    _stepU;          // ��������� ��� ��������� ����������
    Tester::StepI::E    _stepI;          // ��������� ��� ��������� ����
    ENumAverage::E      _enumAverage;    // ���������� ����������
    Tester::ViewMode::E _viewMode;       // ����� ����������� ������
};



namespace PageTester
{
    void Init();

    void OnChanged_Control(bool = true);

    extern const Page *const self;
};
