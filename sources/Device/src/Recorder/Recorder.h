#pragma once


class Recorder
{
public:
    static void Init();

    static void Update();

    static void DeInit();

    static void OnPressStart();

    static void Start();

    static void Stop();

    static bool IsRunning();

    static void ReadPoint();

    // Где хранятся данные
    struct TypeMemory
    {
        enum E
        {
            RAM,    // ОЗУ
            ROM,    // Внутр ЗУ
            EXT     // Внешн ЗУ
        };
    };

    // Используется при выборе оси
    struct Axis
    {
        enum E
        {
            X,
            Y
        };
    };

    // Масшаб по времени
    struct ScaleX
    {
        enum E
        {
            _100ms, // TBase = 0.1c
            _200ms, // TBase = 0.2c
            _500ms, // TBase = 0.5c
            _1s,    // TBase = 1c
            _2s,    // TBase = 2c
            _5s,    // TBase = 5c
            _10s,   // TBase = 10c
            Count
        };
        ScaleX() {}
        static void Load();
        static void Change(int delta);
        static pString ToString();
        // Возвращает скорость потребления памяти в байтах/сек
        int BytesToSec() const;
        // Возвращает время считывания одной точки
        static int TimeForPointMS();
    };
};
