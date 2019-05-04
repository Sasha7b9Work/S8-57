#include "defines.h"
#include "Transceiver.h"

/*
    +------------+-------------+---+-------------+---+
    |    Наим.   |    437ZIT       |    429VIT6      |
    |            |     FPGA        |     Панель      |
    +------------+-------------+---+-------------+---+
    |            |  ПЕРЕДАТЧИК |   |  ПРИЁМНИК   |   |
    | REQ_SEND   | 43    PA7   | O | 8     PC14  | I |
    | ALLOW_SEND | 85    PD14  | I | 97    PE0   | O |
    | DATA       | 86    PD15  | O | 98    PE1   | I |
    | CLK        | 114   PD0   | O | 1     PE2   | I |
    | CONF_DATA  | 115   PD1   | I | 2     PE3   | 0 |
    |            |  ПРИЁМНИК   |   |  ПЕРЕДАТЧИК |   |
    | REQ_SEND   | 44    PC4   | I | 9     PC15  | O |
    | ALLOW_SEND | 58    PE7   | O | 3     PE4   | I |
    | DATA       | 59    PE8   | I | 4     PE5   | O |
    | CLK        | 60    PE9   | I | 5     PE6   | O |
    | CONF_DATA  | 63    PE10  | O | 38    PE7   | I |
    +------------+-------------+---+-------------+---+
*/

namespace Transceiver
{
    namespace Transmitter
    {
        /// Функция инициализации пинов для режима передачи
        void InitPins();

        void DeInitPins();

        /// Устновка/сброс REQ_SEND
        void Write_REQ_SEND(int);
        /// Чтение пина разрешения передачи
        bool Read_ALLOW_SEND();
        /// Чтение подтверждения данных
        bool Read_CONF_DATA();
        /// Установить/сбросить тактовый пин
        void Write_CLK(int);
        /// Установить состояния пина данных
        void Write_DATA(int);

        /// Установить пин данных в соответствии с битом bit байта byte
        void WritePinBit(uint8 byte, int bit);
        /// Пересылка байта
        void SendByte(uint8 data);
        /// Дождаться разрешения на передачу
        void WaitPermitSend();
        /// Ожидание подтверждения передачи бита
        void WaitPermitData();
    }

    namespace Receiver
    {
        void InitPins();

        void DeInitPins();

        bool Read_REQ_SEND();
        void Write_ALLOW_SEND(int);
        void Write_CONF_DATA(int);
        bool Read_CLK();
        bool Read_DATA();

        void FuncRead(uint8);
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::Init()
{
    Transmitter::DeInitPins();
    Receiver::DeInitPins();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 *data, uint size)
{
    InitPins();

    Write_REQ_SEND(0);

    WaitPermitSend();

    for (uint i = 0; i < size; i++)
    {
        SendByte(data[i]);
    }

    Write_REQ_SEND(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::SendByte(uint8 data)
{
    for (int i = 0; i < 8; i++)
    {
        Write_CLK(1);
        WritePinBit(data, i);
        Write_CLK(0);
        WaitPermitData();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::WaitPermitSend()
{
    while (Read_ALLOW_SEND() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::WaitPermitData()
{
    while (Read_CONF_DATA() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 byte)
{
    Send(&byte, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };
    Send(data, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Transmitter::WritePinBit(uint8 byte, int bit)
{
    Write_DATA((byte >> bit) & 0x01);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Receiver::Update()
{
    if (!Read_REQ_SEND())           // Если REQ_SEND устновлен в низкое значение - существует запрос на передачу данных
    {
        Write_ALLOW_SEND(0);
    }
}
