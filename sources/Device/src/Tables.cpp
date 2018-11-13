#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Font/Font.h"
#include "Display/Painter.h"
#include "Tables.h"
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/// Таблица символов алфавита и спецсимволов для ввода текста
pString Tables::symbolsAlphaBet[0x48] =
{
    /* 0x00 */ "A", "B", "C", "D", "E", "F", "G", "H", "I", "J", "K", "L", "M", "N", "O", "P", "Q", "R", "S", "T", "U", "V", "W", "X", "Y", "Z", /* 0x19 */
    /* 0x1a */ " ", "_", "-", "0", "1", "2", "3", "4", "5", "6", "7", "8", "9", /* 0x26 */
    /* 0x27 */ "a", "b", "c", "d", "e", "f", "g", "h", "i", "j", "k", "l", "m", "n", "o", "p", "q", "r", "s", "t", "u", "v", "w", "x", "y", "z", /* 0x40 */
    //         0x41  0x42  0x43  0x44  0x45  0x46   0x47   - это порядковый номер элемента в этом массиве
    /* 0x41 */ "%y", "%m", "%d", "%H", "%M", "%S", "%N" /* 0x47 */
    //         0x01  0x02  0x03  0x04  0x05  0x06   0x07   - под этими значениями элементы хранятся в set.memory.fileNameMask
};

///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Display::ENumSignalsInSec Tables::ENumSignalsInSecToENUM(int numSignalsInSec)
{
    if(numSignalsInSec == 1)
    {
        return Display::ENumSignalsInSec::_1;
    }
    else if(numSignalsInSec == 2)
    {
        return Display::ENumSignalsInSec::_2;
    }
    else if(numSignalsInSec == 5)
    {
        return Display::ENumSignalsInSec::_5;
    }
    else if(numSignalsInSec == 10)
    {
        return Display::ENumSignalsInSec::_10;
    }
    else if(numSignalsInSec == 25)
    {
        return Display::ENumSignalsInSec::_25;
    }
    return Display::ENumSignalsInSec::_1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Tables_RangeNameFromValue(Range range)
{
    static struct StructName
    {
        pString name;
        StructName(pString n) : name(n) {};
    } names[Range::Number] =
    {
        StructName("Range_2mV"),
        StructName("Range_5mV"),
        StructName("Range_10mV"),
        StructName("Range_20mV"),
        StructName("Range_50mV"),
        StructName("Range_100mV"),
        StructName("Range_200mV"),
        StructName("Range_500mV"),
        StructName("Range_1V"),
        StructName("Range_2V"),
        StructName("Range_5V"),
        StructName("Range_10V"),
        StructName("Range_20V")
    };
    return names[range].name;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Tables::DrawStr(int index, int x, int y)
{
    const char *str = symbolsAlphaBet[index];
    if (index == INDEX_SYMBOL)
    {
        Painter::FillRegion(x - 1, y, Font::GetLengthText(str), 9, Color::FLASH_10);
    }
    Painter::DrawText(x, y, symbolsAlphaBet[index], index == INDEX_SYMBOL ? Color::FLASH_01 : Color::FILL);
}
