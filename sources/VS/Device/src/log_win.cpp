#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include <iostream>
#include <windows.h>


#define SIZE_BUFFER 100

static bool consoleIsExist = false;


static void AddToSystemConsole(const char *message)
{
    if(Console::IsShown())
    {
        if(!consoleIsExist)
        {
            consoleIsExist = true;

            AllocConsole();
        }

        std::cout << message << std::endl;
    }
}


void Log::Message(const char *message)
{
    Console::AddString(const_cast<char *>(message));

    AddToSystemConsole(message);
}


void Log::Message(const char *file, int line, char *message)
{
    String msg("%s %d %s", file, line, message);

    Console::AddString(msg.CString());

    AddToSystemConsole(msg.CString());
}
