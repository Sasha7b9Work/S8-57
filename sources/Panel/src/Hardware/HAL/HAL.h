#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void ErrorHandler(const char *, int);
};


// »спользуетс€ дл€ управлени€ €ркостью диспле€
struct HAL_DAC2
{
    static void Init();
    // «начение value от 0 до 100
    static void SetValue(int value);
};


namespace HAL_BUS
{
    void Init();

    void SendToDevice(uint8 *data, uint size);

    void Update();

    bool DataIsReceived();
};
