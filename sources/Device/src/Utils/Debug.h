#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
#define DEBUG_POINT Debug::line = __LINE__; Debug::file = __FILE__;

// #define START_PROFILING() Debug::_StartProfilingMS();
// #define POINT_PROFILING() Debug::_PointProfilingMS(__FILE__, __LINE__)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Debug
{
    static void StartProfiling();
    static void PointProfiling(char *name);

    static void _StartProfilingMS();
    static uint _PointProfilingMS(const char *file, int line);

    static void ClearTimeCounter();
    static void StartIncreaseCounter();
    static void StopIncreaseCounter();
    static uint GetTimeCounterUS();

    static int line;
    static const char *file;
    static int index;

    static void *prev;
    static void *last;
};
