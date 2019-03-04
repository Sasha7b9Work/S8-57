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
};
