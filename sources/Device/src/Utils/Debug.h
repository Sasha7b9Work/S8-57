#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
// #define DEBUG_POINT Debug::line = __LINE__; Debug::file = __FILE__;

// #define START_PROFILING() Debug::_StartProfilingMS();
// #define POINT_PROFILING() Debug::_PointProfilingMS(__FILE__, __LINE__)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Debug
{
    void StartProfiling();
    void PointProfiling(char *name);

    void _StartProfilingMS();
    uint _PointProfilingMS(const char *file, int line);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    extern int line;
    extern const char *file;
};
