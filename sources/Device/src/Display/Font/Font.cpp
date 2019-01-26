#include "defines.h"
#include "Command.h"
#include "Hardware/HAL/HAL.h"
#include "font8.inc"
#include "font5.inc"
#include "fontUGO.inc"
#include "fontUGO2.inc"


using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Font *fonts[Font::Type::Number] = {&font5, &font8, &fontUGO, &fontUGO2};
const Font *font = &font8;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Font::GetSize(void)
{
    return font->height;
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
    return font->symbol[symbol].width + 1;
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
        case Type::None:
        case Type::Number:
            break;
    }

    uint8 buffer[2] = { Command::Paint_SetFont, (uint8)typeFont };
    FSMC::WriteToPanel(buffer, 2);
}
