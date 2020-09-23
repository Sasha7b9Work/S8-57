#pragma once
#include "Osci/ParametersOsci.h"


// Здесь хранятся настройки, которые определяются при калибровке и не перезаписываются при выключении прибора


#define NRST_NUM_SMOOTH_FOR_RAND    (setNRST._numSmoothForRand)
#define NRST_CORRECTION_TIME        (setNRST._correctionTime)
#define NRST_ENUM_GATE_MAX          (setNRST._enumGateMax)
#define NRST_ENUM_GATE_MIN          (setNRST._enumGateMin)
#define NRST_EX_SHIFT(chan, range)  (setNRST._exShift[chan][range])
#define NRST_EX_STRETCH(chan)       (setNRST._exStretch[chan])
#define NRST_ENUM_AVERAGE_RAND      (setNRST._enumAverageRand)


struct SettingsNRST
{ //-V802
    /*
        ВНИМАНИЕ !!! Для корректной работы нельзя уменьшать размер этой структуры
    */

    uint   size;                                    // Размер структуры. За одно при загрузке из памяти можно определить, что структура пуста - здесь будет значение 0xFFFFFFFF
    int16  _numSmoothForRand;                       // Число точек для скользящего фильта в рандомизаторе.
    int16  _correctionTime;                         // Коэффициент коррекции времени.
    int16  _enumGateMax;                            // Ограничение ворот в рандомизаторе сверху
    int16  _enumGateMin;                            // Ограничение ворот в рандомизаторе снизу
    int8   _exShift[Chan::Count][Range::Count];     // Дополнительное смещение
    float  _exStretch[Chan::Count];                 // Дополнительная растяжка по каналу
    int8   _enumAverageRand;                        // Количество дополнительных усреднений в рандомизаторе
    // Инициализация при включении. Проиходит определние наличия настроек в ROM и их загрузка в случае, если настройки есть. Если настроек нету - инициализация значениями по умолчанию
    void Init();
    // Сохранение настроек в ROM. Происходит только если настройки в setNRST не совпадают с хранящимися в ROM
    void Save();

    void Reset();

    bool operator!=(const SettingsNRST &rhs);

    // Сбросить дополнительное смеещение АЦП
    void SetExtraShift(int8 shift);

    // Сбросить дополнительную растяжку АЦП
    void ResetExtraStretch();
};


extern SettingsNRST setNRST;
