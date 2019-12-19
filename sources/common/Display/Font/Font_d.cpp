#include "defines.h"
#include "AdvancedFont_d.h"
#include "common/Command.h"
#include "common/Transceiver.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"
#include <cstring>

#ifdef DEVICE
#include "Keyboard/DecoderDevice.h"
#endif

#ifdef LOADER
#include "Keyboard/DecoderLoader.h"
#endif


const Font *fonts[TypeFont::Count] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
const Font *font = &font8;

TypeFont::E pushedFont = TypeFont::_8;
TypeFont::E currentFont = TypeFont::_8;

static int spacing = 1;

/// Используется для приёма длины текста от панели
static int recvLength = -1;


int Font::GetLengthText(pString text)
{
    recvLength = -1;
    
    uint lenText = std::strlen(text);

    uint size = lenText + 2;

    uint8 *buffer = new uint8[size];
    buffer[0] = Command::Text_Length;
    buffer[1] = static_cast<uint8>(lenText);

    std::memcpy(buffer + 2, text, lenText);

    Transceiver::Send(buffer, size);
    
    delete [] buffer;

    while(recvLength == -1)
    {
        Transceiver::Receive();
        Decoder::Update();
    }

    return recvLength;
}

void Font::SetLength(uint8 l)
{
    recvLength = l;
}


static void SendTypeFontToPanel(TypeFont::E type)
{
    static TypeFont::E prevType = TypeFont::Count;

    if (prevType != type)
    {
        Transceiver::Send(Command::Paint_SetFont, static_cast<uint8>(type));
        prevType = type;
    }
}


TypeFont::E Font::Current()
{
    return currentFont;
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
        case TypeFont::_OMEGA72:
        {
            font = nullptr;
            volatile AdvancedFont f(typeFont);
        }       
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


void Font::SetSpacing(int _spacing)
{
    spacing = _spacing;
    Transceiver::Send(Command::Paint_SetTextSpacing, static_cast<uint8>(spacing));
}

int Font::GetSpacing()
{
    return spacing;
}


void Font::SetMinWidth(uint8 width)
{
    Transceiver::Send(Command::Paint_SetMinWidthFont, width);
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
