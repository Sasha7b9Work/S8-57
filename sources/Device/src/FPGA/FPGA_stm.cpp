// 2023/11/22 13:36:10 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "FPGA/AD9286.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/BufferButtons.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"


uint16 Osci::addrRead = 0xffff;


void FPGA::Init()
{
    forcedStart = false;

    Osci::InputController::Init();

    HAL_PIO::Set(PIN_SPI3_CS1);
    HAL_PIO::Set(PIN_SPI3_CS2);

    HAL_PIO::Reset(PIN_SPI3_SCK);
    HAL_PIO::Reset(PIN_SPI3_DAT);

    AD9286::Init();

    HAL_ADC3::Init();
}
