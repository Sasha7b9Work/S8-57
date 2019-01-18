#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA_Types.h"
#include <stm32f4xx_hal.h>
#endif


static uint8 *addrAltera = ((uint8*)NOR_MEMORY_ADRESS1);    // -V566


namespace FPGA
{
namespace ADDR
{
namespace WR
{
    uint8 *START                = addrAltera + 0;
    uint8 *TBASE                = addrAltera + 1;
    uint8 *PRED_LO              = addrAltera + 2;
    uint8 *PRED_HI              = addrAltera + 3;
    uint8 *POST_LO              = addrAltera + 4;
    uint8 *POST_HI              = addrAltera + 5;
    uint8 *UPR                  = addrAltera + 6;
    uint8 *FREQMETER            = addrAltera + 8;
    uint8 *TRIG                 = addrAltera + 9;
    uint8 *START_ADDR           = addrAltera + 11;
    uint8 *RESET_COUNTER_FREQ   = addrAltera + 12;
    uint8 *RESET_COUNTER_PERIOD = addrAltera + 13;
    uint8 *TRIG_HOLD_ENABLE     = addrAltera + 16;
    uint8 *TRIG_HOLD_VALUE_LOW  = addrAltera + 17;
    uint8 *TRIG_HOLD_VALUE_MID  = addrAltera + 18;
    uint8 *TRIG_HOLD_VALUE_HI   = addrAltera + 19;
}
namespace RD
{
    uint8 *DATA_A               = addrAltera + 0;
    uint8 *DATA_A_PEAK_MAX      = DATA_A;
    uint8 *DATA_A_PEAK_MIN      = DATA_A_PEAK_MAX + 1;
    uint8 *DATA_B               = addrAltera + 2;
    uint8 *DATA_B_PEAK_MAX      = DATA_B;
    uint8 *DATA_B_PEAK_MIN      = DATA_B_PEAK_MAX + 1;
    uint8 *LAST_RECORD_LO       = addrAltera + 4;
    uint8 *LAST_RECORD_HI       = addrAltera + 5;
    uint8 *FREQ_BYTE_0          = addrAltera + 8;
    uint8 *FREQ_BYTE_1          = addrAltera + 9;
    uint8 *FREQ_BYTE_2          = addrAltera + 12;
    uint8 *FREQ_BYTE_3          = addrAltera + 13;
    uint8 *PERIOD_BYTE_0        = addrAltera + 16;
    uint8 *PERIOD_BYTE_1        = addrAltera + 17;
    uint8 *PERIOD_BYTE_2        = addrAltera + 20;
    uint8 *PERIOD_BYTE_3        = addrAltera + 21;
    uint8 *FLAG_LO              = addrAltera + 24;
    uint8 *FLAG_HI              = addrAltera + 25;
}
}
}
