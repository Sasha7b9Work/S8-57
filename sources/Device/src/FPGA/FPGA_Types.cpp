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
    uint8 *START                = ADDR_ALTERA + 0;
    uint8 *TBASE                = ADDR_ALTERA + 1;
    uint8 *PRED_LO              = ADDR_ALTERA + 2;
    uint8 *PRED_HI              = ADDR_ALTERA + 3;
    uint8 *POST_LO              = ADDR_ALTERA + 4;
    uint8 *POST_HI              = ADDR_ALTERA + 5;
    uint8 *UPR                  = ADDR_ALTERA + 6;
    uint8 *FREQMETER            = ADDR_ALTERA + 8;
    uint8 *TRIG                 = ADDR_ALTERA + 9;
    uint8 *START_ADDR           = ADDR_ALTERA + 11;
    uint8 *RESET_COUNTER_FREQ   = ADDR_ALTERA + 12;
    uint8 *RESET_COUNTER_PERIOD = ADDR_ALTERA + 13;
    uint8 *TRIG_HOLD_ENABLE     = ADDR_ALTERA + 16;
    uint8 *TRIG_HOLD_VALUE_LOW  = ADDR_ALTERA + 17;
    uint8 *TRIG_HOLD_VALUE_MID  = ADDR_ALTERA + 18;
    uint8 *TRIG_HOLD_VALUE_HI   = ADDR_ALTERA + 19;
}
}
}
