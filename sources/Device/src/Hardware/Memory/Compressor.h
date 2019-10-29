#pragma once
#include "Hardware/Memory/Memory.h"


struct Compressor
{
    /// Возвращает количество байт, необходимое для хранения данных
    static uint GetPackedSize(const DataSettings *ds);
    /// Упаковывает данные с настойками ds по адресу address
    static void PackToROM(const DataSettings *ds, Address address);
    /// Заполняеет структуру ds из запакованных данных по адресу address
    static bool UnPack(Address address, DataSettings **ds);
    /// Возвращает адрес следующего пакета. Если 
    static Address Next(Address address);
};
