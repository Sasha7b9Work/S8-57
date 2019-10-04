#pragma once
#include "Colors.h"
#include "Display/Font/Font.h"


struct Painter
{
    static void Init();

    static void BeginScene(Color color);

    static void EndScene();

    static void DrawTesterData(uint8 mode, Color color, const uint16 *x, const uint8 *y);
};
