#include "defines.h"
#include "Hardware/Memory/Sector.h"
#include <cstring>


void Sector::Erase() const
{
    if (needLog)
    {
        LOG_WRITE("");
        LOG_WRITE("Стираю сектор %d", number);
        LOG_WRITE("");
    }
    std::memset(reinterpret_cast<void *>(address), 0xFF, size);
}
