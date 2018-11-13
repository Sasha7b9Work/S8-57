#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Console.h"
#include "Display/Painter.h"
#include <string.h>
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Console::inProcessDrawConsole = false; 
int Console::stringInConsole = 0;
#define SIZE_CONSOLE   10
static CHAR_BUF2(buffer, SIZE_CONSOLE, 100);


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Console::Draw()
{
    int x0 = 00;

    inProcessDrawConsole = true;

    Painter::SetFont(Font::Type::_5);

    int y = 1;

    for (int i = 0; i < stringInConsole; i++)
    {
        int length = Font::GetLengthText(buffer[i]);
        Painter::FillRegion(x0, y + 3, length, 6, Color::BACK);
        Painter::DrawText(x0 + 1, y, buffer[i], Color::FILL);
        y += 6;
    }

    Painter::SetFont(Font::Type::_8);

    inProcessDrawConsole = false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
                strcpy(buffer[i - 1], buffer[i]);
            }
            stringInConsole--;
        }
        sprintf(buffer[stringInConsole], "%d %s", count++, string);
        stringInConsole++;
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Console::NumberOfLines()
{
    return stringInConsole;
}
