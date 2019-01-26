#include <stm32f4xx.h>
#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA.h"
#include "FPGA_HAL.h"
#include "FPGA_Settings.h"
#include "AD9286.h"
#include "Data/DataStorage.h"
#include "Display/Display.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Values.h"
#include <stdlib.h>

#include "Hardware/HAL/HAL.h"


using namespace FPGA::HAL::GPIO;


extern bool givingStart;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    class ADC3_
    {
    public:
        static void Init()
        {
            ::HAL::ADC3_::Init();
        }
    };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    givingStart = false;

    HAL::GPIO::Init();

    SetPin(Pin::SPI3_CS1);
    SetPin(Pin::SPI3_CS2);

    ResetPin(Pin::SPI3_SCK);
    ResetPin(Pin::SPI3_DAT);

    AD9286::Init();

    ADC3_::Init();

    DataStorage::Init(Device::State::CurrentMode());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    Buffer dataA(FPGA::NumPoints());    // -V656
    Buffer dataB(FPGA::NumPoints());    // -V656

    ReadDataChanenl(Chan::A, dataA.data);
    ReadDataChanenl(Chan::B, dataB.data);

    DataSettings ds;
    ds.Fill(dataA.data, dataB.data);

    DataStorage::Push(&ds);
}
