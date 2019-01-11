#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Console.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include <cstring>
#include <stdio.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Region;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Console::inProcessDrawConsole = false; 
int Console::stringInConsole = 0;
#define SIZE_CONSOLE   10
static CHAR_BUF2(buffer, SIZE_CONSOLE, 100);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Console::Draw()
{
    inProcessDrawConsole = true;

    Painter::SetFont(Font::Type::_5);

    int y = 1;

    for (int i = 0; i < stringInConsole; i++)
    {
        int length = Font::GetLengthText(buffer[i]);
        Region(length, 6).Draw(0, y + 3, Color::BACK);
        String(buffer[i]).Draw(1, y, Color::FILL);
        y += 6;
    }

    Painter::SetFont(Font::Type::_8);

    inProcessDrawConsole = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Console::AddString(char *string)
{
    /// \todo Мы пропускаем некоторые строки. Сделать отложенное добавление
    if (!inProcessDrawConsole)      // Страхуемся на предмет того, что сейчас не происходит вывод консоли в другом потоке
    {
        static int count = 0;
        if (stringInConsole == SIZE_CONSOLE)
        {
            for (int i = 1; i < SIZE_CONSOLE; i++)
            {
                std::strcpy(buffer[i - 1], buffer[i]); //-V2513
            }
            stringInConsole--;
        }
        sprintf(buffer[stringInConsole], "%d %s", count++, string);
        stringInConsole++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Console::NumberOfLines()
{
    return stringInConsole;
}
