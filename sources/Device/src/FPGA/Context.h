#pragma once


struct OsciC
{
    static void LoadRegUPR();

    static void LoadCalibratorMode();

    static void OnPressStart();

    static bool IsRunning();

    static void ReadFlag();

    static void GiveStart();

    static void ReadData();

    static void Reset();

    static void SetValueADC(uint16 value);

    static uint BytesInChannel();

    static uint16 &post;
    static uint16 &pred;
    static uint &timeStart;
    static bool &isRunning;

    struct GetFlag
    {
        static bool PRED();
        static bool HOLD_OFF();
        static bool TRIG_READY();
        static bool DATA_READY();
    };
};


struct FreqMeterC
{
    struct GetFlag
    {
        static bool FREQ_READY();
        static bool PERIOD_READY();
        static bool FREQ_OVERFLOW();
        static bool PERIOD_OVERFLOW();
        static bool FREQ_IN_PROCESS();
        static bool PERIOD_IN_PROCESS();
    };
};


struct RecorderC
{
    static void LoadRegUPR();
};


struct TesterC
{
    static void OnPressStart();
    static bool Start();
    static bool Read(uint16 *dataA, uint8 *dataB);
};
