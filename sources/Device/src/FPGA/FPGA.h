#pragma once
#include "Settings/SettingsOsci.h"


#define FPGA_IN_STATE_STOP (FPGA::fpgaStateWork == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (FPGA::fpgaStateWork == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (FPGA::fpgaStateWork == StateWorkFPGA_Wait)



namespace FPGA
{
    const uint MAX_NUM_POINTS = (16 * 1024U);

    void Init();

    void OnPressStart();
    /// Принудительный запуск синхронизации
    void GiveStart();
    /// Возвращает количество байт в канале на текущих настройках
    uint BytesInChannel();

    bool IsRunning();

    void Reset();

    void SetValueADC(uint16 value);

    void ReadData();

    uint16 ReadLastRecord(Chan::E ch);

    bool ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS]);

    void ClearDataRand();
    /// Включить/выключить калибратор.
    void LoadCalibratorMode();
    /// Время подачи старта
    extern uint timeStart;
    /// Значение, считанное из handleADC
    extern uint16 valueADC;
    
    extern StateWorkFPGA fpgaStateWork;

    extern bool isRunning;

    extern uint16 post;
    extern uint16 pred;

    class FreqMeter
    {
    public:
        static float GetFreq();
    };

    class ForTester
    {
    public:
        static bool Read(uint16 *dataA, uint8 *dataB);
        /// Запустить цикл чтения для тестер-компонента. В течение time секунд должно быть считано numPoints точек
        /// Если возвращает false - старт не прошёл
        static bool Start();
    };

    namespace VALUE
    {
        const uint8 AVE = 127;
        const uint8 MIN = AVE - 125;
        const uint8 MAX = AVE + 125;
        /// Если значение == 0, значит, его нет. Это нужно для режимов рандомизатора и поточечного вывода p2p, а также для tShift ранее считанного сигнала
        const uint8 NONE = 0;
    }
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
