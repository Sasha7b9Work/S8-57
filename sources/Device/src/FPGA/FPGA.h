#pragma once
#include "FPGATypes.h"
#include "Settings/SettingsService.h"
#include <stm32f4xx_hal.h>


/// На столько единиц нужно изменить значение смещения, чтобы маркер смещения по напряжению передвинулся на одну точку.
#define STEP_RSHIFT     (((RShift::MAX - RShift::MIN) / 24) / 20)
#define STEP_TRIGLEV    STEP_RSHIFT

struct Pin
{
    enum E
    {
        SPI3_SCK,
        SPI3_DAT,
        SPI3_CS1,
        SPI3_CS2,
        A1,
        A2,
        A3,
        A4,
        LF1,
        LF2,
        A1S,
        A0S,
        LFS,
        Number
    } value;
    explicit Pin(E v) : value(v) {};
};

#define FPGA_IN_STATE_STOP (FPGA::fpgaStateWork == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (FPGA::fpgaStateWork == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (FPGA::fpgaStateWork == StateWorkFPGA_Wait)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class HardwareFPGA
{
    friend class FPGA;
private:
};

class FPGA
{
friend class TrigLev;
public:

    static void Init();

    static void Update();

    static void Start();

    static void OnPressStart();

    static void GiveStart();

    static void LoadSettings();

    static void IncreaseRange(Chan::E ch);

    static void DecreaseRange(Chan::E ch);

    static void IncreaseTBase();

    static void DecreaseTBase();

    static void RShiftChange(Chan::E ch, int delta);

    static void TrigLevChange(int delta);

    static void TShiftChange(int delta);

    static void SetRShift(Chan::E ch, uint16 rShift);

    static void LoadTrigPolarity();

    static void LoadTrigMode();

    static void LoadTrigSource();

    static void LoadTrigInput();

    static void Stop(bool pause = false);

    static void LoadRanges();
    /// Запустить цикл чтения для тестер-компонента. В течение time секунд должно быть считано numPoints точек
    static void StartForTester(int numPoints);

    class ForTester
    {
    friend class Tester;
    private:
        static bool Read(uint8 *dataA, uint8 *dataB);
    };

    static uint NumPoints();

    static bool IsRunning()
    {
        return isRunning; 
    }

    static bool InStateStop()
    {
        return false;
    }

    static void LoadTShift();

    static struct State
    {
        bool needCalibration;                       ///< Установленное в true значение означает, что необходимо произвести калибровку.
        StateWorkFPGA stateWorkBeforeCalibration;
        StateCalibration stateCalibration;          ///< Текущее состояние калибровки. Используется в процессе калибровки.
    } state;
    
    static StateWorkFPGA GetStateWork();

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    static void SetTShift(int tShift);

    static void Reset();

    static void SetModeCouple(Chan::E ch, ModeCouple::E couple);

    class FreqMeter
    {
    public:
        static float GetFreq();
    };

    struct GetFlag
    {
        static bool DATA_READY();
        static bool TRIG_READY();
        static bool PRED();
        static bool P2P();
        static bool FREQ_READY();
        static bool PERIOD_READY();
        static bool FREQ_OVERFLOW();
        static bool PERIOD_OVERFLOW();
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };

    /// Установить относительный уровень синхронизации
    static void SetTrigLev(Trig::Source::E ch, uint16 trigLev);
    /// Установить количество считываемых сигналов в секунду
    static void SetENumSignalsInSec(int numSigInSec);
    /// Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора
    static void SetNumberMeasuresForGates(int number);
    /// Установить масштаб по времени
    static void SetTBase(TBase tBase);
    /// Найти и установить уровень синхронизации по последнему считанному сигналу
    static void FindAndSetTrigLevel();
    /// Включить/выключить калибратор.
    static void SetCalibratorMode(CalibratorMode calibratorMode);

    static void EnableRecorderMode(bool enable);
    /// Установить добавочное смещение по времени для режима рандомизатора. В каждой развёртке это смещение должно быть разное
    static void SetDeltaTShift(int16 shift);

    static void SetBandwidth(Chan::E ch);

    static ADC_HandleTypeDef *HandleADC() { return &handleADC; };

    static void SetValueADC(uint16 value) { valueADC = value; };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

private:

    static void GPIO_Init();

    static void LoadTBase();

    static void LoadTrigLev();

    static void LoadRShift(Chan::E ch);

    static void WritePin(Pin::E pin, int enable);

    static void SetPin(Pin::E pin);

    static void ResetPin(Pin::E pin);

    static uint GetPin(Pin::E pin);

    static GPIO_TypeDef *GetPort(Pin::E pin);

    static void WriteRegisters(Pin::E cs, uint16 value);

    static void ReadData();

    static void ReadFlag();

    static uint16 ReadLastRecord();

    static void ReadDataChanenl(Chan::E ch, uint8 data[FPGA_MAX_NUM_POINTS]);
    /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
    static void LoadTrigSourceInput();
    /// Читать канал в рандомизаторе с адреса address
    static void ReadDataChanenlRand(Chan::E ch, uint8 *address, uint8 *data);

    static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);

    static int CalculateShift(Chan::E ch);

    static uint8 ValueForRange(Chan::E ch);

    static bool isRunning;
    /// True, если дан запуск
    static bool givingStart;
    /// Время подачи старта
    static uint timeStart;

    struct Flag
    {
        enum E
        {
            _DATA_READY        = 0,  ///< Данные готовы для считывания (окончание счётчика послезапуска)
            _TRIG_READY        = 1,  ///< Флаг синхроимпульса
            _PRED              = 2,  ///< Если 1, то предзапуск отсчитал, можно давать принудительный запуск (окончание
            _P2P               = 3,  ///< 
            _FREQ_READY        = 4,  ///< Флаг готовности измерения частоты
            _PERIOD_READY      = 5,  ///< Флаг готовности измерения периода
            _FREQ_OVERFLOW     = 8,  ///< Признак переполнения счётчика частоты
            _PERIOD_OVERFLOW   = 9,  ///< Признак переполнения счётчика периода
            _FREQ_IN_PROCESS   = 10, ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
            _PERIOD_IN_PROCESS = 11  ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
        } value;
    };

    static uint16 flag;
    /// Используется в режиме рандомизатора
    static ADC_HandleTypeDef handleADC;
    /// Значение, считанное из handleADC
    static uint16 valueADC;
    
public:
    static StateWorkFPGA fpgaStateWork;

    static uint16 post;
    static uint16 pred;
};
