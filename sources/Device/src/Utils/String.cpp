#include "defines.h"
#include "Display/Primitives.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


const char * const String::ERROR = "---.---";


String::String() : buffer(nullptr)
{

}


String::String(const String &rhs) : buffer(0)
{
    if (Allocate(std::strlen(rhs.CString()) + 1))
    {
        std::strcpy(buffer, rhs.CString());
    }
}


String::String(char symbol) : buffer(0)
{
    if (Allocate(2))
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(const char *format, ...)
{
#define SIZE 150
    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format); //-V2528
    int numSymbols = std::vsprintf(buf, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE)
    {
        LOG_ERROR("Буфер слишком мал");
    }

    if (Allocate(std::strlen(buf) + 1))
    {
        std::strcpy(buffer, buf);
    }
}


void String::Set(TypeConversionString::E conv, const char *format, ...)
{
    Free();

    if(format)
    {
#define SIZE 100
        char buf[SIZE + 1];

        std::va_list args;
        va_start(args, format); //-V2528
        int numSymbols = std::vsprintf(buf, format, args);
        va_end(args);

        if(numSymbols < 0 || numSymbols > SIZE)
        {
            LOG_ERROR("Буфер слишком мал");
        }

        if(Allocate(std::strlen(buf) + 1))
        {
            std::strcpy(buffer, buf);
            Conversion(conv);
        }
    }
}


String::~String()
{
    Free();
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer);
        buffer = nullptr;
    }
}


char *String::CString() const
{
    return buffer;
}


bool String::Allocate(uint size)
{
    buffer = static_cast<char *>(std::malloc(size));
    if (buffer)
    {
        return true;
    }
    else
    {
        LOG_ERROR("Не хватает памяти");
    }
    return false;
}


int String::Draw(int x, int y, Color color) const
{
    color.SetAsCurrent();
    return Text(CString()).Draw(x, y);
}


void String::Conversion(TypeConversionString::E conv)
{
    char *pointer = buffer;

    if(conv == TypeConversionString::FirstUpper)
    {
        if(*pointer)
        {
            *pointer = SU::ToUpper(*pointer);
            pointer++;
        }

        while(*pointer)
        {
            *pointer = SU::ToLower(*pointer);
            pointer++;
        }
    }
}
