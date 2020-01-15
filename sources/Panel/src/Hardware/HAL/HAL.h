#pragma once


#define ERROR_HANDLER()  ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL
{
    static void ErrorHandler(const char *, int);
};


/// »спользуетс€ дл€ управлени€ €ркостью диспле€
struct HAL_DAC2
{
    static void Init();
    /// «начение value от 0 до 100
    static void SetValue(uint value);
};


struct HAL_FSMC
{
    static void Init();

    static void SendToPanel(uint8 *data, uint size);

    static bool ReceiveByte();
};
