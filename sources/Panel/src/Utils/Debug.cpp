#include "log.h"
#include "Debug.h"
#include "Hardware/Timer.h"



uint  Debug::timeStart = 0;
uint  Debug::timeCounter = 0;
uint  Debug::timeStartCounter = 0;
char *Debug::file = 0;
int   Debug::line = 0;
uint  Debug::size = 0;


void Debug::StartProfiling()
{
    timeStart = TIME_US; //-V2571
}


void Debug::PointProfiling(char *name)
{
    LOG_WRITE("%s %d", name, TIME_US - timeStart); //-V2571
}


void Debug::ClearTimeCounter()
{
    timeCounter = 0;
}


void Debug::StartIncreaseCounter()
{
    timeStartCounter = TIME_US; //-V2571
}


void Debug::StopIncreaseCounter()
{
    timeCounter += (TIME_US - timeStartCounter); //-V2571
}


uint Debug::GetTimeCounterUS()
{
    return timeCounter;
}
