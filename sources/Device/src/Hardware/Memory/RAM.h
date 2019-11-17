#pragma once


struct FrameP2P;
struct DataSettings;


struct RAM
{
    /// Номер текущего с конца сигнала. 0 - последний считанный сигнал, 1 - предпоследний считанный сигнал.
    static int16 currentSignal;
    /// Инициализирует первоначальное состояние памяти
    static void Init();
    /// Подготовить в хранилище место для хранения новых данных в соответствии с настройками из ds.
    /// !!! ВНИМАНИЕ !!! Если включён режим усреднения, то возвращается указатель на последний фрейм, чтобы вызывающая функция могла усреднить с ней новые данные
    static void PrepareForNewData(DataSettings *ds);
    /// Считать информацию о num с конца сохранённых данных. 0 - последние данные, 1 - предпоследние
    static DataSettings *Read(uint num = 0);
    /// Возвращает количество записей в памяти
    static uint NumberDatas();

    static void PrepareNewFrameP2P();

    static FrameP2P *GetFrameP2P();
    /// Удалить самую новую запись
    static void RemoveNewest();
};
