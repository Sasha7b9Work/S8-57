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
uint8 Multimeter::zero = 0;

/// Послать команду в прибор

namespace Multimeter
{
    static void SendCommand(char *data);
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этот буфер записывается информация в обработчике прерывания приёма
static uint8 bufferUART[10];


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
        static void _Transmit(void *_buffer, uint size, uint timeout)
        {
            HAL::USART3_::Transmit(_buffer, size, timeout);
        }
        /// Запускает приём одного байта
        static void StartReceiveOneByte()
        {
            HAL::USART3_::StartReceiveIT(bufferUART, 1);
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
class Buffer
{
public:
    static void Push(uint8 byte)
    {
        if (pointer == 0 && byte != 0x02)
        {
            return;
        }

        data[pointer++] = byte;

        if (ExistData())
        {
            Multimeter::DisplayWorker::SetMeasure(data);
            pointer = 0;
        }
    };

private:
    static int pointer;

    static uint8 data[20];

    static bool ExistData()
    {
        if (pointer == 0)
        {
            return false;
        }

        if (data[pointer - 1] == 0x0a)
        {
            return true;
        }

        return false;
    }
};

int Buffer::pointer = 0;
uint8 Buffer::data[20];


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

    char send[] = { 0x02, 'V', '0', 0x0a, 0 };

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::ChangeAVP()
{
    ChangeMode();

    char send[] = { 0x02, 'Z', (MULTI_AVP == AVP::On) ? '1' : '0', 0x0a, 0 };

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Calibrate(int i)
{
    char send[] = { 0x02, 'K', 'A', 'L', 'I', 'B', (char)(i | 0x30), 0x0a, 0 };

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::SendCommand(char *data)
{
    ChangeMode();

    uint size = std::strlen(data);

    if (data[size - 1] != 0x0a)
    {
        LOG_ERROR("Неправильные данные");
    }

    USART3_::_Transmit(data, size, 100);

    USART3_::StartReceiveOneByte();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::LoadZero()
{
    ChangeMode();

    char send[] = { 0x02, 'N', (zero | 0x30), 0x0a, 0};

    SendCommand(send);
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

    char send[] = {0x02, symbol, (char)(range + 0x30), 0x0a, 0};

    SendCommand(send);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Multimeter::Measure::E Multimeter::Measure::ForSymbol(char symbol)
{
    Measure::E result = Measure::Size;

    switch (symbol)
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
    // Сохраняем очередной принятый байт
    Buffer::Push(bufferUART[0]);

    // И даём запрос на чтение следующего
    Multimeter::USART3_::StartReceiveOneByte();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Beep(bool /*on*/)
{

}
