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
namespace FPGA
{
    void Init();

    void Update();

    void Start();

    void OnPressStart();

    void GiveStart();

    void LoadTrigInput();

    void Stop(bool pause = false);

    uint NumPoints();

    bool IsRunning();

    void Reset();

    void LoadRegUPR();
    /// Установить относительный уровень синхронизации
    void SetTrigLev(Trig::Source::E ch, uint16 trigLev);
    /// Установить количество считываемых сигналов в секунду
    void SetENumSignalsInSec(int numSigInSec);
    /// Установить количество измерений, по которым будут рассчитываться ворота в режиме рандомизатора
    void SetNumberMeasuresForGates(int number);
    /// Установить масштаб по времени
    //static void SetTBase(TBase tBase);
    /// Найти и установить уровень синхронизации по последнему считанному сигналу
    void FindAndSetTrigLevel();

    void EnableRecorderMode(bool enable);
    /// Установить добавочное смещение по времени для режима рандомизатора. В каждой развёртке это смещение должно быть разное
    void SetDeltaTShift(int16 shift);

    void SetBandwidth(Chan::E ch);

    ADC_HandleTypeDef *HandleADC();

    void SetValueADC(uint16 value);
    /// Сделать калибровку
    void DoCalibration();

    void GPIO_Init();

    void WritePin(Pin::E pin, int enable);

    void SetPin(Pin::E pin);

    void ResetPin(Pin::E pin);

    static uint GetPin(Pin::E pin);

    static GPIO_TypeDef *GetPort(Pin::E pin);

    void WriteRegisters(Pin::E cs, uint16 value);

    void ReadData();

    static void ReadFlag();

    static uint16 ReadLastRecord();

    void ReadDataChanenl(Chan::E ch, uint8 data[FPGA_MAX_NUM_POINTS]);
    /// Читать канал в рандомизаторе с адреса address
    static void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);

    static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);

    void ClearDataRand();

    static int CalculateShift();
    /// True, если дан запуск
    extern bool givingStart;
    /// Время подачи старта
    extern uint timeStart;

    extern uint16 flag;
    /// Значение, считанное из handleADC
    extern uint16 valueADC;
    
    extern StateWorkFPGA fpgaStateWork;

    extern uint16 post;
    extern uint16 pred;

    extern bool isRunning;

    extern struct State
    {
        bool needCalibration;                       ///< Установленное в true значение означает, что необходимо произвести калибровку.
        StateWorkFPGA stateWorkBeforeCalibration;
        StateCalibration stateCalibration;          ///< Текущее состояние калибровки. Используется в процессе калибровки.
    } state;

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
            _HOLD_OFF_FLAG     = 7,  ///< Принудительный запуск можно давать только когда этот флаг в единице
            _FREQ_OVERFLOW     = 8,  ///< Признак переполнения счётчика частоты
            _PERIOD_OVERFLOW   = 9,  ///< Признак переполнения счётчика периода
            _FREQ_IN_PROCESS   = 10, ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
            _PERIOD_IN_PROCESS = 11  ///< Установленное в единицу значение означает, что идёт процесс измерения - счётчик запущен и считает
        } value;
    };

    struct GetFlag
    {
        static bool DATA_READY();
        static bool TRIG_READY();
        static bool PRED();
        static bool P2P();
        static bool FREQ_READY();
        static bool HOLD_OFF();
        static bool PERIOD_READY();
        static bool FREQ_OVERFLOW();
        static bool PERIOD_OVERFLOW();
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };

    class FreqMeter
    {
    public:
        static float GetFreq();
    };

    class ForTester
    {
    public:
        static bool Read(uint8 *dataA, uint8 *dataB);
        /// Запустить цикл чтения для тестер-компонента. В течение time секунд должно быть считано numPoints точек
        static void Start();
    };
};
