#include "defines.h"
#include "Hardware/HAL/HAL.h"


uint Sector::End() const
{
    return address + size;
}
