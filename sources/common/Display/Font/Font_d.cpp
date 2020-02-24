#include "defines.h"
#include "AdvancedFont_d.h"
#include "common/Command.h"
#include "common/Decoder_d.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"
#include <cstring>


const DFont *fonts[DTypeFont::Count] = {&font5, &font8, &fontUGO, &fontUGO2, nullptr};
const DFont *font = &font8;

DTypeFont::E pushedFont = DTypeFont::_8;
DTypeFont::E currentFont = DTypeFont::_8;

static int spacing = 1;


int WorkerLengthText::recvLength = -1;


int DFont::GetLengthText(pString text)
{
    int result = 0;
    char *symbol = const_cast<char *>(text);

    while(*symbol)
    {
        result += GetWidth(*symbol) + spacing;
        symbol++;
    }
    return result;
}


int WorkerLengthText::Run(pString text)
{
    recvLength = -1;

    uint lenText = std::strlen(text);

    do
    {
        uint size = lenText + 2;

        uint8 *buffer = new uint8[size];
        buffer[0] = Command::Text_Length;
        buffer[1] = static_cast<uint8>(lenText);

        std::memcpy(buffer + 2, text, lenText);

        HAL_BUS::Panel::Send(buffer, size);

        delete[] buffer;

        uint start = TIME_MS;

        while(recvLength == -1)
        {
            HAL_BUS::Panel::Receive();
            DDecoder::Update();

            if(TIME_MS - start > 10)        /// \todo временный костыль. Надо разобраться, почему тут зависает
            {
                break;
            }
        }
    } while(recvLength == -1);

    return recvLength;
}


void WorkerLengthText::SetLength(uint8 l)
{
    recvLength = l;
}


static void SendTypeFontToPanel(DTypeFont::E type)
{
    static DTypeFont::E prevType = DTypeFont::Count;

    if (prevType != type)
    {
        HAL_BUS::Panel::Send(Command::Paint_SetFont, static_cast<uint8>(type));
        prevType = type;
    }
}


DTypeFont::E DFont::Current()
{
    return currentFont;
}


void DFont::Set(const DTypeFont::E typeFont)
{
    pushedFont = currentFont;

    if (typeFont != currentFont)
    {
        switch (typeFont)
        {
        case DTypeFont::_5:
            font = &font5;
            break;
        case DTypeFont::_8:
            font = &font8;
            break;
        case DTypeFont::_UGO:
            font = &fontUGO;
            break;
        case DTypeFont::_UGO2:
            font = &fontUGO2;
            break;
        case DTypeFont::_GOST28:
        case DTypeFont::_GOST72bold:
        case DTypeFont::_OMEGA72:
        {
            font = nullptr;
            volatile DAdvancedFont f(typeFont);
        }       
            break;
        case DTypeFont::None:
        case DTypeFont::Count:
            break;
        }

        SendTypeFontToPanel(typeFont);

        currentFont = typeFont;
    }
}


void DFont::Pop()
{
    Set(pushedFont);
}


void DFont::SetSpacing(int _spacing)
{
    spacing = _spacing;
    HAL_BUS::Panel::Send(Command::Paint_SetTextSpacing, static_cast<uint8>(spacing));
}

int DFont::GetSpacing()
{
    return spacing;
}


void DFont::SetMinWidth(uint8 width)
{
    HAL_BUS::Panel::Send(Command::Paint_SetMinWidthFont, width);
}


static bool FontIsSmall()
{
    return currentFont <= DTypeFont::_UGO2;
}


uint8 DFont::GetWidth(uint8 symbol)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].width;
    }

    return DAdvancedFont().GetWidth(symbol);
}


uint8 DFont::GetWidth(char symbol)
{
    return GetWidth(static_cast<uint8>(symbol));
}


uint8 DFont::GetHeight()
{
    if (FontIsSmall())
    {
        return static_cast<uint8>(font->_height);
    }

    return DAdvancedFont().GetHeight();
}


bool DFont::RowNotEmpty(uint8 symbol, int row)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] != 0;
    }

    return DAdvancedFont().RowNotEmpty(symbol, row);
}


bool DFont::BitIsExist(uint8 symbol, int row, int bit)
{
    if (FontIsSmall())
    {
        return font->symbols[symbol].bytes[row] & (1 << (7 - bit));
    }

    return DAdvancedFont().BitIsExist(symbol, row, bit);
}


bool DFont::IsBig()
{
    return !FontIsSmall();
}
