#pragma once
#include "Settings/SettingsOsci.h"


struct Osci
{
    static void Init();

    static void Update();

    static void DeInit();

    static void Start();

    static void Stop(bool pause = false);

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
    /// Балансировать канал
    static void Balance(Chan::E ch);
    /// Загрузить значение удержания синхронизации
    static void LoadHoldfOff();

    static int addShift;

    static const int Kr[TBase::Count];

    /// Структура для хранения информации, необходимой для чтения в режиме рандомизатора
    struct StructReadRand
    {
        int step;       ///< Шаг между точками
        int posFirst;   ///< Позиция первой считанной точки
    };
    /// Возвращает данные, необходимые для чтения даннхы в режмиме рандомизатора.
    /// Если Tsm == 0, то структура будет использоваться не для чтения данных, а для правильного усредения.
    /// В этом случае
    static StructReadRand GetInfoForReadRand(int Tsm = NULL_TSHIFT, const uint8 *address = nullptr);
};

struct AveragerOsci
{
    static void Process(Chan::E ch, const uint8 *dataNew, int size);
    static void SettingChanged();
    static void Draw();
};
