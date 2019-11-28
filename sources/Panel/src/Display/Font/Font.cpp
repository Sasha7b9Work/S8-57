#include "defines.h"
#include "Display/Font/AdvancedFont.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Hardware/HAL/HAL.h"
#include "Display/Font/font8.inc"
#include "Display/Font/font5.inc"
#include "Display/Font/fontUGO.inc"
#include "Display/Font/fontUGO2.inc"


const Font *fonts[TypeFont::Count] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
const Font *font = &font8;

TypeFont::E pushedFont = TypeFont::_8;
TypeFont::E currentFont = TypeFont::_8;


int Font::GetLengthText(pString text)
{
    int result = 0;
    char *symbol = const_cast<char *>(text);

    while (*symbol)
    {
        result += Font::GetWidth(*symbol);
        symbol++;
    }
    return result;
}


void Font::Set(const TypeFont::E typeFont)
{
    pushedFont = currentFont;

    if (typeFont != currentFont)
    {
        switch (typeFont)
        {
        case TypeFont::_5:
            font = &font5;
            break;
        case TypeFont::_8:
            font = &font8;
            break;
        case TypeFont::_UGO:
            font = &fontUGO;
            break;
        case TypeFont::_UGO2:
            font = &fontUGO2;
            break;
        case TypeFont::_GOST28:
        case TypeFont::_GOST72bold:
        {
            font = nullptr;
            volatile AdvancedFont f(typeFont);
        }       
            break;
        case TypeFont::None:
        case TypeFont::Count:
            break;
        }

        currentFont = typeFont;
    }
}


static bool FontIsSmall()
{
    return currentFont <= TypeFont::_UGO2;
}


uint8 Font::GetWidth(uint8 symbol)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].width;
    }

    return AdvancedFont().GetWidth(symbol);
}


uint8 Font::GetWidth(char symbol)
{
    return GetWidth(static_cast<uint8>(symbol));
}


uint8 Font::GetHeight()
{
    if (FontIsSmall())
    {
        return static_cast<uint8>(font->_height);
    }

    return AdvancedFont().GetHeight();
}


bool Font::RowNotEmpty(uint8 symbol, int row)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    return AdvancedFont().RowNotEmpty(symbol, row);
}


bool Font::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

    return AdvancedFont().BitIsExist(symbol, row, bit);
}


bool Font::IsBig()
{
    return !FontIsSmall();
}
