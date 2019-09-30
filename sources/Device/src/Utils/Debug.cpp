#include "defines.h"
#include "debug.h"
#include "log.h"
#include "Hardware/Timer.h"



static uint timeStart;
static uint timeCounter;
static uint timeStartCounter;

/// Время старта профилировщика
volatile static uint timeStartMS = 0;
/// Время предыдущей точки профилировщика
volatile static uint timePrevMS = 0;

const char *Debug::file = 0;
int   Debug::line = 0;



void Debug::_StartProfilingMS()
{
    timeStartMS = TIME_MS;
    timePrevMS = TIME_MS; //-V656
}


uint Debug::_PointProfilingMS(const char *_file, int _line)
{
    uint delta = TIME_MS - timePrevMS;
    LOG_WRITE("%s %d %d", _file, _line, delta);
    timePrevMS = TIME_MS;

    return delta;
}


void Debug::StartProfiling()
{
    timeStart = TIME_US;
}


void Debug::PointProfiling(char *name)
{
    LOG_WRITE("%s %d", name, TIME_US - timeStart);
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
    timeStartCounter = TIME_US;
}


void Debug::StopIncreaseCounter()
{
    timeCounter += (TIME_US - timeStartCounter);
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
