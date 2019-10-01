#include "defines.h"
#include "Command.h"
#include "Transceiver.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"


using HAL::FSMC;
using namespace Transceiver;



const Font *fonts[Font::Type::Size] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
const Font *font = &font8;

const BigFont *bigFont = &fontDigits64;

Font::Type::E pushedFont = Font::Type::_8;
Font::Type::E currentFont = Font::Type::_8;

#ifndef PANEL
static int spacing = 1;
#endif



int Font::GetLengthText(pString text)
{
    int result = 0;
    char *symbol = (char *)text;

    while (*symbol)
    {
        result += GetLengthSymbol(*symbol);
        symbol++;
    }
    return result;
}


int Font::GetHeightSymbol(char)
{
    return 9;
}


int Font::GetLengthSymbol(char symbol)
{
#ifdef WIN32
    if (symbol < 0)
    {
        symbol += 128;                  // Т.к. char имеет знак в некоторых системах
    }
#endif
    return font->symbols[symbol].width + 1;
}


void Font::SetCurrent(Font::Type::E typeFont)
{
    if (typeFont != currentFont)
    {
        switch (typeFont)
        {
        case Type::_5:
            font = &font5;
            break;
        case Type::_8:
            font = &font8;
            break;
        case Type::_UGO:
            font = &fontUGO;
            break;
        case Type::_UGO2:
            font = &fontUGO2;
            break;
        case Type::_Big64:
            bigFont = &fontDigits64;
            break;
        case Type::None:
        case Type::Size:
            break;
        }

#ifndef PANEL
        Transmitter::Send(Command::Paint_SetFont, (uint8)typeFont);
#endif

        pushedFont = currentFont;
        currentFont = typeFont;
    }
}


void Font::Pop()
{
    SetCurrent(pushedFont);
}


#ifdef PANEL
void Font::SetSpacing(int) {}
#else
void Font::SetSpacing(int _spacing)
{
    spacing = _spacing;
    Transmitter::Send(Command::Paint_SetTextSpacing, (uint8)spacing);
}

int Font::GetSpacing()
{
    return spacing;
}
#endif


#ifdef PANEL
void Font::SetMinWidth(uint8) {}
#else
void Font::SetMinWidth(uint8 width)
{
    Transmitter::Send(Command::Paint_SetMinWidthFont, width);
}
#endif


static bool FontIsSmall()
{
    return currentFont <= Font::Type::_UGO2;
}


uint8 Font::GetWidth(uint8 symbol)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].width;
    }

    return bigFont->GetWidth(symbol);
}


uint8 Font::GetHeight()
{
    if (FontIsSmall())
    {
        return (uint8)font->_height;
    }

    return bigFont->height;
}


bool Font::RowNotEmpty(uint8 symbol, int row)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    FullSymbol fullSymbol;

    if (bigFont->GetFullSymbol(fullSymbol, symbol))
    {
        return fullSymbol.RowNotEmpty(row);
    }

    return false;
}


bool FullSymbol::RowNotEmpty(int numRow) const
{
    uint8 *row = GetRow(numRow);

    for (int i = 0; i < BytesInRow(); i++)
    {
        if (row[i] != 0)
        {
            return true;
        }
    }

    return false;
}


bool FullSymbol::BitIsExist(int numRow, int bit) const
{
    uint8 *row = GetRow(numRow);

    while (bit > 7)
    {
        bit -= 8;
        row++;
    }

    return ((*row) >> bit) & 0x01;
}


uint8 *FullSymbol::GetRow(int row) const
{
    return offset + BytesInRow() * row;
}


uint8 FullSymbol::BytesInRow() const
{
    uint8 width = (uint8)(symbol.width >> 3);

    if (symbol.width % 8 != 0)
    {
        width++;
    }

    return width;
}


bool Font::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

    FullSymbol fullSymbol;

    if (bigFont->GetFullSymbol(fullSymbol, symbol))
    {
        return fullSymbol.BitIsExist(row, bit);
    }

    return false;
}


uint8 BigFont::GetWidth(uint8 code) const
{
    FullSymbol symbol;

    if (GetFullSymbol(symbol, code))
    {
        return symbol.symbol.width;
    }

    return 0;
}


bool BigFont::GetFullSymbol(FullSymbol &symbol, uint8 code) const
{
    for (uint i = 0; i < numSymbols; i++)
    {
        if (symbols[i].code == code)
        {
            symbol.symbol = symbols[i];
            symbol.offset = (uint8 *)data + symbols[i].offset;
            return true;
        }
    }

    return false;
}


bool Font::IsBig()
{
    return !FontIsSmall();
}
