#pragma once
#include "Data/DataSettings.h"
#include "Osci/Osci_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsMemory Settings Memory
 *  @{
 */

 /// Что делать при нажатии кнопки ПАМЯТЬ.
struct ModeBtnMemory
{
    enum E
    {
        Menu,     ///< Будет открывааться соответствующая страница меню.
        Save      ///< Сохранение содержимого экрана на флешку.
    } value;
};

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

/// Как сохранять данные на флешку.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
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


#define FILE_NAME_MASK          (set.mem_fileNameMask)
#define FILE_NAME               (set.mem_fileName)
#define INDEX_SYMBOL            (set.mem_indexCurSymbolNameMask)
#define FLASH_AUTOCONNECT       (set.mem_flashAutoConnect)

#define FILE_NAMING_MODE        (set.mem_fileNamingMode)
#define FILE_NAMING_MODE_MASK   (FILE_NAMING_MODE == FileNamingMode::Mask)
#define FILE_NAMING_MODE_MANUAL (FILE_NAMING_MODE == FileNamingMode::Manually)

#define MODE_WORK               (set.mem_modeWork)
#define MODE_WORK_IS_DIR        (MODE_WORK == ModeWork::Dir)
#define MODE_WORK_IS_RAM        (MODE_WORK == ModeWork::RAM)

#define SHOW_IN_INT             (set.mem_modeShowIntMem)
#define SHOW_IN_INT_SAVED       (SHOW_IN_INT == ModeShowIntMem::Saved)
#define SHOW_IN_INT_DIRECT      (SHOW_IN_INT == ModeShowIntMem::Direct)
#define SHOW_IN_INT_BOTH        (SHOW_IN_INT == ModeShowIntMem::Both)

/// Преобразует перечисление в количество точек
#define ENUM_TO_REL_POINTS(enum) ((uint)(1 << ((enum) + 9)))

#define FPGA_ENUM_POINTS        (set.mem_enumPoints)
#define FPGA_POINTS_512         (FPGA_ENUM_POINTS == ENumPointsFPGA::_512)
#define FPGA_POINTS_8k          (FPGA_ENUM_POINTS == ENumPointsFPGA::_8k)
#define FPGA_POINTS_16k         (FPGA_ENUM_POINTS == ENumPointsFPGA::_16k)
#define FPGA_POINTS_32k         (FPGA_ENUM_POINTS == ENumPointsFPGA::_32k)
#define FPGA_NUM_POINTS         ENUM_TO_REL_POINTS(FPGA_ENUM_POINTS)

#define MODE_BTN_MEMORY         (set.mem_modeBtnMemory)
#define MODE_BTN_MEMORY_IS_MENU (MODE_BTN_MEMORY == ModeBtnMemory::Menu)
#define MODE_BTN_MEMORY_IS_SAVE (MODE_BTN_MEMORY == ModeBtnMemory::Save)

#define MODE_SAVE               (set.mem_modeSaveSignal)
#define MODE_SAVE_BMP           (MODE_SAVE == ModeSaveSignal::BMP)
#define MODE_SAVE_TXT           (MODE_SAVE == ModeSaveSignal::TXT)

#define MEM_DATA_SCALE          (set.mem_DataScale)
#define MEM_DATA_SCALE_RECALC   (MEM_DATA_SCALE == MemDataScale_Recalculated)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Osci::Settings::Memory::ENumPointsFPGA::E NumPoints_2_ENumPoints(int numPoints);
int ENumPoints_2_NumPoints(Osci::Settings::Memory::ENumPointsFPGA::E numPoints);


/** @}  @}
 */
