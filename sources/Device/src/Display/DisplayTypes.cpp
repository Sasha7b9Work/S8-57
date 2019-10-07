#include "defines.h"
#include "Display/DisplayTypes.h"
#include "Hardware/Timer.h"


WarningStruct::WarningStruct(const char *msg) : message(msg)
{
    timeStart = TIME_MS;
}
