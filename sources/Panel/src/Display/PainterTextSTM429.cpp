#include "Painter.h"
#include <cstring>
#include "Display/Font/Font.h"
#include "Display/Text.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Painter::DrawText(int x, int y, const char *text)
{
    uint numSymbols = std::strlen(text);
    for (uint i = 0; i < numSymbols; ++i)
    {
        x = Text::DrawChar(x, y, (uint8)text[i]);
        ++x;
    }

    return x;
}
