#include "defines.h"
#include "common/Display/Font/AdvancedFont_d.h"


bool DAdvancedFont::BitIsExist(uint8, int, int)
{
    return true;
}


bool DAdvancedFont::RowNotEmpty(uint8, int)
{
    return true;
}


uint8 DAdvancedFont::GetWidth(uint8)
{
    return 8;
}


uint8 DAdvancedFont::GetHeight()
{
    return 0;
}


DAdvancedFont::DAdvancedFont(DTypeFont::E)
{
}

