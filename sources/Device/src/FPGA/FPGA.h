#pragma once
#include "Settings/SettingsService.h"


/// Ќа столько единиц нужно изменить значение смещени€, чтобы маркер смещени€ по напр€жению передвинулс€ на одну точку.
#define STEP_RSHIFT     (((Osci::Settings::RShift::MAX - Osci::Settings::RShift::MIN) / 24) / 20)
#define STEP_TRIGLEV    STEP_RSHIFT

#define FPGA_IN_STATE_STOP (FPGA::fpgaStateWork == StateWorkFPGA_Stop)
#define FPGA_IN_STATE_WORK (FPGA::fpgaStateWork == StateWorkFPGA_Work)
#define FPGA_IN_STATE_WAIT (FPGA::fpgaStateWork == StateWorkFPGA_Wait)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    const uint MAX_NUM_POINTS = (16 * 1024U);

    void Init();

    void OnPressStart();
    /// ѕринудительный запуск синхронизации
    void GiveStart();
    /// ¬озвращает количество байт в канале на текущих настройках
    uint BytesInChannel();

    bool IsRunning();

    void Reset();

    void SetValueADC(uint16 value);

    void ReadData();

    uint16 ReadLastRecord(Chan::E ch);

    bool ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS]);

    void ClearDataRand();
    /// ¬рем€ подачи старта
    extern uint timeStart;
    /// «начение, считанное из handleADC
    extern uint16 valueADC;
    
    extern StateWorkFPGA fpgaStateWork;

    extern bool isRunning;

    extern uint16 post;
    extern uint16 pred;

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class FreqMeter
    {
    public:
        static float GetFreq();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class ForTester
    {
    public:
        static bool Read(uint16 *dataA, uint8 *dataB);
        /// «апустить цикл чтени€ дл€ тестер-компонента. ¬ течение time секунд должно быть считано numPoints точек
        /// ≈сли возвращает false - старт не прошЄл
        static bool Start();
    };

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    namespace VALUE
    {
        const uint8 AVE = 127;
        const uint8 MIN = AVE - 125;
        const uint8 MAX = AVE + 125;
        /// ≈сли значение == 0, значит, его нет. Ёто нужно дл€ режимов рандомизатора и поточечного вывода p2p, а также дл€ tShift ранее считанного сигнала
        const uint8 NONE = 0;
    }
};
