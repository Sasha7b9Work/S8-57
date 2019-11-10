#pragma once


struct DataSettings;


struct PacketRAM
{
    /*
        Данные хранятся таким образом
    */
    uint16 size;        /// Размер пакета вместе с упакованными данными. Если size == 0x0000, то этот пакет свободен
    /// Возвращает указатель на самый старый пакет. Он будет перезаписан первым
    static PacketRAM *Oldest();
    /// Возвращает указатель на самый новый пакет. Но будет перезаписан последним
    static PacketRAM *Newest();
    /// Упаковать данные по адресу this
    void Pack(const DataSettings *ds);
    /// Упаковать данные после данного пакета. Возвращает указатель на упкованный пакет, котоырй становится самым новым
    PacketRAM *PackNewest(const DataSettings *ds);
};


struct MemoryRAM
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
