#include "defines.h"
#include "Hardware/VCP.h"
#include "GUI/ConsoleSCPI.h"


PCD_HandleTypeDef  handlePCD;


bool   DVCP::connectedToUSB = false;
bool   DVCP::cableUSBisConnected = false;


void DVCP::Init()
{

}

void *DVCP::HandlePCD()
{
    return static_cast<void *>(&handlePCD);
}


void DVCP::SendDataAsynch(const uint8 *text, uint)
{
    ConsoleSCPI::Self()->AddText(reinterpret_cast<const char *>(text));
}


void DVCP::SendStringAsynch(const char *message)
{
    String text(">>> %s", message);
    ConsoleSCPI::Self()->AddText(text.c_str());
}
