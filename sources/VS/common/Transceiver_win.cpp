#include "defines.h"
#include "Transceiver.h"
#include "Recorder/Recorder.h"


void Transceiver::Init()
{

}


void Transceiver::Transmitter::Send(const uint8 *, uint)
{
    Recorder::ReadPoint();
}


void Transceiver::Transmitter::Send(uint8)
{

}


bool Transceiver::Update()
{
    return false;
}
