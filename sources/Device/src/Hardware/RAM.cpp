#include "defines.h"
#include "Hardware/RAM.h"
#include "Utils/Math.h"
#include "Osci/StorageOsci.h"


static int16 currentSignal = 0;



int16 MemoryRAM::CurrentSignal()
{
    return currentSignal;
}


void MemoryRAM::SelectNextSignal()
{
    Math::CircleIncrease<int16>(&currentSignal, 0, (int16)StorageOsci::NumElementsInStorage() - 1);
}


void MemoryRAM::SelectPrevSignal()
{
    Math::CircleDecrease<int16>(&currentSignal, 0, (int16)StorageOsci::NumElementsInStorage() - 1);
}


void MemoryRAM::ResetSignal()
{
    currentSignal = 0;
}
