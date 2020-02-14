#pragma once


// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
//#define START_PROFILING() Debug::_StartProfilingMS();
//#define POINT_PROFILING() Debug::_PointProfilingMS(__FILE__, __LINE__)


namespace Debug
{
    void StartProfiling();
    uint PointProfiling();

    void StartProfilingMS();
    uint PointProfilingMS(const char *file, int line);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    extern int line[10];
    extern const char *file[10];
};
