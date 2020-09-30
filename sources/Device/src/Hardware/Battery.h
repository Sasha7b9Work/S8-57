#pragma once


class Battery
{
public:

    // Напряжение батареи, при котором нужно производить отключение батареи
    static const float SHUTDOWN_VOLTAGE;

    static void Init();

    static void Draw(int x, int y);

    // Возвращает значение напряжения батареи
    static float GetVoltage();

    static float GetVoltageAverage();

    // Нужно для HAL_TIM5 - при true нельзя обращаться к напряжению
    static bool IsBusy();
};
