#pragma once


// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
//#define START_PROFILING_US Debug::StartProfilingUS()
//#define POINT_PROFILING_US Debug::PointProfilingUS(__FILE__, __LINE__)
//
//#define START_PROFILING_MS Debug::StartProfilingMS()
//#define POINT_PROFILING_MS Debug::PointProfilingMS(__FILE__, __LINE__)
//
#define DEBUG_POINT(x) Debug::line[x] = __LINE__; Debug::file[x] = __FILE__;
#define DEBUG_POINT_0  DEBUG_POINT(0)

#define D_POINT Debug::line0 = __LINE__; Debug::file0 = __FILE__;

namespace Debug
{
    void StartProfilingUS();
    uint PointProfilingUS(const char *file, int line);

    void StartProfilingMS();
    uint PointProfilingMS(const char *file, int line);

    void ClearTimeCounter();
    void StartIncreaseCounter();
    void StopIncreaseCounter();
    uint GetTimeCounterUS();

    extern int line[10];
    extern const char *file[10];

    extern int line0;
    extern const char *file0;
};
