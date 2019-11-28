#include "defines.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"


const Font *fonts[TypeFont::Count] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
const Font *font = &font8;

TypeFont::E pushedFont = TypeFont::_8;
TypeFont::E currentFont = TypeFont::_8;

#ifndef PANEL
static int spacing = 1;
#endif


int Font::GetLengthText(pString text)
{
    int result = 0;
    char *symbol = const_cast<char *>(text);

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

#ifdef PANEL
static void SendTypeFontToPanel(TypeFont::E) {};
#else
static void SendTypeFontToPanel(TypeFont::E type)
{
    static TypeFont::E prevType = TypeFont::Count;

    if (prevType != type)
    {
        Transceiver::Send(Command::Paint_SetFont, static_cast<uint8>(type));
        prevType = type;
    }
}
#endif


void Font::Set(TypeFont::E typeFont)
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
            break;
        case TypeFont::None:
        case TypeFont::Count:
            break;
        }

        SendTypeFontToPanel(typeFont);

        currentFont = typeFont;
    }
}


void Font::Pop()
{
    Set(pushedFont);
}


#ifdef PANEL
void Font::SetSpacing(int) {}
#else
void Font::SetSpacing(int _spacing)
{
    spacing = _spacing;
    Transceiver::Send(Command::Paint_SetTextSpacing, static_cast<uint8>(spacing));
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
    Transceiver::Send(Command::Paint_SetMinWidthFont, width);
}
#endif


static bool FontIsSmall()
{
    return currentFont <= TypeFont::_UGO2;
}


uint8 Font::GetWidth(uint8 symbol)
{
    //uint8 result = bigFont->GetWidth(symbol);

    uint8 result = 8;

    if (FontIsSmall())
    {
        result = font->symbols[symbol].width;
    }

    return result;
}


uint8 Font::GetHeight()
{
    //uint8 result = bigFont->height;

    uint8 result = 8;

    if (FontIsSmall())
    {
        result = static_cast<uint8>(font->_height);
    }

    return result;
}


bool Font::RowNotEmpty(uint8 symbol, int row)
{
    bool result = false;

    if (FontIsSmall())
    {
        result = font->symbols[symbol].bytes[row] != 0;
    }
    else
    {
        //FullSymbol fullSymbol;
        //
        //if (bigFont->GetFullSymbol(fullSymbol, symbol))
        //{
        //    result = fullSymbol.RowNotEmpty(row);
        //}
    }

    return result;
}


bool Font::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

//    FullSymbol fullSymbol;
//
//    if (bigFont->GetFullSymbol(fullSymbol, symbol))
//    {
//        return fullSymbol.BitIsExist(row, bit);
//    }

    return false;
}


bool Font::IsBig()
{
    return !FontIsSmall();
}
