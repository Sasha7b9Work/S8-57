// 2023/11/21 13:22:52 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Recorder
{
    void Init();

    void DeInit();

    void OnPressStart();

    void Start();

    void Stop();

    // true означает, что регистратор находится в режиме записи
    bool InRecordingMode();

    void RecordPoints();

    // Начать процесс "прослушивания" входов
    void StartListening();

    // Где хранятся данные
#define S_REC_TYPE_MEMORY   (set.rec._typeMemory)
    namespace TypeMemory
    {
        enum E
        {
            RAM,    // ОЗУ
            ROM,    // Внутр ЗУ
            EXT     // Внешн ЗУ
        };
    };

    
    // Используется при выборе оси
#define S_REC_AXIS_MOVE     (set.rec._axisMove)
    namespace Axis
    {
        enum E
        {
            X,
            Y
        };
    };

    
    // Масшаб по времени
#define S_REC_SCALE_X   (set.rec._scaleX)
    namespace ScaleX
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

        void Load();

        void Change(int delta);

        pString ToString();
        
        // Возвращает скорость потребления памяти в байтах/сек
        int BytesToSec();
        
        // Возвращает время считывания одной точки
        int TimeForPointMS();
    };
};
