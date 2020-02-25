#include "defines.h"
#include "Hardware/Memory/IntRAM.h"
#include "Osci/Osci.h"


static DataSettings *ds = nullptr;


void Roller::Prepare()
{
    ds = IntRAM::PrepareForP2P();
}


void Roller::ReadPoint()
{
    //    if (FrameP2P::IsCorrect() && FPGA::IsRunning() && HAL_PIO::Read(PIN_P2P))
    //    {
    //        HAL_BUS::FPGA::SetAddrData(RD::DATA_A, RD::DATA_A + 1);
    //        BitSet16 dataA(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());
    //
    //        HAL_BUS::FPGA::SetAddrData(RD::DATA_B, RD::DATA_B + 1);
    //        BitSet16 dataB(HAL_BUS::FPGA::ReadA0(), HAL_BUS::FPGA::ReadA1());
    //
    //        LOG_WRITE("—читаны точки");
    //
    //        FrameP2P::AddPoint(dataA, dataB);
    //    }
}
