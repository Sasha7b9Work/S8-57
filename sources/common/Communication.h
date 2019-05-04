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
        bool(*ReadALLOW_SEND)(),
        bool(*ReadCONF_DATA)(),
        void(*WriteREQ_SEND)(int),
        void(*WriteCLK)(int),
        void(*WriteDATA)(int)
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
        bool(*ReadREQ_SEND)(),
        void(*WriteALLOW_SEND)(int),
        void(*WriteCONF_DATA)(int),
        bool(*ReadCLK)(),
        void(*FuncRead)(uint8)
    );
    /// Функция циклического опроса. Принимает данные, если таковые имеются, и передаёт их через функцию, установленную в SetCallbackReceive.
    void Update();
};

