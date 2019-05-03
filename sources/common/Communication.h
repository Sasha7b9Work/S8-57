#pragma once

/*
    Функциональность взаимодействия между процессорами
*/


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Transceiver
{
    void SetCallbacks(
        void(*InitSendPin)(),
        void(*InitPins)(),
        void(*SetREQ_SEND)(),
        void(*ResetREQ_SEND)(),
        int(*ReadALLOW_SEND)(),
        int(*ReadCONF_DATA)(),
        void(*SetCLK)(),
        void(*ResetCLK)()
    );
    /// Передача size байт, начиная с адреса data
    void Send(uint8 *data, uint size);
    void Send(uint8 data);
    void Send(uint8 byte0, uint8 byte1);
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Receiver
{
    void SetCallbacks(
        void(*InitPins)(),
        int(*ReadREQ_SEND)(),
        void(*SetALLOW_SEND)(),
        void(*ResetALLOW_SEND)(),
        void(*SetCONF_DATA)(),
        void(*ResetCONF_DATA)(),
        int(*ReadCLK)(),
        void(*FuncRead)(uint8)
    );
    /// Функция циклического опроса. Принимает данные, если таковые имеются, и передаёт их через функцию, установленную в SetCallbackReceive.
    void Update();
};

