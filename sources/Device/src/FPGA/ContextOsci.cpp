#include "defines.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"


uint16 &ContextOsci::pred = FPGA::pred;
uint &ContextOsci::timeStart = FPGA::timeStart;
bool &ContextOsci::isRunning = FPGA::isRunning;


void ContextOsci::OnPressStart()
{
    if (ModeWork::IsDir())
    {
        FPGA::OnPressStart();
    }
}
