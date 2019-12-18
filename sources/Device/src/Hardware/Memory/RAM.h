#pragma once


struct DataSettings;
struct FrameP2P;
struct Packet;


class RAM
{
public:
    /// Номер текущего с конца сигнала. 0 - последний считанный сигнал, 1 - предпоследний считанный сигнал.
    static int16 currentSignal;
    /// Инициализирует первоначальное состояние памяти
    static void Init();
    /// Подготовить в хранилище место для хранения новых данных
    static DataSettings *PrepareForNewData();
    /// Считать информацию о num с конца сохранённых данных. 0 - последние данные, 1 - предпоследние
    static DataSettings *Get(uint num = 0);
    /// Возвращает количество записей в памяти
    static uint NumberDatas();

private:
    /// Указатель на самый старый записанный пакет. Он будет стёрт первым
    static Packet *oldest;
    /// Указатель на последний записанный пакет. Он будет стёрт последним
    static Packet *newest;
    /// Освободить место для записи пакета с данными в соответствии с ds
    static uint AllocateMemoryForPacket(const DataSettings *ds);
    /// Удалить самую старую запись
    static void RemoveOldest();
    /// Освободить size байт памяти с начала буфера
    static void AllocateMemoryFromBegin(uint size);
    /// Если текущий фрейм является фреймом поточечного вывода, то возвращает этот фрейм
    static DataSettings *CurrentFrameIsP2P();
};
