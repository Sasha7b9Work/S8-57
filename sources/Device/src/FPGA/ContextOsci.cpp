#include "defines.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"


void ContextOsci::OnPressStart()
{
    if (ModeWork::IsDir())
    {
        FPGA::OnPressStart();
    }
}
