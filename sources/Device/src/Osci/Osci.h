#pragma once
#include "Settings/Settings.h"


struct Osci
{
    static void Init();

    static void Update();

    static void OnPressStart();

    static void DeInit();
    /// (Если button == true) - это запуск кнопкой
    static void Start(bool button);

    static void Stop();

    static bool IsRunning();
    /// Это вызываем в случае изменения настройки
    static void Restart();
    /// Возвращает true, если находится в поточечном режиме
    static bool InModeP2P();
    /// Возвращает true, если работает в режиме рандомизатора
    static bool InModeRandomizer();
    /// Эту функцию надо вызывать при изменении длины памяти. Ну или режима пикового детектора
    static void OnChangedPoints();

    static void ReadPointP2P();
    /// Загрузить значение удержания синхронизации
    static void LoadHoldfOff();
    /// Эту функцию нужно вызывать при изменении режима запуска
    static void ChangedTrigStartMode();
    /// Эту функцию нужно вызывать при изменении TBase
    static void ChangedTBase();
    /// Очистка данных рандомизатора при переключении режимов
    static void ClearDataRand();

    static int addShift;

    /// Структура для хранения информации, необходимой для чтения в режиме рандомизатора
    struct StructReadRand
    {
        uint step;       ///< Шаг между точками
        uint posFirst;   ///< Позиция первой считанной точки
    };
    /// Возвращает данные, необходимые для чтения даннхы в режмиме рандомизатора.
    /// Если Tsm == 0, то структура будет использоваться не для чтения данных, а для правильного усредения.
    /// В этом случае
    static StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT, const uint8 *address = nullptr);

    /// Управитель входными цепями
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };

private:
    /// Возвращает true, если уже можно читать данные
    static bool CanReadData();

    static void UpdateFPGA();

    /// Функции старта
    static void (*funcStart)(bool);
    static void StartNormal(bool button);
    static void StartP2P(bool button);
    static void StartWaitP2P(bool button);
    static void StartSingleP2P(bool button);

    /// Функции стопа
    static void (*funcStop)();
    static void StopNormal();
    static void StopWaitP2P();
    static void StopSingleP2P();

    static void SetFunctionsStartStop();
    /// В зависимости от состояния флага готовности данных читает данные и возвращает флаг необходимости остановить процесс сбора информации
    static bool ProcessFlagReady();
    /// Обработать флаг предзапуска
    static void ProcessFlagPred();
};

struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *dataNew, uint size);
    static void SettingChanged();
    static void Draw();
};
