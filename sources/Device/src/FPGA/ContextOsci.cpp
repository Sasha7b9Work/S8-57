#include "defines.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"


uint16 &ContextOsci::pred = FPGA::pred;
uint16 &ContextOsci::post = FPGA::post;
uint &ContextOsci::timeStart = FPGA::timeStart;
bool &ContextOsci::isRunning = FPGA::isRunning;


void ContextOsci::OnPressStart()
{
    if (ModeWork::IsDir())
    {
        FPGA::OnPressStart();
    }
}

bool ContextOsci::InStateStop()
{
    return FPGA_IN_STATE_STOP;
}
