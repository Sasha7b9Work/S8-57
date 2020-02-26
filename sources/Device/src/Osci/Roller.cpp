#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/IntRAM.h"
#include "Osci/Osci.h"
#include "Utils/Math.h"


DataSettings *Roller::ds = nullptr;
uint          Roller::currentPoint = 0;
void         (*Roller::addPoint)(BitSet16, BitSet16);


void Roller::Prepare()
{
    ds = IntRAM::PrepareForP2P();
    currentPoint = 0;

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
    return OSCI_IN_MODE_P2P;
}
