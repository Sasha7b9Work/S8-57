#include "defines.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Beeper.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"


int main(void)
{
    Device::Init();
    
    while (1)
    {
        FDrive::Update();

        static bool alreadySave = false;

        if (FDrive::IsConnected() && !alreadySave)
        {
            alreadySave = true;

            StructForWrite sw;

            if (FDrive::OpenNewFileForWrite("eeprom.bin", &sw))
            {
                uint start = 0x8000000;
                uint end = start + (1024 * 1024 * 2);
                
                uint address = start;

                static const int SIZE_BUFFER = 512;

                while(address < end)
                {
                    FDrive::WriteToFile((uint8 *)address, SIZE_BUFFER, &sw);
                    
                    address += SIZE_BUFFER;
                }
                
                FDrive::CloseFile(&sw);
            }

        }
    }
}
