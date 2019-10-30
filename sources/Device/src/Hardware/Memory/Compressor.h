#pragma once
#include "Hardware/Memory/Memory.h"


struct Packet
{
    uint  state;    // Состояние пакета:
                    // 0xFFFFFFFF - в пакет запись не производилась
                    // 0x00000000 - пакет стёрт
                    // 0xFF00FF00 - действующий пакет
    uint16 size;    // Размер пакета - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // Тип данных :
};


#define STATE_FREE    0xFFFFFFFFU
#define STATE_ERASED  0x00000000U
#define STATE_VALID   0xFF00FF00U

#define TYPE_DATA     0U


struct Compressor
{
    /// Возвращает адрес следующего пакета в адресном пространстве микроконтроллера.
    static Packet *NextPacket(Packet *packet);
    /// Возвращает количество байт, необходимое для хранения данных
    static uint GetPackedSize(const DataSettings *ds);
    /// Упаковывает данные с настойками ds по адресу address
    static void Pack(const DataSettings *ds, uint address);
    /// Заполняеет структуру ds из запакованных данных по адресу address
    static bool UnPack(uint address, DataSettings **ds);
};
