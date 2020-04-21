#include "defines.h"
#include "device.h"
#include "Multimeter.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"


class USART3_
{
public:
    static void Init(pFuncVV recvCallback)
    {
        HAL_USART3::Init(recvCallback);
    }
    static void Transmit(void *_buffer, uint timeout)
    {
        uint8 *pointer = static_cast<uint8 *>(_buffer);

        uint size = 0;
        while (*pointer != 0x0a)
        {
            size++;
            pointer++;
        }

        HAL_USART3::Transmit(_buffer, size + 1, timeout);
    }
    static void StartReceiveIT(void *_buffer)
    {
        HAL_USART3::StartReceiveIT(_buffer, 13);
    }
};



// В этот буфер записывается информация в обработчике прерывания приёма
static uint8 bufferUART[20];



static void ReceiveCallback();



void Multimeter::ChangeMode()
{
    DisplayMultimeter::ChangedMode();
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

    char send[] = { 0x02, 'Z', AVP::IsEnabled() ? '1' : '0', 0x0a };

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}


void Multimeter::LoadZero(int zero)
{
    ChangeMode();

    char send[] = { 0x02, 'Z', 'E', 'R', 'O', (zero == 0) ? '0' : '1', 0x0a };

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
    if (!Device::InModeMultimeter())
    {
        return;
    }
    
    uint8 range = 0;

    switch(set.mult.meas)
    {
    case MultimeterMeasure::VoltageDC:   range = static_cast<uint8>(set.mult.rangeVoltageDC);   break;
    case MultimeterMeasure::VoltageAC:   range = static_cast<uint8>(RangeAC());                 break;
    case MultimeterMeasure::CurrentDC:   range = static_cast<uint8>(RangeCurrentDC());          break;
    case MultimeterMeasure::CurrentAC:   range = static_cast<uint8>(RangeCurrentAC());          break;
    case MultimeterMeasure::Resistance:  range = static_cast<uint8>(RangeResistance());         break;

    case MultimeterMeasure::TestDiode:
    case MultimeterMeasure::Bell:
    case MultimeterMeasure::Count:
        break;
    }

    char symbol = MultimeterMeasure::Symbol();

    uint8 send[] =
    {
        0x02,
        static_cast<uint8>(symbol),
        static_cast<uint8>(range + 0x30),
        0x0a
    };

    USART3_::Transmit(send, 100);

    USART3_::StartReceiveIT(bufferUART);
}


MultimeterMeasure::E MultimeterMeasure::GetCode(const char buffer[13])
{
    MultimeterMeasure::E result = Count;

    int pos = 0;

    while (pos < 13 && buffer[pos] != 0x0a)
    {
        ++pos;
    }

    if (pos == 13)
    {
        return Count;
    }

    switch (buffer[pos - 2])
    {
    case 'U':
        result = VoltageDC;
        break;
    case 'V':
        result = VoltageAC;
        break;
    case 'I':
        result = CurrentDC;
        break;
    case 'J':
        result = CurrentAC;
        break;
    case 'R':
        result = Resistance;
        break;
    case 'Y':
        result = TestDiode;
        break;
    case 'W':
        result = Bell;
        break;
    }

    return result;
}


static void ReceiveCallback()
{
    DisplayMultimeter::SetMeasure(bufferUART);
    USART3_::StartReceiveIT(bufferUART);
}


Multimeter::RangeAC::E &Multimeter::RangeAC::Ref()
{
    return set.mult.rangeVoltageAC;
}


Multimeter::RangeCurrentAC::E &Multimeter::RangeCurrentAC::Ref()
{
    return set.mult.rangeCurrentAC;
}


Multimeter::RangeCurrentDC::E &Multimeter::RangeCurrentDC::Ref()
{
    return set.mult.rangeCurrentDC;
}


Multimeter::RangeResistance::E &Multimeter::RangeResistance::Ref()
{
    return set.mult.rangeResist;
}


char MultimeterMeasure::Symbol()
{
    static const char symbols[Count] = { 'U', 'V', 'I', 'J', 'R', 'Y', 'W' };
    return symbols[set.mult.meas]; //-V2006
}


bool AVP::IsEnabled()
{
    return set.mult.avp == AVP::On;
}
