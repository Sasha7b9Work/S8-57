#include "defines.h"
#include "FPGA/FPGA.h"
#include <cstring>
#include "Settings/Settings.h"


uint16 addrRead = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenl(Chan::E ch, uint8 data[MAX_NUM_POINTS])
{
    std::memset(data, (ch == Chan::A) ? (FPGA::VALUE::AVE) : (FPGA::VALUE::AVE + 10), FPGA_NUM_POINTS);
}
