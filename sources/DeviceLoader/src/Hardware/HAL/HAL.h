#pragma once
#include "defines.h"


#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL_EEPROM
{
    static void EraseSector(uint startAddress);
    static void WriteBytes(uint address, const uint8 *data, int size);
};


namespace HAL
{
    void ErrorHandler(const char *, int);
    
    class FSMC
    {
    public:
        static void WriteToPanel(uint8 *buffer, int size);
    };
}

