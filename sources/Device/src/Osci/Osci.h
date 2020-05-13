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



struct Osci
{
    friend struct Randomizer;
    friend struct ContextTester;

    static void Init();

    static void Update();

    static void OnPressButtonStart();

    static void DeInit();

    // (Если button == true) - это запуск кнопкой
    static void Start(bool button);

    static void Stop();

    static bool IsRunning();
    
    // Это вызываем в случае изменения настройки
    static void Restart();
    
    // Эту функцию надо вызывать при изменении длины памяти. Ну или режима пикового детектора
    static void OnChangedPoints();
   
    // Загрузить значение удержания синхронизации
    static void LoadHoldfOff();
    
    // Эту функцию нужно вызывать при изменении режима запуска
    static void ChangedTrigStartMode();
      
    // Очистка данных рандомизатора при переключении режимов
    static void ClearDataRand();

    // Значение, считанное из handleADC
    static uint16 valueADC;
    
    static int addShift;
    
    static void ReadData();

    // Управитель входными цепями
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };

    static uint16 ReadLastRecord(Chan::E ch);

    // Обработать флаг предзапуска
    static void ProcessFlagPred();

private:

    // Читать данные канала в памяить data
    static bool ReadDataChannel(Chan::E ch, uint8 *data);

    static bool ReadDataChannelRand(uint8 *address, uint8 *data);

    // Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
    static uint16 addrRead;
};


struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *newData, int size);
    static void SettingChanged();
};



class Interpolator
{
public:
    static void Run(DataSettings *ds);
};


class Roller
{
public:
    // Функцию нужно вызывать перед каждым стартом
    static void Prepare();

    // Эту функцию нужно постоянно вызывать во время выполнения программы для чтения точек поточечного фрейма, если мы находимся в поточечном режиме
    static void ReadPoint();

    // Возвращает true, если нужно рисовать поточечный фрейм
    static bool NeedDraw();

    // Возвращает указатель на данные, которые нужно рисовать
    static DataSettings *GetDS();

    // Заполняет buffer точками для отрисовки. width - ширина окна для отрисовки. Возвращает позицию, в которой нужно рисовать разделитель
    static int FillScreenBuffer(Chan::E ch, Buffer &buffer, int width);

private:

    // Функция добавления считанной точки
    static void (*addPoint)(BitSet16, BitSet16);
    static void AddPointPeakDetEnabled(BitSet16 dataA, BitSet16 dataB);
    static void AddPointPeakDetDisabled(BitSet16 dataA, BitSet16 dataB);

    // Возвращает true, если данный фрейм выводится впервые
    static bool FirstDrawThisFrame();

    // Указатель на настройки текущих данных
    static DataSettings *ds;

    // Позиция точки, которая будет записана следующей
    static int currentPoint;

    // С этой точки следует начинать отрисовку текущего поточечного фрейма. Если firstOnDisplay == -1, то нужно запомнить текущую точку в качестве первой выводимой
    static int firstOnDisplay;
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
