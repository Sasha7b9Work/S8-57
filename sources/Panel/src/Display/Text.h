#pragma once
#include "Display/Colors.h"


class Text
{
public:
    static int DrawChar(int x, int y, char symbol, Color color = Color::NUMBER);

    static int DrawBigChar(int eX, int eY, int size, char symbol);

    static void DrawBigText(int x, int y, uint8 size, pString text);

private:
    static bool ByteFontNotEmpty(int eChar, int byte);

    static bool BitInFontIsExist(int eChar, int numByte, int bit);
};
