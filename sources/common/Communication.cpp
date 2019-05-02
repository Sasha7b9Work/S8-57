#include "defines.h"
#include "Communication.h"


namespace Transceiver
{
    /// Функция инициализации пинов для режима передачи
    void(*InitPins)() = nullptr;
    /// Функция устновки в 1 REQ_SEND
    void(*Set_REQ_SEND)() = nullptr;
    /// Функция установки в 0 REQ_SEND
    void(*Reset_REQ_SEND)() = nullptr;
    /// Чтение пина разрешения передачи
    int (*Read_ALLOW_SEND)() = nullptr;
    /// Чтение подтверждения данных
    int(*Read_CONF_DATA)() = nullptr;
    /// Установить пин данных в соответствии с битом bit байта byte
    void(*WritePinBit)(uint8 byte, int bit) = nullptr;
    /// Установить тактовый пин в единицу
    void(*Set_CLK)() = nullptr;
    /// Установить тактовый пин в ноль
    void(*Reset_CLK)() = nullptr;
    /// Пересылка байта
    void SendByte(uint8 data);
    /// Дождаться разрешения на передачу
    void WaitPermitSend();
    /// Ожидание подтверждения передачи бита
    void WaitPermitData();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Transceiver::SetCallbacks(
    void(*initSendPin)(),
    void(*initPins)(),
    void(*setREQ_SEND)(),
    void(*resetREQ_SEND)(),
    int(*readALLOW_SEND)(),
    int(*readCONF_DATA)(),
    void(*setCLK)(),
    void(*resetCLK)())
{
    initSendPin();

    InitPins = initPins;
    Set_REQ_SEND = setREQ_SEND;
    Reset_REQ_SEND = resetREQ_SEND;
    Read_ALLOW_SEND = readALLOW_SEND;
    Read_CONF_DATA = readCONF_DATA;
    Set_CLK = setCLK;
    Reset_CLK = resetCLK;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::Send(uint8 *data, uint size)
{
    InitPins();

    Reset_REQ_SEND();

    WaitPermitSend();

    for (uint i = 0; i < size; i++)
    {
        SendByte(data[i]);
    }

    Set_REQ_SEND();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::SendByte(uint8 data)
{
    for (int i = 0; i < 8; i++)
    {
        Set_CLK();
        WritePinBit(data, i);
        Reset_CLK();
        WaitPermitData();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::WaitPermitSend()
{
    while (Read_ALLOW_SEND() == 1)
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Transceiver::WaitPermitData()
{
    while (Read_CONF_DATA() == 1)
    {
    }
}

namespace Receiver
{

}
