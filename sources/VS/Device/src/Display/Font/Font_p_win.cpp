#include "common/Display/Font/Font_d.h"
#include "../../../../Panel/src/Display/Font/Font_p.h"


static int spacing = 1;


void PFont::SetSpacing(int s)
{
    spacing = s;
}


int PFont::GetLengthText(pString text)
{
    int result = 0;
    char *symbol = const_cast<char *>(text);

    while(*symbol)
    {
        result += DFont::GetWidth(*symbol) + spacing;
        symbol++;
    }
    return result;
}
