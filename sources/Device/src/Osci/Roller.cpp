#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Osci/Osci.h"


static DataSettings *ds = nullptr;


void Roller::Prepare()
{
    ds = IntRAM::PrepareForP2P();
}
