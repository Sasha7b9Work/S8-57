#include "Painter.h"
#include <cstring>
#include "Display/Font/Font.h"
#include "Display/Text.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Painter::DrawText(int x, int y, const char *text, Color color)
{
    SetColor(color);
    
    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = Text::DrawChar(x, y, text[i]);
        ++x;
    }

    return x;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SetFont(Font::Type::E typeFont)
{
    font = fonts[typeFont];
}
