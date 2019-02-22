/*
    Захрузчик располагается по адресу 0x08000000
    После сброса он проверяет наличие флешки.
        Если флешка есть:
            проверяет наличие файла S8-54.bin. Если таковой имеется, выводит сообщение "На USB-диске обнаружено новоое программное обеспечение. Установить?"
            Если нажато "Да":
                1. Стираются сектора:
                    5 - 0x08020000
                    6 - 0x08040000
                    7 - 0x08060000
                2. На их место записывается содержимое файла S8-54.bin
        Если флешку примонтировать не удалось:
            Вывести сообщение "Не удалось примонтировать флешку. Убедитесь, что на ней файловая система fat32"
    Далее выполняется переход по адресу, указанному в 0x0802004 (второе 32х-битное слово в таблице векторов, расположенной по адресу 0x0802000)
*/

#include "defines.h"
#include <ff.h>
#include "main.h"
#include "Hardware/CPU.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define FILE_NAME "S8-57.bin"

typedef void(*pFunction)();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
MainStruct *ms; //-V707


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Upgrade();


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int main()
{
    ms = (MainStruct *)malloc(sizeof(MainStruct));
    ms->percentUpdate = 0.0F; //-V522
   
    CPU::Init();

    Settings::Load();

    Timer::PauseOnTime(250);
    
    Display::Init();
    
    ms->state = State_Start;

    Timer::SetAndEnable(kTemp, Display::Update, 10);

    uint timeStart = TIME_MS;

    CPU::FDrive::Init();
    
    bool update = CPU::FDrive::Update();
    
    uint time = TIME_MS - timeStart;

    while (TIME_MS - timeStart < TIME_WAIT && !CPU::FDrive::Update())
    {
        update = CPU::FDrive::Update();
        time = TIME_MS - timeStart;
    }

    if ((ms->drive.connection && ms->drive.active == 0) ||  // Если флеша подключена, но в активное состояние почему-то не перешла
        (ms->drive.active && ms->state != State_Mount))     // или перешла в активное состояние, по почему-то не запустился процесс монтирования
    {
        free(ms);
        NVIC_SystemReset();
    }

    if (ms->state == State_Mount)                           // Это означает, что диск удачно примонтирован //-V774
    {
        if (CPU::FDrive::FileExist(FILE_NAME))                    // Если на диске обнаружена прошивка
        {
            Upgrade();
        }
        else
        {
            ms->state = State_NotFile;
        }
    }
    else if (ms->state == State_WrongFlash) // Диск не удалось примонтировать //-V774
    {
        Timer::PauseOnTime(5000);
    }
    else
    {
        // здесь ничего
    }

    ms->state = State_Ok; //-V774 //-V519
    
    Timer::Disable(kTemp);

    while (Display::IsRun())
    {
    }

    free(ms);

    __disable_irq();
    // Теперь переходим на основную программу
    pFunction JumpToApplication;
    JumpToApplication = (pFunction)(*(__IO uint *)(MAIN_PROGRAM_START_ADDRESS + 4));
    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS);
    __enable_irq();
    JumpToApplication();

    return 0;
}


//----------------------------------------------------------------------------------------------------------------------------------------------------
void Upgrade()
{
#define sizeSector (1 * 1024)
    
    uint8 buffer[sizeSector];
    
    CPU::FLASH_::Prepare();
    
    int size = CPU::FDrive::OpenFileForRead(FILE_NAME);
    int fullSize = size;
    uint address = CPU::FLASH_::ADDR_SECTOR_PROGRAM_0;

    while (size)
    {
        int readedBytes = CPU::FDrive::ReadFromFile(sizeSector, buffer);
        CPU::FLASH_::WriteData(address, buffer, readedBytes);
        size -= readedBytes;
        address += (uint)readedBytes;

        ms->percentUpdate = 1.0F - (float)size / fullSize;
    }
    
    CPU::FDrive::CloseOpenedFile();
}
