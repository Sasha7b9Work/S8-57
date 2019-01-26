#include "defines.h"
#include "Multimeter.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Hardware/Hardware.h"
#include "Hardware/Timer.h"
#include "Device.h"
#include <cstring>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
char   Multimeter::buffer[11] = {0};
/// В этот буфер записывается информация в обработчике прерывания приёма
static uint8 bufferUART[10];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ReceiveCallback();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::SetMeasure(const uint8 buf[10])
{
    std::memcpy(buffer, buf, 10);
    buffer[2] |= 0x30;
    buffer[3] |= 0x30;
    buffer[4] |= 0x30;
    buffer[5] |= 0x30;
    buffer[6] |= 0x30;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeMode()
{
    std::memset(buffer, '8', 10); //-V512
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Init()
{
    HAL::UART_::Init(ReceiveCallback);

    uint8 send[4] = { 0x02, 'V', '0', 0x0a };

    HAL::UART_::Transmit(send, 4, 10);

    HAL::UART_::StartReceiveIT(bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeAVP()
{
    ChangeMode();
    char send[4] = {0x02, 'Z', MULTI_AVP == AVP::On ? '1' : '0', 0x0a};

    HAL::UART_::Transmit(send, 4, 100);

    HAL::UART_::StartReceiveIT(bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Update()
{
    if (!Device::State::InModeMultimeter())
    {
        return;
    }
    
    uint8 range = 0;
    if(MULTI_MEASURE == Measure::VoltageDC)        { range = (uint8)MULTI_RANGE_DC; }
    else if(MULTI_MEASURE == Measure::VoltageAC)   { range = (uint8)MULTI_RANGE_AC; }
    else if(MULTI_MEASURE == Measure::Resistance)  { range = (uint8)MULTI_RANGE_RESISTANCE; }
    else
    {
        // больше выборов нету
    }

    char symbol = Measure(MULTI_MEASURE).Symbol();

    uint8 send[4] = {0x02, (uint8)symbol, (uint8)(range + 0x30), 0x0a};

    HAL::UART_::Transmit(send, 4, 100);

    HAL::UART_::StartReceiveIT(bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReceiveCallback()
{
    Multimeter::SetMeasure(bufferUART);
    HAL::UART_::StartReceiveIT(bufferUART, 10);
}
