#include "defines.h"
#include "Data/Heap.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Osci/StorageOsci.h"
#include "Utils/Math.h"
#include <cstring>


int16 MemoryRAM::currentSignal = 0;

/// Указатель на самый старый записанный пакет. Он будет стёрт первым
PacketRAM *oldest = reinterpret_cast<PacketRAM *>(Heap::Begin());
/// Указатель на последний записанный пакет. Он будет стёрт последним
PacketRAM *newest = reinterpret_cast<PacketRAM *>(Heap::Begin());


void MemoryRAM::Init()
{
    std::memset(Heap::Begin(), 0x00, 4);
}


void MemoryRAM::Save(const DataSettings *ds)
{
    PacketRAM *packet = newest;

    if(packet->IsEmpty())
    {
        packet->Pack(ds);
    }
    else
    {
        packet->PackNewest(ds);
    }
}


bool PacketRAM::IsEmpty() const
{
    return (addrNext == 0x0);
}


bool MemoryRAM::Read(DataSettings **, uint)
{
    return false;
}


uint MemoryRAM::NumberDatas()
{
    return 0;
}


void PacketRAM::Pack(const DataSettings *)
{

}


PacketRAM *PacketRAM::PackNewest(const DataSettings *)
{
    return nullptr;
}
