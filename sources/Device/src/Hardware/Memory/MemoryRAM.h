#pragma once


struct DataSettings;


struct PacketRAM
{
    /*
        Данные хранятся таким образом
    */
    uint addrNewest;    /// Адрес следующего пакета, более "свежего"
                        /// addrNext == 0x00000000 - в пакете ничего не записано
                        /// addrNext == 0xffffffff - в пакете записаны данные, но это последний пакет
    /// Упаковать данные по адресу this. Возвращает указатель на пакет, следующий за ним
    PacketRAM *Pack(const DataSettings *ds);
    /// Упаковать данные после данного пакета. Возвращает указатель на упкованный пакет, котоырй становится самым новым
    PacketRAM *PackNewest(const DataSettings *ds);
    /// Возвращает указатель на следующий пакет
    PacketRAM *Next() const;
    /// Возвращает true, если пакет пустой (size == 0x0000)
    bool IsEmpty() const;

    uint Size() const;

    DataSettings *GetDataSettings() const;

    uint Address() const { return reinterpret_cast<uint>(this); };
};


struct RAM
{
    /// Номер текущего с конца сигнала. 0 - последний считанный сигнал, 1 - предпоследний считанный сигнал.
    static int16 currentSignal;
    /// Инициализирует первоначальное состояние памяти
    static void Init();
    /// Добавить вновь считанные данные
    static void Save(const DataSettings *ds);
    /// Считать информацию о num с конца сохранённых данных. 0 - последние данные, 1 - предпоследние
    static bool Read(DataSettings **ds, uint num = 0);
    /// Возвращает количество записей в памяти
    static uint NumberDatas();
};
