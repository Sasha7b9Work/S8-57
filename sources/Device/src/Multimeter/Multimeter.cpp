#include "defines.h"
#include "Multimeter.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>


using namespace Display::Primitives;



namespace Multimeter
{
    class USART3_
    {
    public:
        static void Init(pFuncVV recvCallback)
        {
            HAL::USART3_::Init(recvCallback);
        }
        static void Transmit(void *_buffer, uint timeout)
        {
            uint8 *pointer = (uint8 *)_buffer;

            uint size = 0;
            while (*pointer != 0x0a)
            {
                size++;
                pointer++;
            }

            HAL::USART3_::Transmit(_buffer, size + 1, timeout);
        }
        static void StartReceiveIT(void *_buffer)
        {
            HAL::USART3_::StartReceiveIT(_buffer, 13);
        }
    };
}



/// В этот буфер записывается информация в обработчике прерывания приёма
static uint8 bufferUART[20];



static void ReceiveCallback();



void Multimeter::ChangeMode()
{
    Display::ChangedMode();
}


void Multimeter::Init()
{
    USART3_::Init(ReceiveCallback);

    uint8 send[] = { 0x02, 'V', '0', 0x0a };

    USART3_::Transmit(send, 10);

    USART3_::StartReceiveIT(bufferUART);
}


void Multimeter::DeInit()
{

}


void Multimeter::ChangeAVP()
{
    ChangeMode();

    char send[] = { 0x02, 'Z', (set.mult.avp == MultimeterAVP::On) ? '1' : '0', 0x0a };

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}


void Multimeter::Calibrate(int calibr)
{
    char send[] = { 0x02, 'K', 'A', 'L', 'I', 'B', (calibr == 0) ? '0' : '1', 0x0a };
    
    USART3_::Transmit(send, 100);
}


void Multimeter::Update()
{
    if (!Device::State::InModeMultimeter())
    {
        return;
    }
    
    uint8 range = 0;
    if(set.mult.meas == MultimeterMeasure::VoltageDC)        { range = (uint8)set.mult.rangeVoltageDC; }
    else if(set.mult.meas == MultimeterMeasure::VoltageAC)   { range = (uint8)set.mult.rangeVoltageAC; }
    else if(set.mult.meas == MultimeterMeasure::CurrentDC)   { range = (uint8)set.mult.rangeCurrentDC; }
    else if(set.mult.meas == MultimeterMeasure::CurrentAC)   { range = (uint8)set.mult.rangeCurrentAC; }
    else if(set.mult.meas == MultimeterMeasure::Resistance)  { range = (uint8)set.mult.rangeResist; }
    else
    {
        // больше выборов нету
    }

    char symbol = MultimeterMeasure(set.mult.meas).Symbol();

    uint8 send[] = {0x02, (uint8)symbol, (uint8)(range + 0x30), 0x0a};

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}


MultimeterMeasure::E MultimeterMeasure::GetCode(const char buffer[13])
{
    MultimeterMeasure::E result = MultimeterMeasure::Size;

    int pos = 0;

    while (pos < 13 && buffer[pos] != 0x0a)
    {
        ++pos;
    }

    if (pos == 13)
    {
        return MultimeterMeasure::Size;
    }

    switch (buffer[pos - 2])
    {
    case 'U':
        result = MultimeterMeasure::VoltageDC;
        break;
    case 'V':
        result = MultimeterMeasure::VoltageAC;
        break;
    case 'I':
        result = MultimeterMeasure::CurrentDC;
        break;
    case 'J':
        result = MultimeterMeasure::CurrentAC;
        break;
    case 'R':
        result = MultimeterMeasure::Resistance;
        break;
    case 'Y':
        result = MultimeterMeasure::TestDiode;
        break;
    case 'W':
        result = MultimeterMeasure::Bell;
        break;
    }

    return result;
}


static void ReceiveCallback()
{
    Multimeter::Display::SetMeasure(bufferUART);
    Multimeter::USART3_::StartReceiveIT(bufferUART);
}
