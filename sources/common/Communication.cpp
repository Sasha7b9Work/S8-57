#include "defines.h"
#include "Communication.h"

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

namespace Communicator
{
    namespace Transmitter
    {
        /// Функция инициализации пинов для режима передачи
        void InitPins();
        /// Устновка/сброс REQ_SEND
        void(*Write_REQ_SEND)(int) = nullptr;
        /// Чтение пина разрешения передачи
        bool(*Read_ALLOW_SEND)() = nullptr;
        /// Чтение подтверждения данных
        bool(*Read_CONF_DATA)() = nullptr;
        /// Установить пин данных в соответствии с битом bit байта byte
        void WritePinBit(uint8 byte, int bit);
        /// Установить/сбросить тактовый пин
        void(*Write_CLK)(int) = nullptr;
        /// Установить состояния пина данных
        void(*Write_DATA)(int) = nullptr;
        /// Пересылка байта
        void SendByte(uint8 data);
        /// Дождаться разрешения на передачу
        void WaitPermitSend();
        /// Ожидание подтверждения передачи бита
        void WaitPermitData();
    }

    namespace Receiver
    {
        void(*InitPins)() = nullptr;
        bool(*ReadREQ_SEND)() = nullptr;
        void(*WriteALLOW_SEND)(int) = nullptr;
        void(*WriteCONF_DATA)(int) = nullptr;
        bool(*ReadCLK)() = nullptr;
        void(*FuncRead)(uint8) = nullptr;
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Communicator::Transmitter::Send(uint8 *data, uint size)
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
void Communicator::Transmitter::SendByte(uint8 data)
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
void Communicator::Transmitter::WaitPermitSend()
{
    while (Read_ALLOW_SEND() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Transmitter::WaitPermitData()
{
    while (Read_CONF_DATA() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Transmitter::Send(uint8 byte)
{
    Send(&byte, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Transmitter::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };
    Send(data, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Transmitter::WritePinBit(uint8 byte, int bit)
{
    Write_DATA((byte >> bit) & 0x01);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Communicator::Receiver::Update()
{

}
