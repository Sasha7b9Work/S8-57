#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"


#define SHOW_IN_INT             (set.mem.modeShowIntMem)
#define SHOW_IN_INT_SAVED       (SHOW_IN_INT == ModeShowIntMem::Saved)
#define SHOW_IN_INT_DIRECT      (SHOW_IN_INT == ModeShowIntMem::Direct)
#define SHOW_IN_INT_BOTH        (SHOW_IN_INT == ModeShowIntMem::Both)

/// Преобразует перечисление в количество точек
#define ENUM_TO_REL_POINTS(enum) ((uint)(1 << ((enum) + 9)))

#define FPGA_ENUM_POINTS        (set.mem.enumPoints)
#define FPGA_POINTS_512         (FPGA_ENUM_POINTS == ENumPointsFPGA::_512)
#define FPGA_POINTS_8k          (FPGA_ENUM_POINTS == ENumPointsFPGA::_8k)
#define FPGA_NUM_POINTS         ENUM_TO_REL_POINTS(FPGA_ENUM_POINTS)

#define MODE_BTN_MEMORY         (set.mem.modeBtnMemory)
#define MODE_BTN_MEMORY_IS_MENU (MODE_BTN_MEMORY == ModeBtnMemory::Menu)
#define MODE_BTN_MEMORY_IS_SAVE (MODE_BTN_MEMORY == ModeBtnMemory::Save)

#define MODE_SAVE               (set.mem.modeSaveSignal)
#define MODE_SAVE_BMP           (MODE_SAVE == ModeSaveSignal::BMP)
#define MODE_SAVE_TXT           (MODE_SAVE == ModeSaveSignal::TXT)

#define MEM_DATA_SCALE          (set.mem.DataScale)
#define MEM_DATA_SCALE_RECALC   (MEM_DATA_SCALE == MemDataScale_Recalculated)


/// Режим работы.
struct ModeWork
{
    enum E
    {
        Dir,        ///< Основной режим.
        RAM,        ///< В этом режиме можно просмотреть последние сохранённые измерения.
        ROM,        ///< В этом режиме можно сохранять во flash-памяти измерения просматривать ранее сохранённые.
        Number      ///< Используется в модуле Data.c. Нужен, чтобы указать, что мудуль не настроен ни на какой режим.
    } value;
    explicit ModeWork(E v) : value(v) {};
};

/// Что делать при нажатии кнопки ПАМЯТЬ.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     ///< Будет открывааться соответствующая страница меню.
        Save      ///< Сохранение содержимого экрана на флешку.
    } value;
};

/// Как сохранять данные на флешку.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
    } value;
};

/// Что показывать в режиме Внутр ЗУ - считанный или записанный сигнал.
struct ModeShowIntMem
{
    enum E
    {
        Direct,  ///< Показывать данные реального времени.
        Saved,   ///< Показывать сохранённые данные.
        Both     ///< Показывать оба сигнала.
    } value;
};

/// Режим наименования файлов.
struct FileNamingMode
{
    enum E
    {
        Mask,        ///< Именовать по маске.
        Manually     ///< Именовать вручную.
    } value;
};


struct SettingsMemory
{
#define MAX_SYMBOLS_IN_FILE_NAME 35
    Osci::Settings::Memory::ENumPointsFPGA::E   enumPoints;                             ///< Число точек
    ModeBtnMemory::E                            modeBtnMemory;
    ModeWork::E                                 modeWork;                               ///< Режим работы.
    bool                                        flashAutoConnect;                       ///< Если true, при подключении флешки автоматически выводится Файл-Менеджер.
    int8                                        indexCurSymbolNameMask;                 ///< Индекс текущего символа в режиме задания маски или выбора имени.
    ModeSaveSignal::E                           modeSaveSignal;                         ///< В каком виде сохранять сигнал.
    char                                        fileName[MAX_SYMBOLS_IN_FILE_NAME];     ///< Имя файла для режима ручного задания.
    ModeShowIntMem::E                           modeShowIntMem;                         ///< Что показывать в режиме ВНУТР ЗУ - считанный или записанный сигнал.
    FileNamingMode::E                           fileNamingMode;                         ///< Режим именования файлов.
    char                                        fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; ///< \brief Здесь маска для автоматического именования файлов.
                        ///< \details Правила именования.\n
                        /// \code
                        /// %y('\x42') - год, %m('\x43') - месяц, %d('\x44') - день, %H('\x45') - часы, %M('\x46') - минуты, %S('\x47') - секунды
                        /// %Nn('\x48''n') - порядковый номер, котрый занимает не менее n знакомест, например, 7 в %3N будет преобразовано в 007
                        /// Примеры
                        /// name_%4N_%y_%m_%d_%H_%M_%S будет генерировать файлы вида name_0043_2014_04_25_14_45_32
                        /// При этом обратите внимание, что если спецификатор %4N стоИт после временнЫх параметров, то, скорее всего, этот параметр 
                        /// будет всегда равен 0001, т.к. для определения номера просматриваются.
                        /// \endcode
};

Osci::Settings::Memory::ENumPointsFPGA::E NumPoints_2_ENumPoints(int numPoints);
int ENumPoints_2_NumPoints(Osci::Settings::Memory::ENumPointsFPGA::E numPoints);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageMemory
{
    static void OnChanged_Points(bool active);

    static void OnOpenClose_Drive_Manager(bool enter = true);

    static void SaveSignalToFlashDrive();

    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageRAM
{
    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageROM
{
    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageDrive
{
    static const Page * const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageManager
    {
        static const Page * const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageMask
    {
        static const Page * const self;
    };
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageSetName
{
    static const Page * const self;
};
