#pragma once
#include "Settings/Settings.h"
#include "FPGA/TypesFPGA.h"
#include "FPGA/MathFPGA.h"


struct FlagFPGA
{
    static uint16 flag;

    FlagFPGA() { flag = 0; }

    static bool DataReady();
    static bool TrigReady();
    static bool Pred();
    static bool FreqReady();
    static bool HoldOff();
    static bool PeriodReady();
    static bool FreqOverflow();
    static bool PeriodOverflow();
    static bool FreqInProcess();
    static bool PeriodInProcess();
};


struct FPGA
{
    friend struct ContextFreqMeter;
    friend struct ContextOsci;
    friend struct ContextRecorder;
    friend struct ContextTester;
    friend struct Randomizer;

    friend struct Bandwidth;
    friend struct Device;
    friend struct Range;
    friend struct RShift;
    friend struct TBase;
    friend struct TrigInput;
    friend struct TrigPolarity;
    friend struct TShift;
    friend struct TrigLevel;
    friend struct Osci;

    static bool IsRunning();

    // Включить/выключить калибратор.
    static void LoadCalibratorMode();


    static void GiveStart(uint16 pred, uint16 post);


    static FlagFPGA flag;


    static const int MAX_NUM_POINTS = 8 * 1024;

private:


    static void Init();


    static void OnPressStart();
    
    // Принудительный запуск синхронизации
    static void ForcedStart();


    static void Reset();


    static void LoadRegUPR();


    static void ReadFlag();


    static uint16 post;
    static uint16 pred;
    

    static bool isRunning;
   
    // True, если дан принудительный запуск
    static bool forcedStart;
};
