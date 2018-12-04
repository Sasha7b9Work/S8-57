#pragma once

#define DEBUG

#ifdef WIN32
#define __attribute(x)
#endif

#define MODEL_RU "Осциллограф-мультиметр"
#define MODEL_EN "Oscilloscope-multimeter"
#define NUM_VER "0.0.1"


typedef const char  *const  pString;
typedef unsigned char       uchar;
typedef signed char         int8;
typedef unsigned char       uint8;
typedef signed short int    int16;
typedef unsigned short int  uint16;
typedef unsigned int        uint;
typedef unsigned long long  uint64;


#ifdef WIN32
typedef unsigned char  BYTE;
typedef unsigned short WORD;
#define WCHAR unsigned short
typedef unsigned long  DWORD;
typedef unsigned long  FSIZE_t;
typedef unsigned int   UINT;
typedef unsigned char  uint8_t;
typedef unsigned short uint16_t;
typedef unsigned int   uint32_t;

#ifndef _STM32
#define TCHAR unsigned char
#endif

#define __IO
#endif


typedef void (*pFuncVV)();
typedef void (*pFuncVB)(bool);
typedef bool (*pFuncBV)();
typedef void (*pFuncVI)(int);
typedef void (*pFuncVII)(int, int);
typedef void (*pFuncVpIII)(int *, int, int);
typedef void (*pFuncVpI8I8I8)(int8 *, int8, int8);
typedef bool (*pFuncBI)(int);
typedef void (*pFuncVU8)(uint8);

inline void EmptyFuncVV(){}
inline void EmptyFuncVB(bool){}
inline bool EmptyFuncBtV() { return true; }
inline bool EmptyFuncBfV() { return false; }
inline void EmptyFuncVI(int) {}
inline void EmptyFuncVII(int, int) {}
inline bool EmptyFuncBfI(int) { return false; }
inline bool EmptyFuncBtU8(uint8) { return true; }

#define ERROR_VALUE_INT     INT_MAX
#define ERROR_VALUE_INT16   SHRT_MAX
    

#ifndef _WIN32
#if __ARMCLIB_VERSION < 6070001
#pragma anon_unions
#endif
#endif


#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= (bitValue << numBit))
#define _GET_BIT(value, numBit) ((value >> numBit) & 1)
#define _CLEAR_BIT(value, bit) ((value) &= (~(1 << bit)))
#define _SET_BIT(value, bit) ((value) |= (1 << (bit)))


#define DEF_STRUCT(name, type)  \
static const struct name        \
{                               \
    type val;                   \
    name(type v) : val(v) {};   \
}


#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)

#define ERROR_VALUE_FLOAT   std::numeric_limits<float>::quiet_NaN()
#define ERROR_STRING_VALUE  "--.--"
#define ERROR_VALUE_UINT8   255

#define MAX_UINT 0xffffffff


// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]


#define ENABLE_RU "Вкл"
#define ENABLE_EN "On"

#define DISABLE_RU "Откл"
#define DISABLE_EN "Off"

#define SAFE_FREE(x) if(x) free(x); (x) = 0;


union BitSet16
{
    uint16 halfWord;
    uint8  byte[2];
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
    BitSet16(uint16 v) : halfWord(v) {};
};

union BitSet32
{
    uint    word;
    uint16  halfWord[2];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
    };
    uint8   byte[4];
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
    };

    BitSet32(uint w = 0) : word(w) {}

    BitSet32(uint8 _byte3, uint8 _byte2, uint8 _byte1, uint8 _byte0) : byte0(_byte0), byte1(_byte1), byte2(_byte2), byte3(_byte3)
    {
    }

    void Set(uint8 _byte3, uint8 _byte2, uint8 _byte1, uint8 _byte0)
    {
        byte[0] = _byte0;
        byte[1] = _byte1;
        byte[2] = _byte2;
        byte[3] = _byte3;
    }

    void Set(uint _word)
    {
        word = _word;
    }
};

union BitSet64
{
    uint64 dword;
    uint   word[2];
    uint16 halfWord[4];
    uint8  byte[8];
    struct
    {
        int sword0;
        int sword1;
    };
    struct
    {
        uint word0;
        uint word1;
    };
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
        uint16 halfWord2;
        uint16 halfWord3;
    };
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
        uint8 byte4;
        uint8 byte5;
        uint8 byte6;
        uint8 byte7;
    };
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RETURN_TO_MAIN_MENU     0
#define RETURN_TO_LAST_MEM      1
#define RETURN_TO_INT_MEM       2
#define RETURN_TO_DISABLE_MENU  3


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этом при нажатии на кнопки вместо выполнения её функции выводится информация о её назначении
#define HINT_MODE_ENABLED       (gBF.showHelpHints)
#define NEED_FINISH_DRAW        (gBF.needFinishDraw)
#define SHOW_DEBUG_MENU         (gBF.showDebugMenu)
#define NEED_SAVE_TO_FLASHDRIVE (gBF.needForSaveToFlashDrive)
#define EXIT_FROM_SETNAME_TO    (gBF.exitFromModeSetNameTo)
#define SHOW_STRING_NAVIGATION  (gBF.temporaryShowStrNavi)
#define NUM_ROM_SIGNAL          (gBF.currentNumROMSignal)
#define RUN_FPGA_BEFORE_SB      (gBF.runningFPGAbeforeDrawButtons)
#define NUM_RAM_SIGNAL          (gBF.currentNumRAMSignal)
#define EXIT_FROM_ROM_TO_RAM    (gBF.exitFromROMtoRAM)
#define FPGA_NEED_AUTO_FIND     (gBF.FPGAneedAutoFind)
#define ALWAYS_SHOW_ROM_SIGNAL  (gBF.alwaysShowROMSignal)
#define PANEL_CONTROL_RECEIVE   (gBF.panelControlReceive)
#define FPGA_IN_PROCESS_OF_READ (gBF.FPGAinProcessingOfRead)
/// Если 1, то консоль находится в режиме паузы
#define CONSOLE_IN_PAUSE        (gBF.consoleInPause)

