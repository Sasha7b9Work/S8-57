#pragma once
#include "Settings/Settings.h"


struct Osci
{
    static void Init();

    static void Update();

    static void OnPressStart();

    static void DeInit();

    static void Start();

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

private:
    /// Возвращает true, если уже можно читать данные
    static bool CanReadData();

    static void UpdateFPGA();

    /// Функции старта
    static void (*funcStart)();
    static void StartAuto();
    static void StartWait();
    static void StartSingle();

    /// Функции стопа
    static void (*funcStop)();
    static void StopAuto();
    static void StopWait();
    static void StopSingle();
};

struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *dataNew, uint size);
    static void SettingChanged();
    static void Draw();
};
