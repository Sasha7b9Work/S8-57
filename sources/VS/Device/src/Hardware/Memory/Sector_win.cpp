#include "defines.h"
#include "Hardware/Memory/Sector.h"
#include <cstring>


void Sector::Erase() const
{
    static int counter = 0;

    if (number == 21)
    {
        counter++;

        if (counter == 23)
        {
            needLog = true;
        }

        if (needLog)
        {
            LOG_WRITE("%d Стираю сектор %d 0x%x", counter, number, address);
        }
    }
    std::memset(reinterpret_cast<void *>(address), 0xFF, size);
}
