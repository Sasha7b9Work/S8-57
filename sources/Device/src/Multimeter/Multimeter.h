#pragma once


struct DisplayMultimeter
{
    static void Update();
    /// Через эту функцию поступает измерение от прибора
    static void SetMeasure(const uint8 buffer[13]);

    static void ChangedMode();
};


struct Multimeter
{
    /// Инициализация
    static void Init();

    static void DeInit();

    static void Update();
    /// Сообщает мультиметру, что произошло изменение режима и нужно дождаться результата измерения перед выводом
    static void ChangeMode();

    static void Calibrate(int calibr);

    static void ChangeAVP();
};
