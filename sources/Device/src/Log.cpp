#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Display/Console.h"
#include "Utils/String.h"
#include <cstring>
#include <stdio.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SIZE_BUFFER 100


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Log::Message(const char *message)
{
    Console::AddString((char *)message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Log::Message(const char *file, int line, char *message)
{
    Console::AddString(String("%s %d %s", file, line, message).CString());
}
