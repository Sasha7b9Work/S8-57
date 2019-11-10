#pragma once


struct MemoryRAM
{
    static int16 currentSignal;

    /// Установить текущим предыдущий сигнал
    static void SelectPrevSignal();
};
