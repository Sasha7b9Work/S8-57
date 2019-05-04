#include "defines.h"
#include "Transceiver.h"
#include "Transceiver.h"


#define PORT_C  GPIOC
#define PORT_E  GPIOE

/// Receiver

#define PORT_READ_REQ_SEND      PORT_C
#define PIN_READ_REQ_SEND       GPIO_PIN_14
#define READ_REQ_SEND           PORT_READ_REQ_SEND, PIN_READ_REQ_SEND

#define PORT_WRITE_ALLOW_SEND   PORT_E
#define PIN_WRITE_ALLOW_SEND    GPIO_PIN_0
#define WRITE_ALLOW_SEND        PORT_WRITE_ALLOW_SEND, PIN_WRITE_ALLOW_SEND

#define PORT_READ_DATA          PORT_E
#define PIN_READ_DATA           GPIO_PIN_1

#define PORT_READ_CLK           PORT_E
#define PIN_READ_CLK            GPIO_PIN_2
#define READ_CLK                PORT_READ_CLK, PIN_READ_CLK

#define PORT_WRITE_CONF_DATA    PORT_E
#define PIN_WRITE_CONF_DATA     GPIO_PIN_3
#define WRITE_CONF_DATA         PORT_WRITE_CONF_DATA, PIN_WRITE_CONF_DATA

/// Transmitter

#define PORT_WRITE_REQ_SEND     PORT_C
#define PIN_WRITE_REQ_SEND      GPIO_PIN_15
#define WRITE_REQ_SEND          PORT_WRITE_REQ_SEND, PIN_WRITE_REQ_SEND

#define PORT_READ_ALLOW_SEND    PORT_E
#define PIN_READ_ALLOW_SEND     GPIO_PIN_4
#define READ_ALLOW_SEND         PORT_READ_ALLOW_SEND, PIN_READ_ALLOW_SEND

#define PORT_WRITE_DATA         PORT_E
#define PIN_WRITE_DATA          GPIO_PIN_5
#define WRITE_DATA              PORT_WRITE_DATA, PIN_WRITE_DATA

#define PORT_WRITE_CLK          PORT_E
#define PIN_WRITE_CLK           GPIO_PIN_6
#define WRITE_CLK               PORT_WRITE_CLK, PIN_WRITE_CLK

#define PORT_READ_CONF_DATA     PORT_E
#define PIN_READ_CONF_DATA      GPIO_PIN_7
#define READ_CONF_DATA          PORT_READ_CONF_DATA, PIN_READ_CONF_DATA



namespace Transceiver
{
    namespace Transmitter
    {
        /// Функции передатчика
        void InitSendPin();
        void InitPins();
        void Write_REQ_SEND(int);
    };
    
    bool Read_ALLOW_SEND();
    bool Read_CONF_DATA();
    void Write_CLK(int);
    void Write_DATA(int);
 
    /// Функции приёмника
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
    GPIO_InitTypeDef gpio =
    {
        PIN_WRITE_REQ_SEND,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };

    HAL_GPIO_Init(PORT_WRITE_REQ_SEND, &gpio);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::InitPins()
{
    GPIO_InitTypeDef gpioOut =
    {
        PIN_WRITE_DATA | PIN_WRITE_CLK,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };

    HAL_GPIO_Init(PORT_E, &gpioOut);

    GPIO_InitTypeDef gpioIn =
    {
        PIN_READ_ALLOW_SEND | PIN_READ_CONF_DATA,
        GPIO_MODE_INPUT
    };

    HAL_GPIO_Init(PORT_E, &gpioIn);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::InitPins_Receiver()
{
    GPIO_InitTypeDef gpioReqSend = { PIN_READ_REQ_SEND, GPIO_MODE_INPUT,  GPIO_PULLDOWN };

    HAL_GPIO_Init(PORT_C, &gpioReqSend);

    GPIO_InitTypeDef gpioIn = { PIN_READ_DATA | PIN_READ_CLK, GPIO_MODE_INPUT, GPIO_PULLDOWN };

    HAL_GPIO_Init(PORT_E, &gpioIn);

    GPIO_InitTypeDef gpioOut = { PIN_WRITE_ALLOW_SEND | PIN_WRITE_CONF_DATA, GPIO_MODE_OUTPUT_PP, GPIO_PULLDOWN };

    HAL_GPIO_Init(PORT_E, &gpioOut);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Write_REQ_SEND(int state)
{
    HAL_GPIO_WritePin(WRITE_REQ_SEND, (GPIO_PinState)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_ALLOW_SEND()
{
    return HAL_GPIO_ReadPin(READ_ALLOW_SEND) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_CONF_DATA()
{
    return HAL_GPIO_ReadPin(READ_CONF_DATA) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_CLK(int state)
{
    HAL_GPIO_WritePin(WRITE_CLK, (GPIO_PinState)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_DATA(int state)
{
    HAL_GPIO_WritePin(WRITE_CLK, (GPIO_PinState)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_REQ_SEND()
{
    return HAL_GPIO_ReadPin(READ_REQ_SEND) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_CONF_DATA(int state)
{
    HAL_GPIO_WritePin(WRITE_CONF_DATA, (GPIO_PinState)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Write_ALLOW_SEND(int state)
{
    HAL_GPIO_WritePin(WRITE_ALLOW_SEND, (GPIO_PinState)state);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::FuncRead(uint8)
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Transceiver::Read_CLK()
{
    return HAL_GPIO_ReadPin(READ_CLK) != 0;
}
