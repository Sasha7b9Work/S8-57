#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void ErrorHandler(const char *, int);
};


/// Используется для управления яркостью дисплея
struct HAL_DAC2
{
    static void Init();
    /// Значение value от 0 до 100
    static void SetValue(uint value);
};


struct HAL_BUS
{
    static void Init();

    static void SendToDevice(uint8 *data, uint size);

    static void Update();

private:
    // Послать имеющиеся данные в ЦП
    static void TransmitData();
    // Принять данные от ЦП
    static void ReceiveData();
};
