#include "defines.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"



void HAL_EEPROM::WriteBufferBytes(uint, void *, int)
{

}


void HAL_EEPROM::EraseSector(uint)
{
    Timer::PauseOnTime(2000);
}


void HAL_EEPROM::WriteBytes(uint, const uint8 *, int)
{

}