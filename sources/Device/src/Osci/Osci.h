#pragma once
#include "Osci/Osci_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{
    void Init();

    void Update();

    void DeInit();

    void Start();

    void Stop(bool pause = false);
    /// Это вызываем в случае изменения настройки
    void Restart();
    /// Возвращает true, если находится в поточечном режиме
    bool InModeP2P();
    /// Возвращает true, если работает в режиме рандомизатора
    bool InModeRandomizer();
    /// Эту функцию надо вызывать при изменении длины памяти. Ну или режима пикового детектора
    void OnChangedPoints();

    void ReadPointP2P();
    /// Балансировать канал
    void Balance(Chan::E ch);

    extern int addShift;

    extern const int Kr[Settings::TBase::Size];

    /// Структура для хранения информации, необходимой для чтения в режиме рандомизатора
    struct StructReadRand
    {
        int step;       ///< Шаг между точками
        int posFirst;   ///< Позиция первой считанной точки
    };
    /// Возвращает данные, необходимые для чтения даннхы в режмиме рандомизатора.
    /// Если Tsm == 0, то структура будет использоваться не для чтения данных, а для правильного усредения.
    /// В этом случае
    StructReadRand GetInfoForReadRand(int Tsm = 0);
};
