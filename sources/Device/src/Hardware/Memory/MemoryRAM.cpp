#include "defines.h"
#include "Data/Heap.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Osci/StorageOsci.h"
#include "Utils/Math.h"
#include <cstring>


int16 MemoryRAM::currentSignal = 0;


void MemoryRAM::Init()
{
    std::memset(Heap::Begin(), 0x00, 4);
}


void MemoryRAM::Save(const DataSettings *ds)
{
    PacketRAM *packet = PacketRAM::Newest();

    if(packet == nullptr)                                   
    {
        packet = reinterpret_cast<PacketRAM *>(Heap::Begin());
        packet->Pack(ds);
    }
    else
    {
        packet->PackNewest(ds);
    }
}


bool MemoryRAM::Read(DataSettings **, uint)
{
    return false;
}


uint MemoryRAM::NumberDatas()
{
    return 0;
}


PacketRAM *PacketRAM::Oldest()
{
    return nullptr;
}


PacketRAM *PacketRAM::Newest()
{
    return nullptr;
}


void PacketRAM::Pack(const DataSettings *)
{

}


PacketRAM *PacketRAM::PackNewest(const DataSettings *)
{
    return nullptr;
}
