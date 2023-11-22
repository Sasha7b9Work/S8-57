// 2023/11/22 13:46:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Utils/Buffer.h"


#define OSCI_IN_MODE_P2P        (S_TIME_BASE >= TBase::MIN_P2P)
#define OSCI_IN_MODE_RANDOMIZER (S_TIME_BASE <= TBase::_50ns)



namespace OsciStateWork
{
    enum E
    {
        Stopped,        // Осциллограф в состоянии останова
        Awaiting,       // Осциллограф в состоянии ожидания синхронизации
        Triggered       // Синхронизировани
    };

    OsciStateWork::E Current();
};



namespace Osci
{
    void Init();

    void Update();

    void OnPressButtonStart();

    void DeInit();

    // (Если button == true) - это запуск кнопкой
    void Start(bool button);

    void Stop();

    bool IsRunning();

    // Это вызываем в случае изменения настройки
    void Restart();

    // Эту функцию надо вызывать при изменении длины памяти. Ну или режима пикового детектора
    void OnChangedLengthMemoryChannel();

    // Эту функцию нужно вызывать при изменении режима запуска
    void ChangedTrigStartMode();

    // Очистка данных рандомизатора при переключении режимов
    void ClearDataRand();

    void ReadData();

    uint16 ReadLastRecord(Chan::E ch);

    // Обработать флаг предзапуска
    void ProcessFlagPred();

    // Выполнить поиск сигнала
    void RunAutoSearch();

    // Послать данные в SCPI, если это необходимо
    void SendDataToSCPI();

    // Управитель входными цепями
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };

    // Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
    extern uint16 addrRead;

    // Читать данные канала в памяить data
    bool ReadDataChannel(Chan::E ch, uint8 *data);

    bool ReadDataChannelRand(uint8 *address, uint8 *data);
};


namespace Roller
{
    // Функцию нужно вызывать перед каждым стартом
    void Prepare();

    // Эту функцию нужно постоянно вызывать во время выполнения программы для чтения точек поточечного фрейма, если мы находимся в поточечном режиме
    void ReadPoint();

    // Возвращает true, если нужно рисовать поточечный фрейм
    bool NeedDraw();

    // Возвращает указатель на данные, которые нужно рисовать
    DataSettings *GetDS();

    // Заполняет buffer точками для отрисовки. width - ширина окна для отрисовки. Возвращает позицию, в которой нужно рисовать разделитель
    int FillScreenBuffer(Chan::E ch, Buffer &buffer, int width);
};


// 
struct ShiftPoint
{
    enum E
    {
        FAIL,   // Смещение рассчитать не удалось, точки считывать не надо
        READED  // Нормально считанная точка
    } type;

    int shift;

    ShiftPoint(E t = FAIL, int s = 0) : type(t), shift(s)
    {
    }
};
