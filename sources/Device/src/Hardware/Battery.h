// 2023/11/22 13:39:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Battery
{
    // ���������� �������, ��� ������� ����� ����������� ���������� �������
    extern const float SHUTDOWN_VOLTAGE;

    void Init();

    void Draw(int x, int y);

    // ���������� �������� ���������� �������
    float GetVoltage();

    float GetVoltageAverage();

    // ����� ��� HAL_TIM5 - ��� true ������ ���������� � ����������
    bool IsBusy();
};
