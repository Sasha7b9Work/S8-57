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
    /// Включить/выключить калибратор.
    static void LoadCalibratorMode();

    static void SetValueADC(uint16 value);

    static void GiveStart(uint16 pred, uint16 post);

private:

    static void Init();

    static void OnPressStart();
    /// Принудительный запуск синхронизации
    static void ForcedStart();

    static void Reset();

    static void ReadData();

    static uint16 ReadLastRecord(Chan::E ch);

    static bool ReadDataChannel(Chan::E ch, uint8 *data);

    static void ClearDataRand();

    static void LoadRegUPR();

    static void ReadFlag();

    static bool ReadDataChannelRand(Chan::E ch, uint8 *address, uint8 *data);

    static int CalculateShift();

    static uint16 post;
    static uint16 pred;
    /// Значение, считанное из handleADC
    static uint16 valueADC;

    static bool isRunning;

    static FlagFPGA flag;
    /// Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
    static uint16 addrRead;
    /// True, если дан принудительный запуск
    static bool forcedStart;
};

struct GPIO
{
    static void Init();
    static uint16 GetPin(FPin::E pin);
    static void SetPin(FPin::E pin);
    static void ResetPin(FPin::E pin);
    static void WriteRegisters(HPort::E port, uint16 pin, uint16 value);
};
