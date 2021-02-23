#include "defines.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "FlashDrive/FlashDrive.h"


int main(void)
{
    HAL::Init();

    Timer::Init();

    PAUSE_ON_MS(500);

    FDrive::Init();

    StructForWrite sw;

    FDrive::OpenNewFileForWrite("test.txt", &sw);

    char buffer[] = "test string";

    FDrive::WriteToFile((uint8 *)buffer, (int)std::strlen(buffer), &sw);

    FDrive::CloseFile(&sw);
}
