#pragma once
#include "Hardware/Memory/Memory.h"


#define STATE_FREE    0xFFFFFFFFU
#define STATE_ERASED  0x00000000U
#define STATE_VALID   0xFF00FF00U

#define TYPE_DATA     0U


struct Packet
{
    uint  state;    // Состояние пакета:
                    // 0xFFFFFFFF - в пакет запись не производилась
                    // 0x00000000 - пакет стёрт
                    // 0xFF00FF00 - действующий пакет
    uint16 size;    // Размер пакета - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // Тип данных

    bool IsData() const { return (state == STATE_VALID) && (type == TYPE_DATA); }
    bool IsFree() const { return (state == STATE_FREE); }
    bool IsErased() const { return (state == STATE_ERASED); }
    bool IsValid() const { return (state == STATE_VALID); }
    Packet *Next() const;
    bool UnPack(DataSettings **ds) const;
    int Size() const;
    /// Делает попытку записи пакета в сектор sector. В случае неудачи возвращает false
    bool WriteToSector(const Sector *sector) const;
};


struct Compressor
{
    /// Возвращает количество байт, необходимое для хранения данных
    static uint GetPackedSize(const DataSettings *ds);
    /// Упаковывает данные с настойками ds по адресу address
    static void Pack(const DataSettings *ds, uint address);
    /// Скопировать пакет из src в dest
    static void Copy(Packet *dest, const Packet *src);
};
