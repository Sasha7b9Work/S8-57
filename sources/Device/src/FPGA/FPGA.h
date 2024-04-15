// 2023/11/22 13:36:19 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


struct PredFPGA
{
    PredFPGA(uint16 raw) : value(raw) { }
    void SetRaw(uint16 raw) { value = raw; }
    uint16 ConvertForReal() { value = (uint16)(~(value + 3)); return value; }
    uint16 ConvertForRand() { value = (uint16)(~value); return value; }
    uint16 Get() const { return value; }
private:
    uint16 value;
};


extern PredFPGA predFPGA;


struct FPGA
{
    friend struct ContextFreqMeter;
    friend struct ContextOsci;
    friend struct ContextRecorder;
    friend struct Randomizer;

    friend struct Bandwidth;
    friend struct TrigInput;
    friend struct TrigPolarity;
    friend struct TrigLevel;

    static void Init();

    static bool IsRunning();

    // Включить/выключить калибратор.
    static void LoadCalibratorMode();

    static void GiveStart(uint16 pred, uint16 post);

    static const int MAX_NUM_POINTS = 8 * 1024;

    // Принудительный запуск синхронизации
    static void ForcedStart();

    static void LoadRegUPR();

    struct Flag
    {
        static void Read(bool updateFreqMeter = true);

        static void Clear();
        static bool DataReady();
        static bool TrigReady();
        static bool Pred();
        static bool HoldOff();

        static bool FreqReady();
        static bool PeriodReady();
        static bool FreqOverflow();
        static bool PeriodOverflow();
        static bool FreqInProcess();
        static bool PeriodInProcess();
    };

    // True, если дан принудительный запуск
    static bool forcedStart;

    static uint16 post;

    static bool isRunning;

    static void Reset();

    static void OnPressStart();
};
