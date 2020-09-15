#include "defines.h"
#include "Hardware/VCP.h"
#include "GUI/ConsoleSCPI.h"
#include "Utils/String.h"


PCD_HandleTypeDef  handlePCD;


bool VCP::connectedToUSB = false;
bool VCP::cableUSBisConnected = false;


void VCP::Init()
{

}


void VCP::SendDataAsynch(const uint8 *text, int)
{
    ConsoleSCPI::Self()->AddText(reinterpret_cast<const char *>(text));
}



static int Position0D(const char *message)
{
    int position = 0;

    while (*message)
    {
        if (*message == 0x0D)
        {
            return position;
        }
        message++;
        position++;
    }
    return -1;
}


void VCP::SendStringAsynch(const char *message)
{
    static char buffer[32 * 1024] = { 0 };

    int position0D = Position0D(message);

    if (position0D < 0)
    {
        std::strcat(buffer, message);
    }
    else
    {
        std::strcat(buffer, message);
        String text(">>> %s", buffer);
        ConsoleSCPI::Self()->AddText(text.c_str());
        buffer[0] = 0;
    }
}
