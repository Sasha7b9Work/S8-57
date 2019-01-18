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
    uint8 *WR_START   = ADDR_ALTERA + 0;
    uint8 *WR_TBASE   = ADDR_ALTERA + 1;
    uint8 *WR_PRED_LO = ADDR_ALTERA + 2;
    uint8 *WR_PRED_HI = ADDR_ALTERA + 3;
    uint8 *WR_POST_LO = ADDR_ALTERA + 4;
    uint8 *WR_POST_HI = ADDR_ALTERA + 5;
}
}
