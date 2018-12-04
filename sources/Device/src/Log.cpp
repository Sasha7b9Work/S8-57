#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Utils/String.h"
#include <stdarg.h>
#include <cstring>
#include <stdio.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_BUFFER 100


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Message(char *format, ...)
{
    char buffer[SIZE_BUFFER + 1] = { 0 };
    
    va_list args;
    va_start(args, format);
    int numSymbols = std::vsprintf(buffer, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE_BUFFER)
    {
        LOG_ERROR("Буфер слишком мал");
    }

    Console::AddString(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::MessageString(char *message)
{
    Console::AddString(message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::MessageTrace(char *file, int line, char *format, ...)
{
    char buffer[SIZE_BUFFER + 1] = { 0 };

    va_list args;
    va_start(args, format);
    int numSymbols = std::vsprintf(buffer, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE_BUFFER)
    {
        LOG_ERROR("Буфер слишком мал");
    }

    Console::AddString(String("%s %d %s", file, line, buffer).CString());
}
