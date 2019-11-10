#pragma once


struct MemoryRAM
{
    static int16 currentSignal;

    /// Установить текущим следующий сигнал
    static void SelectNextSignal();
    /// Установить текущим предыдущий сигнал
    static void SelectPrevSignal();
    /// Текущий номер сигнала в режиме ПОСЛЕДНИЕ
    static int16 CurrentSignal();
};
