#include "defines.h"
#include "Command.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"


using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Font *fonts[Font::Type::Size] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
const Font *font = &font8;

const BigFont *bigFont = &fontDigits64;

Font::Type::E pushedFont = Font::Type::_8;
Font::Type::E currentFont = Font::Type::_8;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Font *Font::Current()
{
    return font;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Font::GetHeightSymbol(char)
{
    return 9;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Font::SetCurrent(Font::Type::E typeFont)
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
            font = nullptr;
            break;
        case Type::None:
        case Type::Size:
            break;
    }

    uint8 buffer[2] = { Command::Paint_SetFont, (uint8)typeFont };
    FSMC::WriteToPanel(buffer, 2);

    pushedFont = currentFont;
    currentFont = typeFont;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Font::Pop()
{
    SetCurrent(pushedFont);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool FontIsSmall()
{
    return currentFont <= Font::Type::_UGO2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Font::GetWidth(uint8 symbol)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].width;
    }

    return bigFont->GetWidth(symbol);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 Font::GetHeight()
{
    if (FontIsSmall())
    {
        return font->_height;
    }

    return bigFont->height;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Font::RowNotEmpty(uint8 symbol, int row)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Font::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << bit);
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 BigFont::GetWidth(uint8 symbol) const
{
    for (uint i = 0; i < numSymbols; i++)
    {
        if (symbols[i].code == symbol)
        {
            return symbols[i].width;
        }
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool BigFont::ExistSymbol(uint8 symbol) const
{
    for (uint i = 0; i < numSymbols; i++)
    {
        if (symbols[i].code == symbol)
        {
            return true;
        }
    }

    return false;
}
