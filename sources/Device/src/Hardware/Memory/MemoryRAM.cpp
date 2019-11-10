#include "defines.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Utils/Math.h"
#include "Osci/StorageOsci.h"


int16 MemoryRAM::currentSignal = 0;



int16 MemoryRAM::CurrentSignal()
{
    return currentSignal;
}


void MemoryRAM::SelectNextSignal()
{
    Math::CircleIncrease<int16>(&currentSignal, 0, static_cast<int16>(StorageOsci::NumElementsInStorage()) - 1);
}


void MemoryRAM::SelectPrevSignal()
{
    Math::CircleDecrease<int16>(&currentSignal, 0, static_cast<int16>(StorageOsci::NumElementsInStorage()) - 1);
}
