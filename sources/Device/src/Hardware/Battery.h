// 2023/11/22 13:39:21 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Battery
{
    // Напряжение батареи, при котором нужно производить отключение батареи
    extern const float SHUTDOWN_VOLTAGE;

    void Init();

    void Draw(int x, int y);

    // Возвращает значение напряжения батареи
    float GetVoltage();

    float GetVoltageAverage();

    // Нужно для HAL_TIM5 - при true нельзя обращаться к напряжению
    bool IsBusy();
};
