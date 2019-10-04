#pragma once


struct MemoryRAM
{
    /// Установить текущим сигнал по умолчанию
    static void ResetSignal();
    /// Установить текущим следующий сигнал
    static void SelectNextSignal();
    /// Установить текущим предыдущий сигнал
    static void SelectPrevSignal();
    /// Текущий номер сигнала в режиме ПОСЛЕДНИЕ
    static int16 CurrentSignal();
};
