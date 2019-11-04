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
    uint Address() const { return reinterpret_cast<uint>(this); };
    /// Делает попытку записи пакета в сектор sector. В случае неудачи возвращает false
    bool WriteToSector(const Sector *sector) const;
    /// Возвращает количество байт, необходимое для хранения данных
    static uint GetPackedSize(const DataSettings *ds);
};


struct Compressor
{
    /// Скопировать пакет из src в dest
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
        /// Первый сектор для сохранения настроек. При его заполнении начинает использоваться сектор 2.
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
    /// Адрес первого байта за сектором
    uint End() const;

    void Erase() const;
    /// Перемещает пакеты из сектора src. Пакеты в исходном секторе src затираются
    void MovePacketsFromSector(const Sector *src) const;
    /// Возвращает указатель на первый пакет с данными. nullptr, если пакетов с данными нет
    const Packet *GetFirstPacketWithData() const;
    /// true означает, что в сектор не записан ни один пакет
    bool ExistPackets() const;

    const Packet *FirstFreePacket() const;

    const Packet *WriteData(int numInROM, const DataSettings *ds) const;

    const Packet *ReadData(int numInROM, DataSettings **ds) const;
    /// Получить информацию о сохранённх в секторе данных
    void GetDataInfo(bool existData[FlashMemory::Data::MAX_NUM_SAVED_WAVES]) const;
    /// Возвращает указатель на первый пакет
    const Packet *FirstPacket() const;
    /// Возвращает номер сектора, которому принадлежит address
    static int Number(uint address);

};
