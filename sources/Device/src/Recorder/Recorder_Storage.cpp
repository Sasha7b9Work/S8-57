#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Recorder/Recorder_Storage.h"

#include <stm32f4xx_hal.h>
#include "Hardware/Timer.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Storage::Init()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Recorder::Storage::Data::NumPoints()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Storage::AddPoint(BitSet16 dataA, BitSet16 dataB)
{
    static uint prevTime = 0;

    uint time = TIME_MS;

    LOG_WRITE("%d мс Получены новые данные %d %d %d %d", time - prevTime, dataA.byte0, dataA.byte1, dataB.byte0, dataB.byte1);

    prevTime = time;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Recorder::Storage::Data &Recorder::Storage::CurrentFrame()
{
    static Data data;
    return data;
}
