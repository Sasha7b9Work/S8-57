#include "defines.h"
#include "Display/Colors.h"



void Color::SetAsCurrent()
{
    if (value != Color::NUMBER.value)
    {
        currentColor = *this;
    }
    if (!WriteFlashColor())
    {
        WriteToDisplay(currentColor);
    }
}
