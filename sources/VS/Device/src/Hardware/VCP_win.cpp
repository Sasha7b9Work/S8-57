#include "defines.h"
#include "Hardware/VCP.h"
#include "ConsoleSCPI.h"


PCD_HandleTypeDef  handlePCD;


bool   VCP::connectedToUSB = false;
bool   VCP::cableUSBisConnected = false;


void VCP::Init()
{

}

void *VCP::HandlePCD()
{
    return static_cast<void *>(&handlePCD);
}


void VCP::SendDataAsynch(const uint8 *text, uint)
{
    ConsoleSCPI::AddLine(reinterpret_cast<const char *>(text));
}
