#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include <stm32f4xx_hal.h>
#include "FPGA/FPGA_Types.h"
#endif



namespace FPGA
{
namespace ADDR
{
namespace WR
{
    uint8 *START     = ADDR_ALTERA + 0;
    uint8 *TBASE     = ADDR_ALTERA + 1;
    uint8 *PRED_LO   = ADDR_ALTERA + 2;
    uint8 *PRED_HI   = ADDR_ALTERA + 3;
    uint8 *POST_LO   = ADDR_ALTERA + 4;
    uint8 *POST_HI   = ADDR_ALTERA + 5;
    uint8 *UPR       = ADDR_ALTERA + 6;
    uint8 *FREQMETER = ADDR_ALTERA + 8;
    uint8 *TRIG      = ADDR_ALTERA + 9;
}
}
}
