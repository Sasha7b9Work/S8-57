#include "defines.h"
#include "log.h"
#include "debug.h"
#include "Hardware/Timer.h"



static uint timeStart;
static uint timePrev;
static uint timeCounter;
static uint timeStartCounter;

/// Время старта профилировщика
volatile static uint timeStartMS = 0;
/// Время предыдущей точки профилировщика
volatile static uint timePrevMS = 0;

void *Debug::packet = nullptr;
const char *Debug::file = 0;
int   Debug::line = 0;



void Debug::StartProfilingMS()
{
    timeStartMS = TIME_MS;
    timePrevMS = TIME_MS; //-V656
}


uint Debug::PointProfilingMS(const char *_file, int _line)
{
    uint delta = TIME_MS - timePrevMS;
    LOG_WRITE("%s %d %d", _file, _line, delta);
    timePrevMS = TIME_MS;

    return delta;
}


void Debug::StartProfiling()
{
    timeStart = TIME_US;
    timePrev = timeStart;
}


uint Debug::PointProfiling()
{
    uint result = TIME_US - timePrev;
    timePrev = TIME_US;
    return result;
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
