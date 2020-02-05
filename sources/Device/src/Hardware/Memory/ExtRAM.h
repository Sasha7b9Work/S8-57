#pragma once


class ExtRAM
{
public:
    /// Первый байт внешней памяти
    static uint8 *Begin();
    /// За последним байтом внешней памяти
    static uint8 *End();
    /// Записывает во внешнюю память по адресу address данные buffer, size кратен 4
    static void Write(uint8 *buffer, uint size, uint8 *address);
};
