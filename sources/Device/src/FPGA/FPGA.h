#pragma once
#include "Settings/SettingsOsci.h"
#include "FPGA/Context.h"
#include "FPGA/TypesFPGA.h"


#define FPGA_IN_STATE_STOP (FPGA::fpgaStateWork == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (FPGA::fpgaStateWork == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (FPGA::fpgaStateWork == StateWorkFPGA_Wait)


struct FPGA
{
    static const uint MAX_NUM_POINTS = (16 * 1024U);

    static void Init();

    static void OnPressStart();
    /// Принудительный запуск синхронизации
    static void GiveStart();
    /// Возвращает количество байт в канале на текущих настройках
    static uint BytesInChannel();

    static bool IsRunning();

    static void Reset();

    static void SetValueADC(uint16 value);

    static void ReadData();

    static uint16 ReadLastRecord(Chan::E ch);

    static bool ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS]);

    static void ClearDataRand();
    /// Включить/выключить калибратор.
    static void LoadCalibratorMode();
    /// Время подачи старта
    static uint timeStart;
    /// Значение, считанное из handleADC
    static uint16 valueADC;
    
    static StateWorkFPGA fpgaStateWork;

    static bool isRunning;

    static uint16 post;
    static uint16 pred;

    struct FreqMeter
    {
        static float GetFreq();
    };

    struct ForTester
    {
        static bool Read(uint16 *dataA, uint8 *dataB);
        /// Запустить цикл чтения для тестер-компонента. В течение time секунд должно быть считано numPoints точек
        /// Если возвращает false - старт не прошёл
        static bool Start();
    };

    struct VALUE
    {
        static const uint8 AVE = 127;
        static const uint8 MIN = AVE - 125;
        static const uint8 MAX = AVE + 125;
        /// Если значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного вывода p2p, а также для tShift ранее считанного сигнала
        static const uint8 NONE = 0;
    };

    struct GetFlag
    {
        static bool DATA_READY();
        static bool TRIG_READY();
        static bool PRED();
        static bool FREQ_READY();
        static bool HOLD_OFF();
        static bool PERIOD_READY();
        static bool FREQ_OVERFLOW();
        static bool PERIOD_OVERFLOW();
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };

    static void LoadRegUPR();

    static void ReadFlag();

    static uint16 flag;

    struct GPIO
    {
        static void Init();
        static uint16 GetPin(FPin::E pin);
        static void WritePin(FPin::E pin, int enable);
        static void SetPin(FPin::E pin);
        static void ResetPin(FPin::E pin);
        static void WriteRegisters(FPin::E cs, uint16 value);
    };
};

namespace MathFPGA
{
    float RShift2Abs(int rShift, Range::E range);

    float VoltageCursor(float shiftCurU, Range::E range, uint16 rShift);

    int RShift2Rel(float rShixftAbs, Range::E range);

    float TimeCursor(float shiftCurT, TBase::E tBase);

    void PointsRel2Voltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage);

    uint8 Voltage2Point(float voltage, Range::E range, uint16 rShift);

    float Point2Voltage(uint8 value, Range::E range, uint16 rShift);

    void PointsVoltage2Rel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points);

    void CalculateFFT(float *data, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1);

    float TShift2Abs(int tShift, TBase::E tBase);
}
