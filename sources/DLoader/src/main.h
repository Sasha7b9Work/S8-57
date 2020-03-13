#pragma once
#include "defines.h"
#include <ff.h>

// Key configuration
#define KEY_START_ADDRESS   (uint)0x0
#define KEY_PAGE_NUMBER     20
#define KEY_VALUE           0xAAAA5555

// Flash configuration
#define MAIN_PROGRAM_START_ADDRESS  (uint)0x8020000
#define MAIN_PROGRAM_PAGE_NUMBER    21
#define NUM_OF_PAGES                256
#define FLASH_PAGE_SIZE             2048

#define TIME_WAIT   5000    // Время работы заставки


struct State
{
    enum E
    {
        Start,
        Mount,            // Монтирование флешки
        WrongFlash,       // Флешка есть, но прочитать нельзя
        RequestAction,    // Что делать - апгрейдить или нет
        NotFile,          // Если диск примонтирован, но обновления на нём нету
        Upgrade,          // Процесс апгрейда
        Ok                // Обновление удачно завершено
    };
};

typedef enum
{
    StateDisk_Idle,
    StateDisk_Start
} StateDisk;

typedef struct
{
    bool    isRun;
    uint8   notUsed[3];
    float   value;
    float   direction;
    uint    timePrev;
} DisplayStr;

typedef struct
{
    FATFS       USBDISKFatFS;
    char        USBDISKPath[4];
    StateDisk   state;
    uint8       notUsed[3];
    FIL         file;
    int         connection;
    int         active;
} FlashDrive;

typedef struct
{
    FlashDrive  drive;
    DisplayStr  display;
    float       percentUpdate;
    State::E    state;
    uint8       notUsed[3];
} MainStruct;


// Данная структура используется во всех модулях программы для уменьшения расхода ОЗУ
// Память для деё должна быть выделена с помощью malloc в начале программы и возвращена в момент перехода на основную программу
extern MainStruct *ms; //-V707
