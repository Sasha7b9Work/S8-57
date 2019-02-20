#include "defines.h"
#include "Hardware/RAM.h"
#include "Utils/Math.h"
#include "Osci/Osci_Storage.h"


static int16 currentSignal = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int16 Memory::RAM::CurrentSignal()
{
    return currentSignal;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Memory::RAM::SelectNextSignal()
{
    Math::CircleIncrease<int16>(&currentSignal, 0, (int16)Osci::Storage::NumElementsInStorage() - 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Memory::RAM::SelectPrevSignal()
{
    Math::CircleDecrease<int16>(&currentSignal, 0, (int16)Osci::Storage::NumElementsInStorage() - 1);
}
