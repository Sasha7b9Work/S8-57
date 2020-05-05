#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageTime.h"


// Преобразует перечисление в количество точек
#define ENUM_TO_REL_POINTS(enum)    ((int)(1 << (static_cast<int>(enum) + 9)))


// Режим работы.
struct ModeWork
{
    enum E
    {
        Dir,        // Основной режим.
        RAM,        // В этом режиме можно просмотреть последние сохранённые измерения.
        ROM,        // В этом режиме можно сохранять во flash-памяти измерения просматривать ранее сохранённые.
        Count       // Используется в модуле Data.c. Нужен, чтобы указать, что мудуль не настроен ни на какой режим.
    };
};

// Что делать при нажатии кнопки ПАМЯТЬ.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     // Будет открывааться соответствующая страница меню.
        Save      // Сохранение содержимого экрана на флешку.
    };
};

// Как сохранять данные на флешку.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
    };
};

// Что показывать в режиме Внутр ЗУ - считанный или записанный сигнал.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  // Показывать данные реального времени.
        Saved,   // Показывать сохранённые данные.
        Both     // Показывать оба сигнала.
    };
};

// Режим наименования файлов.
struct FileNamingMode
{
    enum E
    {
        Mask,        // Именовать по маске.
        Manually     // Именовать вручную.
    };
};

// Число точек сигнала, с которым идёт работа.
struct ENumPointsFPGA
{
    enum E
    {
        _512,
        _1k,
        _2k,
        _4k,
        _8k,
        Count
    };
    static int BytesInChannel();
    static int PointsInChannel();
};


// Какой сигнал показывать в режиме ПАМЯТЬ - ВНЕШН ЗУ
struct TypeSignalROM
{
    enum E
    {
        Recorded,
        Current,
        Both,
        Count
    };
};

#define MAX_SYMBOLS_IN_FILE_NAME 35

struct SettingsMemory
{
    ENumPointsFPGA::E   enumPoints;                             // Число точек
    ModeBtnMemory::E    modeBtnMemory;
    TypeSignalROM::E    typeSignalROM;                          // Какой сигнал выводить в режиме ПАМЯТЬ-ВНУТР ЗУ
    ModeWork::E         modeWork;                               // Режим работы.
    bool                flashAutoConnect;                       // Если true, при подключении флешки автоматически выводится Файл-Менеджер.
    int8                indexCurSymbolNameMask;                 // Индекс текущего символа в режиме задания маски или выбора имени.
    ModeSaveSignal::E   modeSaveSignal;                         // В каком виде сохранять сигнал.
    char                fileName[MAX_SYMBOLS_IN_FILE_NAME];     // Имя файла для режима ручного задания.
    ModeShowIntMem::E   _;                                      // Что показывать в режиме ВНУТР ЗУ - считанный или записанный сигнал.
    FileNamingMode::E   fileNamingMode;                         // Режим именования файлов.
    char                fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // \brief Здесь маска для автоматического именования файлов.
                        // \details Правила именования.\n
                        // \code
                        // %y('\x42') - год, %m('\x43') - месяц, %d('\x44') - день, %H('\x45') - часы, %M('\x46') - минуты, %S('\x47') - секунды
                        // %Nn('\x48''n') - порядковый номер, котрый занимает не менее n знакомест, например, 7 в %3N будет преобразовано в 007
                        // Примеры
                        // name_%4N_%y_%m_%d_%H_%M_%S будет генерировать файлы вида name_0043_2014_04_25_14_45_32
                        // При этом обратите внимание, что если спецификатор %4N стоИт после временнЫх параметров, то, скорее всего, этот параметр 
                        // будет всегда равен 0001, т.к. для определения номера просматриваются.
                        // \endcode
};


struct PageMemory
{
    static void OnChanged_Points(bool active);

    static void OnOpenClose_Drive_Manager(bool enter = true);

    static void SaveSignalToFlashDrive();

    static const Page * const self;
};



struct PageRAM
{
    static const Page * const self;
};



struct PageROM
{
    static const Page * const self;
};



struct PageDrive
{
    static const Page * const self;


    struct Manager
    {
        static const Page * const self;
    };


    struct Mask
    {
        static const Page * const self;
    };

    struct SetName
    {
        static const Page *const self;
    };
};
