#pragma once

#define DEBUG

#define LANG_RU true

#define VERSION "54-1.0"

#define MATH_FUNC_IS_SUM false
#define MATH_FUNC_IS_MUL false

#ifdef WIN32
#define __attribute(x) //-V2573
#define __attribute__(x) //-V2573
#define WCHAR unsigned short
#else
#define nullptr 0 //-V1059
#endif


typedef signed   char        int8;
typedef unsigned char        uint8;
typedef unsigned char        uchar;
typedef signed   short int   int16;
typedef unsigned short int   uint16;
typedef unsigned int         uint;
typedef const    char *const pString;
typedef uint16               col_val;

typedef void  (*pFuncVV)();
typedef bool  (*pFuncBV)();

#define _GET_BIT(value, bit)                    (((value) >> (bit)) & 0x01) //-V2573
#define _SET_BIT(value, bit)                    ((value) |= (1 << (bit))) //-V2573
#define _CLEAR_BIT(value, bit)                  ((value) &= (~(1 << (bit)))) //-V2573
#define _SET_BIT_VALUE(value, numBit, bitValue) (value |= ((bitValue) << (numBit))) //-V2573

// Для определения задания буферов лучше пользоваться этими макросами, чтобы потом легче было отследить, где можно памяти освободить
#define CHAR_BUF(name, size)            char name[size]
#define CHAR_BUF2(name, size1, size2)   char name[size1][size2]

#define NOT_USED 0

#define ERROR_VALUE_FLOAT   1.111e29f
#define ERROR_VALUE_UINT8   255
#define ERROR_STRING_VALUE  "--.--"

#include <stm32f4xx_hal.h>
