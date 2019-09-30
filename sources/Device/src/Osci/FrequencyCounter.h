#pragma once
#include "Utils/Stack.h"



class FrequencyCounter
{
public:
    /// Инициализация.
    static void Init();
    /// Заслать настройки для счётчика частоты
    static void LoadFreqSettings();
    /// Заслать настрйки для счётчика периода
    static void LoadPeriodSettings();

    static void Update();
    /// Возвращает измеренное значение частоты
    static float GetFreq();
    /// Отрисовка значения
    static void Draw();

private:
    /// Установить состояние лампочек счётчиков в состояние, соответствующее текущему моменту
    static void SetStateLamps();

    static void SetStateLampFreq();

    static void SetStateLampPeriod();

    static void ReadFreq();

    static void ReadPeriod();

    static void LoadSettings();

    static float FreqSetToFreq(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);
    /// Установленное в true значение означает, что частоту нужно считать по счётчику периода
    static bool readPeriod;

    static float prevFreq;

    static float frequency;

    static float period;
};
