#include "defines.h"
#include "FPGA/FPGA.h"


/// Время чтения предыдущих данных в поточечном режиме
static uint timePrevReadP2P = 0;


extern uint16 flagFPGA;


void FPGA::ReadFlag()
{
    flagFPGA = 0xFFFF;
}
