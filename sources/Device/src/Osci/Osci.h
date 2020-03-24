#pragma once
#include "Settings/Settings.h"
#include "Utils/Buffer.h"


#define OSCI_IN_MODE_P2P        (set.time.base >= TBase::MIN_P2P)
#define OSCI_IN_MODE_RANDOMIZER (set.time.base <= TBase::_50ns)


struct Osci
{
    friend struct Randomizer;
    friend struct ContextTester;


    static void Init();


    static void Update();


    static void OnPressStart();


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
    
    // Эту функцию нужно вызывать при изменении TBase
    static void ChangedTBase();
    
    // Очистка данных рандомизатора при переключении режимов
    static void ClearDataRand();

    // Значение, считанное из handleADC
    static uint16 valueADC;

    
    static int addShift;

    
    static void ReadData();

    // Структура для хранения информации, необходимой для чтения в режиме рандомизатора
    struct StructReadRand
    {
        uint step;       ///< Шаг между точками
        uint posFirst;   ///< Позиция первой считанной точки
    };
    
    // Возвращает данные, необходимые для чтения даннхы в режмиме рандомизатора.
    // Если Tsm == 0, то структура будет использоваться не для чтения данных, а для правильного усредения.
    // В этом случае
    static StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT, const uint8 *address = nullptr);

    // Управитель входными цепями
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };

    static uint16 ReadLastRecord(Chan::E ch);

private:
    // Возвращает true, если уже можно читать данные
    static bool CanReadData();

    static void UpdateFPGA();

    // Функции стопа
    static void (*funcStop)();
    static void StopNormal();
    static void StopWaitP2P();
    static void StopSingleP2P();

    static void SetFunctionsStartStop();

    // В зависимости от состояния флага готовности данных читает данные и возвращает флаг необходимости остановить процесс сбора информации
    static bool ProcessFlagReady();

    // Обработать флаг предзапуска
    static void ProcessFlagPred();

    // Читать данные канала в памяить data
    static bool ReadDataChannel(Chan::E ch, uint8 *data);


    static bool ReadDataChannelRand(uint8 *address, uint8 *data);


    static int CalculateShift();

    // Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
    static uint16 addrRead;
};


struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *newData, uint size);
    static void SettingChanged();
};


// Вспомогательная структура для функций рандомизатора
struct Randomizer
{
    // Перемещает считанные данные из временного хранилища в место, указанное ds
    static void MoveReadedData(DataSettings *ds);

private:
    static void MoveReadedDataChannel(DataSettings *ds, Chan::E ch);
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
    static uint currentPoint;

    // С этой точки следует начинать отрисовку текущего поточечного фрейма. Если firstOnDisplay == -1, то нужно запомнить текущую точку в качестве первой выводимой
    static uint firstOnDisplay;
};
