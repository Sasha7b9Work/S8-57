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

#include "Display/Display.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/CPU.h"
#include "Hardware/Timer.h"


#define MAIN_PROGRAM_START_ADDRESS  0x8020000


typedef void(*pFunction)();


int main()
{
    CPU::Init();

    Timer::PauseOnTime(250);
    
    Display::Init();
   
    //Timer::SetAndEnable(kTemp, Display::Update, 10);

    FDrive::Init();

    FDrive::AttemptUpdate();
    
    Timer::Disable(kTemp);

    //while (Display::IsRun())
    //{
    //}
    
    CPU::DeInit();

    __disable_irq();
    // Теперь переходим на основную программу
    pFunction JumpToApplication;
    JumpToApplication = reinterpret_cast<pFunction>(*reinterpret_cast<__IO uint *>(MAIN_PROGRAM_START_ADDRESS + 4)); //-V2571
    __set_MSP(*(__IO uint *)MAIN_PROGRAM_START_ADDRESS);
    __enable_irq();
    JumpToApplication();

    return 0;
}
