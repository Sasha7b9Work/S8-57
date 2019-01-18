#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include <stm32f4xx_hal.h>
#include "FPGA/FPGA_Types.h"
#endif



uint8 *FPGA::ADDR::WR_START = (ADDR_ALTERA + 0);
