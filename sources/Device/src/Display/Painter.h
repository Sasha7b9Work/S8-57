// 2023/11/22 13:37:40 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Colors.h"
#include "common/Display/Font/Font_d.h"


namespace Painter
{
    void Init();

    void BeginScene(Color color);

    void EndScene();

    void DrawTesterData(uint8 mode, Color color, const uint16 *x, const uint8 *y);
};
