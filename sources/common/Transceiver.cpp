#include "defines.h"
#include "Transceiver.h"


DataBusMode::E DataBusMode::state = DataBusMode::FPGA;


/// Установленое в true значение означает, что идёт обмен с панелью
bool Transceiver::inInteraction = false;



void Transceiver::Transmitter::Send(uint8 byte0, uint8 byte1)
{
    uint8 data[2] = { byte0, byte1 };
    Send(data, 2);
}
