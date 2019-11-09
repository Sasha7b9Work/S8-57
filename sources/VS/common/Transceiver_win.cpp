#include "defines.h"
#include "common/Transceiver.h"
#include "Recorder/Recorder.h"


void Transceiver::Init()
{

}


void Transceiver::Send(const uint8 *, uint)
{
    Recorder::ReadPoint();
}


void Transceiver::Send(uint8)
{

}


bool Transceiver::Receive()
{
    return false;
}
