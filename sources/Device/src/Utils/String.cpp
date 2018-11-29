#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "String.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
String::String(char *format, ...)
{
    char buf[100];

    std::va_list args;
    va_start(args, format);
    vsprintf(buf, format, args);
    va_end(args);

    buffer = (char *)std::malloc(std::strlen(buf) + 1);

    if (buffer)
    {
        std::strcpy(buffer, buf);
    }
    else
    {
        LOG_ERROR("Нету памяти");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String::~String()
{
    std::free(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *String::CString() const
{
    return buffer;
}
