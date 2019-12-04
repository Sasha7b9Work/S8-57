#pragma once


/// »спользуетс€ дл€ управлени€ €ркостью диспле€
struct HAL_DAC2
{
    static void Init();
    /// «начение value от 0 до 100
    static void SetValue(uint value);
};
