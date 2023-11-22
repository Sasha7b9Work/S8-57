// 2023/11/22 13:37:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"
#include "common/Display/Font/Font_d.h"


struct Painter
{
    static void Init();

    static void BeginScene(Color color);

    static void EndScene();

    static void DrawTesterData(uint8 mode, Color color, const uint16 *x, const uint8 *y);
};
