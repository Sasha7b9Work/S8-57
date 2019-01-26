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
namespace Multimeter
{
    class USART3_
    {
    public:
        static void Init(pFuncVV recvCallback)
        {
            HAL::USART3_::Init(recvCallback);
        }
        static void Transmit(void *_buffer, uint size, uint timeout)
        {
            HAL::USART3_::Transmit(_buffer, size, timeout);
        }
        static void StartReceiveIT(void *_buffer, uint size)
        {
            HAL::USART3_::StartReceiveIT(_buffer, size);
        }
    };
}


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
    USART3_::Init(ReceiveCallback);

    uint8 send[4] = { 0x02, 'V', '0', 0x0a };

    USART3_::Transmit(send, 4, 10);

    USART3_::StartReceiveIT(bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeAVP()
{
    ChangeMode();

    char send[4] = { 0x02, 'Z', MULTI_AVP == AVP::On ? '1' : '0', 0x0a };

    USART3_::Transmit(send, 4, 100);

    USART3_::StartReceiveIT(bufferUART, 10);
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

    USART3_::Transmit(send, 4, 100);

    USART3_::StartReceiveIT(bufferUART, 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReceiveCallback()
{
    Multimeter::SetMeasure(bufferUART);
    Multimeter::USART3_::StartReceiveIT(bufferUART, 10);
}
