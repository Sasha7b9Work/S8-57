#include "defines.h"
#include "Transceiver.h"


namespace Transceiver
{
    namespace Transmitter
    {
        void Init();
        /// Инициализация пинов для режима передачи.
        void InitPinsSend();
    }

    namespace Receiver
    {
        void Init();
        /// Инициализация пинов для режима приёма.
        void InitPinsRecieve();
    }

    /// Установленое в true значение означает, что идёт обмен с панелью
    bool inInteraction = false;
}



void Transceiver::Transmitter::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };
    Send(data, 2);
}


bool Transceiver::InInteraction()
{
    return inInteraction;
}
