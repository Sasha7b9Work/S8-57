#pragma once


class ExtRAM
{
public:
    /// Первый байт внешней памяти
    static uint8 *Begin();
    /// За последним байтом внешней памяти
    static uint8 *End();
};
