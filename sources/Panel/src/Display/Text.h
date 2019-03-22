#pragma once
#include "Display/Colors.h"


class Text
{
public:
    static int DrawChar(int x, int y, uint8 symbol, Color color = Color::NUMBER);

    static int DrawBigChar(int eX, int eY, int size, uint8 symbol);

    static void DrawBigText(int x, int y, uint8 size, pString text);
};
