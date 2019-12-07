#pragma once


struct Recorder
{
    static bool IsEnabledA();

    static bool IsEnabledB();

    static bool IsEnabledSensor();

    static void Init();

    static void DeInit();

    static void Update();

    static void OnPressStart();

    static void Start();

    static void Stop();

    static bool IsRunning();

    static void ReadPoint();

    /// Где хранятся данные
    struct TypeMemory
    {
        enum E
        {
            RAM,    ///< ОЗУ
            ROM,    ///< Внутр ЗУ
            EXT     ///< Внешн ЗУ
        };
        static TypeMemory::E &Ref();
    };
};
