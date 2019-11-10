#pragma once


struct MemoryRAM
{
    /// Ќомер текущего с конца сигнала. 0 - последний считанный сигнал, 1 - предпоследний считанный сигнал.
    static int16 currentSignal;
};
