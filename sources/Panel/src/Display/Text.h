#pragma once
#include "Display/Colors.h"


class Text
{
public:
    static int Draw(int x, int y, const char *text);

    static int DrawChar(int x, int y, uint8 symbol, Color color = Color::NUMBER);
    /// ”становка моноширинного режима вывода текста
    static void SetMinWidthFont(uint8 width);

    static void SetSpacing(uint8 space);
};
