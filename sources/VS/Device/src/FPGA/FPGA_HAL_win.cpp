#include "defines.h"
#include "FPGA/FPGA.h"


/// Время чтения предыдущих данных в поточечном режиме
static uint timePrevReadP2P = 0;


void FPGA::ReadFlag()
{
    flag.flag = 0xFFFF;
}
