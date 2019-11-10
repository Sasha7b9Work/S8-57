#pragma once


struct DataSettings;


struct MemoryRAM
{
    /// Номер текущего с конца сигнала. 0 - последний считанный сигнал, 1 - предпоследний считанный сигнал.
    static int16 currentSignal;
    /// Добавить вновь считанные данные
    static void Save(const DataSettings *ds);
    /// Считать информацию о num с конца сохранённых данных. 0 - последние данные, 1 - предпоследние
    static bool Read(DataSettings **ds, uint num = 0);
    /// Возвращает количество записей в памяти
    static uint NumberDatas();
};
