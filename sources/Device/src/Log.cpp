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
void Log::Message(char *message)
{
    Console::AddString(message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::Message(char *file, int line, char *message)
{
    Console::AddString(String("%s %d %s", file, line, message).CString());
}
