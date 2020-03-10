#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/IntRAM.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Utils/Math.h"
#include <cstring>


DataSettings *Roller::ds = nullptr;
uint          Roller::currentPoint = 0;
uint          Roller::firstOnDisplay = static_cast<uint>(-1);
void         (*Roller::addPoint)(BitSet16, BitSet16);


void Roller::Prepare()
{
    if(!OSCI_IN_MODE_P2P)
    {
        return;
    }

    ds = IntRAM::PrepareForP2P();
    currentPoint = 0;
    firstOnDisplay = static_cast<uint>(-1);

    addPoint = PEAKDET_ENABLED(ds) ? AddPointPeakDetEnabled : AddPointPeakDetDisabled;
}


void Roller::ReadPoint()
{
    if (FPGA::IsRunning() && HAL_PIO::Read(PIN_P2P))
    {
        HAL_BUS::FPGA::SetAddrData(RD::DATA_A, RD::DATA_A + 1);
        BitSet16 dataA(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());
    
        HAL_BUS::FPGA::SetAddrData(RD::DATA_B, RD::DATA_B + 1);
        BitSet16 dataB(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());
    
        addPoint(dataA, dataB);
    }
}


void Roller::AddPointPeakDetEnabled(BitSet16 dataA, BitSet16 dataB)
{
    if(ds->dataA)
    {
        ds->dataA[currentPoint * 2] = dataA.byte0;
        ds->dataA[currentPoint * 2 + 1] = dataA.byte1;
    }

    if(ds->dataB)
    {
        ds->dataB[currentPoint * 2] = dataB.byte0;
        ds->dataB[currentPoint * 2 + 1] = dataB.byte1;
    }

    Math::CircleIncrease<uint>(&currentPoint, 0, ds->PointsInChannel());
}


void Roller::AddPointPeakDetDisabled(BitSet16 dataA, BitSet16 dataB)
{
    if(ds->dataA)
    {
        ds->dataA[currentPoint] = dataA.byte0;
    }

    if(ds->dataB)
    {
        ds->dataB[currentPoint] = dataB.byte1;
    }

    Math::CircleIncrease<uint>(&currentPoint, 0, ds->PointsInChannel());
}


DataSettings *Roller::GetDS()
{
    return ds;
}


bool Roller::NeedDraw()
{
    if(!OSCI_IN_MODE_P2P)
    {
        return false;
    }

    if(RAM::NumberDatas() == 0)
    {
        return true;
    }

    DataSettings *last = RAM::Get(0);

    if(!last->IsEquals(*ds))
    {
        return true;
    }

    if(TrigStartMode().IsAuto())
    {
        return (TIME_MS - last->timeMS > 1000);
    }
    else
    {
        return !last->IsEquals(*ds);
    }
}


bool Roller::FirstDrawThisFrame()
{
    return firstOnDisplay == static_cast<uint>(-1);
}


int Roller::FillScreenBuffer(Chan::E ch, Buffer &buffer, int width)
{
    uint numBytes = currentPoint;

    if(PEAKDET_ENABLED(ds))
    {
        width *= 2;
        numBytes *= 2;
    }

    if(FirstDrawThisFrame())
    {
        firstOnDisplay = currentPoint;
        if(PEAKDET_ENABLED(ds))
        {
            firstOnDisplay *= 2;
        }
    }

    buffer.Realloc(static_cast<uint>(width));
    std::memset(buffer.data, VALUE::NONE, static_cast<uint>(width));

    uint position = 0;
    uint8 *in = ds->Data(ch);
    uint8 *out = buffer.data;

    for(uint i = firstOnDisplay; i < numBytes; i++)
    {
        out[position] = in[i];
        Math::CircleIncrease<uint>(&position, 0, static_cast<uint>(width - 1));
    }

    return static_cast<int>(PEAKDET_ENABLED(ds) ? (position / 2 - 1) : (position - 1));
}
