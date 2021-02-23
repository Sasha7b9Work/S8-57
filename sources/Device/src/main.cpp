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

            if (FDrive::OpenNewFileForWrite("test.txt", &sw))
            {
                char buffer[] = "test string";

                if (FDrive::WriteToFile((uint8 *)buffer, (int)std::strlen(buffer), &sw))
                {
                    FDrive::CloseFile(&sw);
                }
            }

        }
    }
}
