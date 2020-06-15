#pragma once
#include "Menu/MenuItems.h"
#include "Menu/Pages/Include/PageTime.h"


// Преобразует перечисление в количество точек
#define ENUM_TO_REL_POINTS(enum)    ((int)(1 << (static_cast<int>(enum) + 9)))


// Режим работы.
struct ModeWork
{
#define S_MEM_MODE_WORK         (set.mem._modeWork)
#define S_MEM_MODE_WORK_IS_DIR  (S_MEM_MODE_WORK == ModeWork::Dir)
#define S_MEM_MODE_WORK_IS_RAM  (S_MEM_MODE_WORK == ModeWork::RAM)
#define S_MEM_MODE_WORK_IS_ROM  (S_MEM_MODE_WORK == ModeWork::ROM)

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
#define S_MEM_MODE_BTN_MEMORY           (set.mem._modeBtnMemory)
#define S_MEM_MODE_BTN_MEMORY_IS_SAVE   (S_MEM_MODE_BTN_MEMORY == ModeBtnMemory::Save)

    enum E
    {
        Menu,     // Будет открывааться соответствующая страница меню.
        Save      // Сохранение содержимого экрана на флешку.
    };
};


// Как сохранять данные на флешку.
struct ModeSaveSignal
{
#define S_MEM_MODE_SAVE_SIGNAL          (set.mem._modeSaveSignal)
#define S_MEM_MODE_SAVE_SIGNAL_IS_BMP   (S_MEM_MODE_SAVE_SIGNAL == ModeSaveSignal::BMP)

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
#define S_MEM_FILE_NAMING_MODE              (set.mem._fileNamingMode)
#define S_MEM_FILE_NAMING_MODE_IS_MASK      (S_MEM_FILE_NAMING_MODE == FileNamingMode::Mask)
#define S_MEM_FILE_NAMING_MODE_IS_MANUALLY  (S_MEM_FILE_NAMING_MODE == FileNamingMode::Manually)

    enum E
    {
        Mask,        // Именовать по маске.
        Manually     // Именовать вручную.
    };
};


// Число точек сигнала, с которым идёт работа.
struct ENumPointsFPGA
{
#define S_ENUMPOINTS_FPGA   (set.mem._enumPoints)

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
#define S_MEM_TYPE_SIGNAL_ROM               (set.mem._typeSignalROM)
#define S_MEM_TYPE_SIGNAL_ROM_IS_CURRENT    (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Current)
#define S_MEM_TYPE_SIGNAL_ROM_IS_RECORDED   (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Recorded)
#define S_MEM_TYPE_SIGNAL_ROM_IS_BOTH       (S_MEM_TYPE_SIGNAL_ROM == TypeSignalROM::Both)

    enum E
    {
        Recorded,
        Current,
        Both,
        Count
    };
};

#define MAX_SYMBOLS_IN_FILE_NAME 35

#define S_MEM_FILE_NAME             (set.mem._fileName)
#define S_MEM_FILE_NAME_MASK        (set.mem._fileNameMask)
#define S_MEM_FLASH_AUTO_CONNECT    (set.mem._flashAutoConnect)
#define S_MEM_INDEX_CUR_SYMBOL_MASK (set.mem._indexCurSymbolNameMask)

struct SettingsMemory
{
    ENumPointsFPGA::E   _enumPoints;                             // Число точек
    ModeBtnMemory::E    _modeBtnMemory;
    TypeSignalROM::E    _typeSignalROM;                          // Какой сигнал выводить в режиме ПАМЯТЬ-ВНУТР ЗУ
    ModeWork::E         _modeWork;                               // Режим работы.
    bool                _flashAutoConnect;                       // Если true, при подключении флешки автоматически выводится Файл-Менеджер.
    int8                _indexCurSymbolNameMask;                 // Индекс текущего символа в режиме задания маски или выбора имени.
    ModeSaveSignal::E   _modeSaveSignal;                         // В каком виде сохранять сигнал.
    char                _fileName[MAX_SYMBOLS_IN_FILE_NAME];     // Имя файла для режима ручного задания.
    ModeShowIntMem::E   _;                                      // Что показывать в режиме ВНУТР ЗУ - считанный или записанный сигнал.
    FileNamingMode::E   _fileNamingMode;                         // Режим именования файлов.
    char                _fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; // \brief Здесь маска для автоматического именования файлов.
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
