#pragma once
#include "Data/DataSettings.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
    operator uint8() const { return (uint8)value; };
};

 /// Режим работы.
struct ModeWork
{
    enum E
    {
        Dir,           ///< Основной режим.
        RAM,           ///< В этом режиме можно просмотреть последние сохранённые измерения.
        ROM,           ///< В этом режиме можно сохранять во flash-памяти измерения просматривать ранее сохранённые.
        None           ///< Используется в модуле Data.c. Нужен, чтобы указать, что мудуль не настроен ни на какой режим.
    } value;
    ModeWork(E v = Dir) : value(v) {};
    operator uint8() const { return (uint8)value; };
};

/// Число точек сигнала, с которым идёт работа.
struct ENumPointsFPGA
{
    enum E
    {
        _512,
        _1k,
        _2k,
        _4k,
        _8k,
        Size
    } value;
    ENumPointsFPGA(E v = _512) : value(v) {};
    operator uint8() const { return (uint8)value; };
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
    operator uint8() const { return (uint8)value; };
};

/// Как сохранять данные на флешку.
struct ModeSaveSignal
{
    enum E
    {
        BMP,
        TXT
    } value;
    operator uint8() const { return (uint8)value; };
};

/// Режим наименования файлов.
struct FileNamingMode
{
    enum E
    {
        Mask,        ///< Именовать по маске.
        Manually     ///< Именовать вручную.
    } value;
    operator uint8() const { return (uint8)value; };
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
#define MODE_WORK_IS_ROM        (MODE_WORK == ModeWork::ROM)


#define SHOW_IN_INT             (set.mem_modeShowIntMem)
#define SHOW_IN_INT_SAVED       (SHOW_IN_INT == ModeShowIntMem::Saved)
#define SHOW_IN_INT_DIRECT      (SHOW_IN_INT == ModeShowIntMem::Direct)
#define SHOW_IN_INT_BOTH        (SHOW_IN_INT == ModeShowIntMem::Both)

#define FPGA_ENUM_POINTS        (set.mem_enumPoints)
#define FPGA_NUM_POINTS         (1 << (FPGA_ENUM_POINTS + 9))
#define FPGA_POINTS_512         (FPGA_ENUM_POINTS == ENumPointsFPGA::_512)
#define FPGA_POINTS_8k          (FPGA_ENUM_POINTS == ENumPointsFPGA::_8k)
#define FPGA_POINTS_16k         (FPGA_ENUM_POINTS == ENumPointsFPGA::_16k)
#define FPGA_POINTS_32k         (FPGA_ENUM_POINTS == ENumPointsFPGA::_32k)

#define MODE_BTN_MEMORY         (set.mem_modeBtnMemory)
#define MODE_BTN_MEMORY_IS_MENU (MODE_BTN_MEMORY == ModeBtnMemory::Menu)
#define MODE_BTN_MEMORY_IS_SAVE (MODE_BTN_MEMORY == ModeBtnMemory::Save)

#define MODE_SAVE               (set.mem_modeSaveSignal)
#define MODE_SAVE_BMP           (MODE_SAVE == ModeSaveSignal::BMP)
#define MODE_SAVE_TXT           (MODE_SAVE == ModeSaveSignal::TXT)

#define MEM_DATA_SCALE          (set.mem_DataScale)
#define MEM_DATA_SCALE_RECALC   (MEM_DATA_SCALE == MemDataScale_Recalculated)

#define NUM_BYTES_SET           sMemory_NumBytesInChannel_()
#define SET_POINTS_IN_CHANNEL   sMemory_NumPointsInChannel_()

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

int sMemory_NumPointsInChannel_();
int sMemory_NumBytesInChannel_();

ENumPointsFPGA NumPoints_2_ENumPoints(int numPoints);
int ENumPoints_2_NumPoints(ENumPointsFPGA numPoints);

/// Выделяет память из кучи для канала. Настройки длины памяти берёт из ds или set (если ds == 0)
void *AllocMemForChannelFromHeap(Chan ch, DataSettings *ds);


/** @}  @}
 */
