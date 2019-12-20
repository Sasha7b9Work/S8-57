#include "defines.h"
#include "Display/Console.h"
#include "Display/Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>
#include <cstdio>


char Console::buffer[33][100];
bool Console::inProcessDraw = false;
bool Console::inProcessAddingString = false;
int Console::stringInConsole = 0;
int16 Console::prevMaxStrinsInConsole = -1;


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
        prevMaxStrinsInConsole = set.dbg.numStrings;
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


void Console::AddString(char *string)
{
    /// \todo Мы пропускаем некоторые строки. Сделать отложенное добавление

    //if (!IsBusy())      // Страхуемся на предмет того, что сейчас не происходит вывод консоли в другом потоке
    {
        inProcessAddingString = true;

        static int count = 0;
        if (stringInConsole == set.dbg.numStrings)
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
    return set.dbg.showConsole != 0;
}


void Console::OnChanged_MaxStringsInConsole()
{
    /// \todo Здесь, видимо, не совсем корректное поведение в случае, когда реальных строк меньше, чем максимально допустимое их количество

    int delta = prevMaxStrinsInConsole - set.dbg.numStrings;

    for (int i = 0; i < delta; i++)
    {
        DeleteFirstString();
    }

    prevMaxStrinsInConsole = set.dbg.numStrings;


}


bool Console::IsBusy()
{
    return inProcessDraw || inProcessAddingString;
}
