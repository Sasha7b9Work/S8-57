#pragma once
#include "defines.h"


#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL_EEPROM
{
    static void EraseSector(uint startAddress);
    static void WriteBytes(uint address, const uint8 *data, int size);
};


struct HAL
{
    static void ErrorHandler(const char *, int);
};


struct HAL_BUS
{
    static void Init();

    // Конфигурировать для работы по шине FSMC с альтерой и памятью
    static void ConfigureToFSMC();

    struct Panel
    {
        static void Send(uint8 byte);
        static void Send(uint8 byte0, uint8 byte1);
        static void Send(uint8 *data, uint size);
        static bool Receive();
        static bool InInteraction();
    };

    struct Mode
    {
        enum E
        {
            FSMC,
            PanelRead,
            PanelWrite
        };
    };

    static Mode::E mode;

private:

    static void InitPanel();

    // Настроить FSMC для работы с внешней RAM
    static void InitRAM();
};
