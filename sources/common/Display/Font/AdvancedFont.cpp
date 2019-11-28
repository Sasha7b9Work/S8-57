#include "defines.h"
#include "AdvancedFont.h"


extern unsigned char fontGOST28[9158];


TypeFont::E AdvancedFont::currentType = TypeFont::None;


static uint8 *fonts[TypeFont::Count] =
{
    nullptr,        // _5
    nullptr,        // _8
    nullptr,        // _UGO
    nullptr,        // _UGO2
    fontGOST28      // _GOST28
};

static uint8 *font = nullptr;


void AdvancedFont::Set(TypeFont::E t)
{
    currentType = t;
    font = fonts[currentType];
}


bool AdvancedFont::RowNotEmpty(uint8, int)
{
    return false;
}


uint8 AdvancedFont::GetWidth(uint8)
{
    return 0;
}

uint8 AdvancedFont::GetHeight(uint8)
{
    return 0;
}


uint8 AdvancedFont::GetHeight()
{
    return 0;
}
