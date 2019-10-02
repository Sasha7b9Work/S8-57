#pragma once

namespace Multimeter
{
    /// Инициализация
    void Init();

    void DeInit();

    void Update();
    /// Сообщает мультиметру, что произошло изменение режима и нужно дождаться результата измерения перед выводом
    void ChangeMode();

    void Calibrate(int calibr);

    void ChangeAVP();

    /// Используется для отрисовки
    class Display
    {
    public:
        static void Update();
        /// Через эту функцию поступает измерение от прибора
        static void SetMeasure(const uint8 buffer[13]);

        static void ChangedMode();
    };
};
