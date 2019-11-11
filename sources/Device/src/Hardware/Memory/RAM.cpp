#include "defines.h"
#include "Data/Heap.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/StorageOsci.h"
#include "Utils/Math.h"
#include <cstring>


int16 RAM::currentSignal = 0;

/// Указатель на самый старый записанный пакет. Он будет стёрт первым
PacketRAM *oldest = reinterpret_cast<PacketRAM *>(Heap::Begin());
/// Указатель на последний записанный пакет. Он будет стёрт последним
PacketRAM *newest = reinterpret_cast<PacketRAM *>(Heap::Begin());


void RAM::Init()
{
    std::memset(Heap::Begin(), 0x00, 4);
}


void RAM::Save(const DataSettings *ds)
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
    return (addrNewest == 0x0000000);
}


bool RAM::Read(DataSettings **, uint)
{
    return false;
}


uint RAM::NumberDatas()
{
    return 0;
}


PacketRAM *PacketRAM::Pack(const DataSettings *)
{
    if(IsEmpty())
    {

    }

    return nullptr;
}


PacketRAM *PacketRAM::PackNewest(const DataSettings *)
{
    return nullptr;
}


uint PacketRAM::Size() const
{
    uint result = 0;

    if(!IsEmpty())
    {
        result = sizeof(PacketRAM) + sizeof(DataSettings) + GetDataSettings()->NeedMemoryForData();
    }

    return result;
}


DataSettings *PacketRAM::GetDataSettings() const
{
    if(IsEmpty())
    {
        return nullptr;
    }

    return reinterpret_cast<DataSettings *>(Address() + sizeof(PacketRAM));
}
