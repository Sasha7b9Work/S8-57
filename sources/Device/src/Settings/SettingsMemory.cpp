#include "defines.h"
#include "SettingsMemory.h"

#include <stdlib.h>


using Osci::Settings::Memory::ENumPointsFPGA;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int sMemory_NumBytesInChannel_()
{
    DataSettings ds;
    ds.Fill();
    return ds.SizeChannel();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ENumPointsFPGA::E NumPoints_2_ENumPoints(int numPoints)
{
    if (numPoints == 8192)  { return ENumPointsFPGA::_8k; }
    else if (numPoints == 4096)  { return ENumPointsFPGA::_4k; }
    else if (numPoints == 2048)  { return ENumPointsFPGA::_2k; }
    else if (numPoints == 1024)  { return ENumPointsFPGA::_1k; }
    else
    {
        LOG_ERROR("");
    }
    return ENumPointsFPGA::_512;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int ENumPoints_2_NumPoints(ENumPointsFPGA::E numPoints)
{
    static const int n[ENumPointsFPGA::Size] =
    {
        512,
        1024,
        2048,
        4096,
        8192
    };
    return n[(uint)numPoints];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void *AllocMemForChannelFromHeap(Chan ch, DataSettings *ds)
{
    int numBytes = ch.RequestBytes(ds);
    if (numBytes)
    {
        return malloc((uint)numBytes);
    }
    return 0;
}