#define FM_NEED_REDRAW          (gBF.needRedrawFileManager)
#define FM_REDRAW_FULL          1
#define FM_REDRAW_FOLDERS       2
#define FM_REDRAW_FILES         3


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct BitField
{
    int16 currentNumRAMSignal;              ///< Текущий номер последнего сигнала в режиме ПАМЯТЬ - Последние.
    int8 currentNumROMSignal;               ///< Текущий номер сигнала, сохранённого в ППЗУ.
    uint showHelpHints : 1;                 ///< Если 1, то при нажатии кнопки вместо выполнения её фунции выводится подсказка о её назначении.
    uint needFinishDraw : 1;                ///< Если 1, то нужно немедленно завершить отрисовку и вывести на экран то, что уже нарисовано.
    uint showDebugMenu : 1;
    uint needForSaveToFlashDrive : 1;       ///< Если 1, то нужно сохранить после отрисовки на флешку.
    uint exitFromModeSetNameTo : 2;         ///< \brief Куда возвращаться из окна установки имени при сохранении : 0 - в основное меню, 1 - 
                                            ///< в окно последних, 2 - в окно Внутр ЗУ, 3 - в основно окно в выключенным меню.
    uint temporaryShowStrNavi : 1;          ///< Признак того, что нужно временно показывать строку навигации меню.
    uint runningFPGAbeforeDrawButtons : 1; ///< Здесь сохраняется информация о том, работала ли ПЛИС перед переходом в режим работы с памятью.
    uint exitFromROMtoRAM : 1;              ///< Если 1, то выходить из страницы внутренней памяти нужно не стандартно, а в меню последних.
    uint FPGAneedAutoFind : 1;              ///< Если 1, то нужно найти сигнал.
    uint needRedrawFileManager : 2;         ///< @brief Если 1, то файл-менеджер нуждается в полной перерисовке.
                                            ///< Если 2, то перерисовать только каталоги.
                                            ///< Если 3, то перерисовать только файлы.
    uint alwaysShowROMSignal : 1;           ///< Если 1, то показывать всегда выбранный в режиме "Внутр. ЗУ" сигнал.
    uint panelControlReceive : 1;           ///< Если 1, то панель прислала команду.
    uint FPGAinProcessingOfRead : 1;
    uint consoleInPause : 1;                ///< Если 1, то консоль находится в режиме паузы.
};

extern volatile BitField gBF;   ///< @brief Структура сделана volatile, потому что иначе при вклюённой оптимизации зависает во время выключения. 
                                ///< Вероятно, это связано с переменной soundIsBeep (перед стиранием сектора в цикле происходит ожидание, когда эта 
                                ///< переменная изменит своё состояние (каковое изменение происходит из прерывания, ясен перец)).

enum StateCalibration
{
    StateCalibration_None,
    StateCalibration_ADCinProgress,
    StateCalibration_RShiftAstart,
    StateCalibration_RShiftAinProgress,
    StateCalibration_RShiftBstart,
    StateCalibration_RShiftBinProgress,
    StateCalibration_ErrorCalibrationA,
    StateCalibration_ErrorCalibrationB
};

enum StateWorkFPGA
{
    StateWorkFPGA_Stop,     ///< СТОП - не занимается считыванием информации.
    StateWorkFPGA_Wait,     ///< Ждёт поступления синхроимпульса.
    StateWorkFPGA_Work,     ///< Идёт работа.
    StateWorkFPGA_Pause     ///< Это состояние, когда временно приостановлен прибор, например, для чтения данных или для записи значений регистров.
};


#define SAFE_HANDLER(handler)                       \
    if(handler) { handler(); }                      \
    else { LOG_WRITE("Отсутствует обработчик"); }

#define HANDLER_CHOICE_AND_SAFE_RUN(type, index)    \
    type func = funcs[index].val;                   \
    SAFE_HANDLER(func)

/// Защищённый обработчик с тремя параметрами. Защищённый - потому что в случае его отсутствия выводится сообщение об ошибке
#define SAFE_HANDLER_3(handler, param1, param2, param3)     \
    if(handler) { handler(param1, param2, param3); }        \
    else { LOG_WRITE("Отсутствует обработчик");  }

/// Выбрать обработчик и защищённо его выполнить
#define HANDLER_CHOICE_AND_SAFE_RUN_3(type, index, param1, param2, param3)  \
    type func = funcs[index].val;                                           \
    SAFE_HANDLER_3(func, param1, param2, param3)


#include "log.h"
