#include "defines.h"
#include "Transceiver.h"
#include "Transceiver.h"
#include "Hardware/HAL/HAL_PIO.h"


using namespace HAL;


#define PORT_D                  PIO::Port::_D
#define PORT_E                  PIO::Port::_E

// Transmitter

#define PORT_WRITE_REQ_SEND     PIO::Port::_A
#define PIN_WRITE_REQ_SEND      PIO::Pin::_7
#define WRITE_REQ_SEND          PORT_WRITE_REQ_SEND, PIN_WRITE_REQ_SEND

#define PORT_READ_ALLOW_SEND    PORT_D
#define PIN_READ_ALLOW_SEND     PIO::Pin::_14
#define READ_ALLOW_SEND         PORT_READ_ALLOW_SEND, PIN_READ_ALLOW_SEND

#define PORT_READ_CONF_DATA     PORT_D
#define PIN_READ_CONF_DATA      PIO::Pin::_1
#define READ_CONF_DATA          PORT_READ_CONF_DATA, PIN_READ_CONF_DATA

#define PORT_WRITE_DATA         PORT_D
#define PIN_WRITE_DATA          PIO::Pin::_15
#define WRITE_DATA              PORT_WRITE_DATA, PIN_WRITE_DATA

#define PORT_WRITE_CLK          PORT_D
#define PIN_WRITE_CLK           PIO::Pin::_0
#define WRITE_CLK               PORT_WRITE_CLK, PIN_WRITE_CLK

// Receiver

#define PORT_READ_REQ_SEND      PIO::Port::_C
#define PIN_READ_REQ_SEND       PIO::Pin::_4
#define READ_REQ_SEND           PORT_READ_REQ_SEND, PIN_READ_REQ_SEND

#define PORT_WRITE_ALLOW_SEND   PORT_E
#define PIN_WRITE_ALLOW_SEND    PIO::Pin::_7
#define WRITE_ALLOW_SEND        PORT_WRITE_ALLOW_SEND, PIN_WRITE_ALLOW_SEND

#define PORT_READ_DATA          PORT_E
#define PIN_READ_DATA           PIO::Pin::_8
#define READ_DATA               PORT_READ_DATA, PIN_READ_DATA

#define PORT_READ_CLK           PORT_E
#define PIN_READ_CLK            PIO::Pin::_9
#define READ_CLK                PORT_READ_CLK, PIN_READ_CLK

#define PORT_WRITE_CONF_DATA    PORT_E
#define PIN_WRITE_CONF_DATA     PIO::Pin::_10
#define WRITE_CONF_DATA         PORT_WRITE_CONF_DATA, PIN_WRITE_CONF_DATA



namespace Transceiver
{
    namespace Transmitter
    {
        /// Функции передатчика
        void InitSendPin();
        void InitPins();
    };

    void Write_REQ_SEND(int);
    bool Read_ALLOW_SEND();
    bool Read_CONF_DATA();
    void Write_CLK(int);
    void Write_DATA(int);
    /// Функциии приёмника
    void InitPins_Receiver();
    bool Read_REQ_SEND();
    void Write_ALLOW_SEND(int);
    void Write_CONF_DATA(int);
    bool Read_CLK();
    void FuncRead(uint8);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Transmitter::InitSendPin()
{
    PIO::Init(WRITE_REQ_SEND, PIO::Mode::Output_PP, PIO::Pull::Down);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitPins()
{
    PIO::Init(PORT_D,
        (uint)(PIN_READ_ALLOW_SEND |                    // PD14 - ALLOW_SEND
        PIN_READ_CONF_DATA),                            // PD1 - CONF_DATA
        PIO::Mode::Input, PIO::Pull::Down);

    PIO::Init(PORT_D,
        (uint)(PIN_WRITE_DATA |                         // PD15 - DATA
        PIN_WRITE_CLK),                                 // PD0 - CLK
        PIO::Mode::Output_PP, PIO::Pull::Down);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_REQ_SEND(int state)
{
    PIO::Write(WRITE_REQ_SEND, (PIO::State::E)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_ALLOW_SEND()
{
    return PIO::Read(READ_ALLOW_SEND);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_CONF_DATA()
{
    return PIO::Read(READ_CONF_DATA);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_CLK(int state)
{
    PIO::Write(WRITE_CLK, (PIO::State::E)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_DATA(int state)
{
    PIO::Write(WRITE_CLK, (PIO::State::E)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::InitPins_Receiver()
{
    PIO::Init(READ_REQ_SEND, PIO::Mode::Input, PIO::Pull::Down);

    PIO::Init(PORT_E, (uint)(PIN_READ_DATA |PIN_READ_CLK), PIO::Mode::Input, PIO::Pull::Down);

    PIO::Init(PORT_E, (uint)(PIN_WRITE_ALLOW_SEND | PIN_WRITE_CONF_DATA), PIO::Mode::Output_PP, PIO::Pull::Down);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_REQ_SEND()
{
    return PIO::Read(READ_REQ_SEND);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_ALLOW_SEND(int state)
{
    PIO::Write(WRITE_ALLOW_SEND, (PIO::State::E)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_CONF_DATA(int state)
{
    PIO::Write(WRITE_CONF_DATA, (PIO::State::E)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::FuncRead(uint8 /*data*/)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_CLK()
{
    return PIO::Read(READ_CLK);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::InInteraction()
{
    return true;
}
