#pragma once
#include "Colors.h"
#include "common/Display/Font/Font_d.h"


struct Painter
{
    static void BeginScene(Color color);

    static void EndScene();
};
