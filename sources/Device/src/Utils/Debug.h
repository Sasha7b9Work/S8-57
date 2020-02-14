#pragma once
#include "Display/Console.h"
#include "Display/Painter.h"


// Хорошо использовать для трассировке, если потом в HardFault_Handler() отслеживать эти переменные
#define START_PROFILING() Debug::_StartProfilingMS();
#define POINT_PROFILING() Debug::_PointProfilingMS(__FILE__, __LINE__)



#define DEBUG_POINT(x) Debug::line[x] = __LINE__; Debug::file[x] = __FILE__;   VALIDATE_RAM;
//#define DEBUG_POINT_0 DEBUG_POINT(0)
//#define DEBUG_POINT_1 DEBUG_POINT(1)
//#define DEBUG_POINT_2 DEBUG_POINT(2)
//#define DEBUG_POINT_3 DEBUG_POINT(3)

#define DEBUG_TRACE   LOG_WRITE("%s : %d", __FILE__, __LINE__)
//#define DEBUG_TRACE

#define DEBUG_MESSAGE(x)                                \
    LOG_WRITE("%s %d %s", __FUNCTION__, __LINE__, x);   \
    Console::DisableAdding();                           \
    Painter::BeginScene(Color::BLACK);                  \
    Color::WHITE.SetAsCurrent();                        \
    Console::Draw();                                    \
    Painter::EndScene();                                \
    while(true) {}


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
