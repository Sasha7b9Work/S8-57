#pragma once
#include "Hardware/Memory/Memory.h"


#define STATE_FREE    0xFFFFFFFFU
#define STATE_ERASED  0x00000000U
#define STATE_VALID   0xFF00FF00U

#define TYPE_DATA     0U

#define SECTOR(i) HAL_FLASH::sectors[i]
#define ADDR_SECTOR(i) (SECTOR(i).address)
#define SIZE_SECTOR(i) (SECTOR(i).size)
#define END_SECTOR(i) (ADDR_SECTOR(i) + SIZE_SECTOR(i))

struct Sector;

struct Packet
{
    uint  state;    // —осто€ние пакета:
                    // 0xFFFFFFFF - в пакет запись не производилась
                    // 0x00000000 - пакет стЄрт
                    // 0xFF00FF00 - действующий пакет
    uint16 size;    // –азмер пакета - sizeof(size) + sizeof(type) + sizeof(data)
    uint16 type;    // “ип данных

    bool IsData() const { return (state == STATE_VALID) && (type == TYPE_DATA); }
    bool IsFree() const { return (state == STATE_FREE); }
    bool IsErased() const { return (state == STATE_ERASED); }
    bool IsValid() const { return (state == STATE_VALID); }
    Packet *Next() const;
    bool UnPack(DataSettings **ds) const;
    int Size() const;
    uint Address() const { return reinterpret_cast<uint>(this); };
    /// ƒелает попытку записи пакета в сектор sector. ¬ случае неудачи возвращает false
    bool WriteToSector(const Sector *sector) const;
    /// ¬озвращает количество байт, необходимое дл€ хранени€ данных
    static uint GetPackedSize(const DataSettings *ds);
};


struct Compressor
{
    /// —копировать пакет из src в dest
    static void Copy(Packet *dest, const Packet *src);
};

struct Sector
{
    enum E
    {
        _00_BOOT_1,
        _01_BOOT_2,
        _02,
        _03,
        _04,
        _05_FIRM_1,
        _06_FIRM_2,
        _07_FIRM_3,
        _08,
        _09,
        /// ѕервый сектор дл€ сохранени€ настроек. ѕри его заполнении начинает использоватьс€ сектор 2.
        _10_SETTINGS_1,
        _11_SETTINGS_2,
        _12,
        _13,
        _14,
        _15,
        _16,
        _17_RECORDER_1,
        _18_RECORDER_2,
        _19_DATA_1,
        _20_DATA_2,
        _21_DATA_3,
        _22_DATA_4,
        _23_DATA_5,
        Count
    } number;

    uint address;

    uint size;
    /// јдрес первого байта за сектором
    uint End() const;

    void Erase() const;
    /// ѕеремещает пакеты из сектора src. ѕакеты в исходном секторе src затираютс€
    void MovePacketsFromSector(const Sector *src) const;
    /// ¬озвращает указатель на первый пакет с данными. nullptr, если пакетов с данными нет
    const Packet *GetFirstPacketWithData() const;
    /// ¬озвращает номер сектора, которому принадлежит address
    static int Number(uint address);
    /// true означает, что в сектор не записан ни один пакет
    bool NotExistPackets() const;

    Packet *GetFirstFreePacket() const;

    const Packet *WritePacket(const DataSettings *ds) const;
};
