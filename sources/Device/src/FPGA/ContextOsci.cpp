#include "defines.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"


uint &ContextOsci::timeStart = FPGA::timeStart;


void ContextOsci::OnPressStart()
{
    if (ModeWork::IsDir())
    {
        FPGA::OnPressStart();
    }
}
