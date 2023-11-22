// 2023/11/22 13:38:02 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Console.h"
#include "Display/Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>
#include <cstdio>


namespace Console
{
    static char buffer[33][100];

    // true означает, что идЄт процесс вывода консоли и добавл€ть в неЄ новые строки нельз€ (это происходит, когда добавление идЄт из прерывани€)
    static bool inProcessDraw = false;

    // true, если происходит процесс добавлени€ строки
    static bool inProcessAddingString = false;

    //  оличество заполненных строк в консоли
    static int stringInConsole = 0;

    // «десь сохран€етс€ предыдущее значение максимального количества строк в консоли
    static int16 prevMaxStrinsInConsole = -1;

    static void DeleteFirstString();

    // ¬озвращает true, если консоль зан€та и с ней нельз€ производить работу
    static bool IsBusy();
}


void Console::Init()
{
    inProcessDraw = false;
    inProcessAddingString = false;
    stringInConsole = 0;
    prevMaxStrinsInConsole = -1;
}


void Console::Draw()
{
    if (prevMaxStrinsInConsole == -1)
    {
        prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;
    }

    if (!IsShown() || stringInConsole == 0)
    {
        return;
    }

    inProcessDraw = true;

    DFont::Set(DTypeFont::_5);

    int y = -1;

    for (int i = 0; i < stringInConsole; i++)
    {
        int length = DFont::GetLengthText(buffer[i]);
        Region(length, 6).Fill(0, y + 3, Color::BACK);
        String(buffer[i]).Draw(1, y, Color::FILL);
        y += 6;
    }

    DFont::Set(DTypeFont::_8);

    inProcessDraw = false;
}


void Console::DeleteFirstString()
{
    for (int16 i = 1; i < stringInConsole; i++)
    {
        std::strcpy(buffer[i - 1], buffer[i]); //-V2513
    }
    stringInConsole--;
}


void Console::AddString(const char *string)
{
    // \todo ћы пропускаем некоторые строки. —делать отложенное добавление

    //if (!IsBusy())      // —трахуемс€ на предмет того, что сейчас не происходит вывод консоли в другом потоке
    {
        inProcessAddingString = true;

        static int count = 0;
        if (stringInConsole == S_DBG_NUM_STRINGS_IN_CONSOLE)
        {
            DeleteFirstString();
        }
        std::sprintf(buffer[stringInConsole], "%d %s", count++, string);
        stringInConsole++;

        inProcessAddingString = false;
    }
}


bool Console::IsShown()
{
    return (S_DBG_SHOW_CONSOLE != 0);
}


void Console::OnChanged_MaxStringsInConsole()
{
    // \todo «десь, видимо, не совсем корректное поведение в случае, когда реальных строк меньше, чем максимально допустимое их количество

    int delta = prevMaxStrinsInConsole - S_DBG_NUM_STRINGS_IN_CONSOLE;

    for (int i = 0; i < delta; i++)
    {
        DeleteFirstString();
    }

    prevMaxStrinsInConsole = S_DBG_NUM_STRINGS_IN_CONSOLE;


}


bool Console::IsBusy()
{
    return inProcessDraw || inProcessAddingString;
}
