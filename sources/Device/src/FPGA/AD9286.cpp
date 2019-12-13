#include "defines.h"
#include "AD9286.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL_PIO.h"


static void  WriteByte(uint8 address, uint8 byte);


void AD9286::Init()
{
    /*
        Для чипселекта используется вывод
        А вот для тактов и данных используются те же, выводы, которые в дальнейшем управляют микросхемой EEPROM AT25160

        CS  - 64 - PE11
        DAT - 29 - PC3
        SCK - 69 - PB10
    */
   
    HAL_PIO::Init(PORT_AD9286_CS, HMode::Output_PP, HPull::Up);       // Инициализация CS

    HAL_PIO::Init(PORT_AD9286_DAT, HMode::Output_PP, HPull::Up);     // Инициализация DAT
    
    HAL_PIO::Init(PORT_AD9286_SCK, HMode::Output_PP, HPull::Up);     // Инициализация SCK

    HAL_PIO::Set(PORT_AD9286_CS);
    HAL_PIO::Reset(PORT_AD9286_DAT);
    HAL_PIO::Reset(PORT_AD9286_SCK);

    Tune();
}


void AD9286::Tune()
{
    WriteByte(0x09, 0x09);
    /// \todo Это должно быть в функции записи
    WriteByte(0xff, 0x01);   // Пишем бит подтверждения.
}


static void WriteByte(uint8 address, uint8 byte)
{
    HAL_PIO::Reset(PORT_AD9286_CS);

    uint value = static_cast<uint>((address << 8) + byte);

    for (int i = 23; i >= 0; --i)
    {
        if (_GET_BIT(value, i))
        {
            HAL_PIO::Set(PORT_AD9286_DAT);
        }
        else
        {
            HAL_PIO::Reset(PORT_AD9286_DAT);
        }
        PAUSE_ON_TICKS(100);

        HAL_PIO::Set(PORT_AD9286_SCK);
        PAUSE_ON_TICKS(100);

        HAL_PIO::Reset(PORT_AD9286_SCK);
    }

    HAL_PIO::Set(PORT_AD9286_CS);
}
