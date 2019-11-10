#include "defines.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Utils/Math.h"
#include "Osci/StorageOsci.h"


int16 MemoryRAM::currentSignal = 0;


void MemoryRAM::Save(const DataSettings *ds)
{

}


bool MemoryRAM::Read(DataSettings **ds, uint num)
{
    return false;
}


uint MemoryRAM::NumberDatas()
{
    return 0;
}
