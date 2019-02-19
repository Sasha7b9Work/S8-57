#include "defines.h"
#include "Multimeter.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include "Device.h"
#include <cstring>

#include "Hardware/HAL/HAL.h"


using namespace Display::Primitives;
using namespace Multimeter::Settings;


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
        static void StartReceiveIT(void *_buffer)
        {
            HAL::USART3_::StartReceiveIT(_buffer, 13);
        }
    };

    class DisplayWorker
    {
    public:
        static void ChangedMode()
        {
            Display::ChangedMode();
        }
        static void SetMeasure(const uint8 *buffer)
        {
            Display::SetMeasure(buffer);
        }
    };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этот буфер записывается информация в обработчике прерывания приёма
static uint8 bufferUART[20];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void ReceiveCallback();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::ChangeMode()
{
    DisplayWorker::ChangedMode();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Init()
{
    USART3_::Init(ReceiveCallback);

    uint8 send[4] = { 0x02, 'V', '0', 0x0a };

    USART3_::Transmit(send, 4, 10);

    USART3_::StartReceiveIT(bufferUART);
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

    USART3_::StartReceiveIT(bufferUART);
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

    USART3_::StartReceiveIT(bufferUART);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Multimeter::Measure::E Multimeter::Measure::GetCode(char buffer[13])
{
    Measure::E result = Measure::Number;

    int pos = 0;

    while (pos < 13 && buffer[pos] != 0x0a)
    {
        ++pos;
    }

    if (pos == 13)
    {
        return Measure::Number;
    }

    switch (buffer[pos - 1])
    {
    case 'V':
        result = Measure::VoltageAC;
        break;
    case 'I':
        result = Measure::CurrentDC;
        break;
    case 'J':
        result = Measure::CurrentAC;
        break;
    case 'R':
        result = Measure::Resistance;
        break;
    case 'Y':
        result = Measure::TestDiode;
        break;
    case 'W':
        result = Measure::Bell;
        break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReceiveCallback()
{
    LOG_WRITE((char *)bufferUART);
    Multimeter::DisplayWorker::SetMeasure(bufferUART);
    Multimeter::USART3_::StartReceiveIT(bufferUART);
}
