#include "defines.h"
#include "Display/Primitives.h"
#include "Utils/StringUtils.h"
#include <cstring>
#include <cstdio>
#include <cstdlib>
#include <cstdarg>


const char * const String::_ERROR = "---.---"; //-V2573


String::String() : buffer(nullptr)
{
    Set(TypeConversionString::None, "");
}


String::String(const String &rhs) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(static_cast<int>(std::strlen(rhs.c_str()) + 1))) //-V2513
    {
        std::strcpy(buffer, rhs.c_str()); //-V2513
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(2))
    {
        buffer[0] = symbol;
        buffer[1] = 0;
    }
}


String::String(const char *format, ...) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (format == nullptr)
    {
        return;
    }

#ifdef WIN32
    static const int SIZE = 5000;
#else
    static const int SIZE = 5000;
#endif
    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format); //-V2528 //-V2567 //-V2571
    int numSymbols = std::vsprintf(buf, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE)
    {
        std::strcpy(buffer, "����� ������� ���"); //-V2513
    }
    else if (Allocate(static_cast<int>(std::strlen(buf) + 1))) //-V2513 //-V2516
    {
        std::strcpy(buffer, buf); //-V2513
    }
}


void String::Set(TypeConversionString::E conv, const char *format, ...)
{
    Free();

    if(format)
    {
        static const int SIZE = 100;
        char buf[SIZE + 1];

        std::va_list args;
        va_start(args, format); //-V2528 //-V2567 //-V2571
        int numSymbols = std::vsprintf(buf, format, args);
        va_end(args);

        if(numSymbols < 0 || numSymbols > SIZE)
        {
            std::strcpy(buffer, "����� ������� ���"); //-V2513
        }
        else if(Allocate(static_cast<int>(std::strlen(buf) + 1))) //-V2513 //-V2516
        {
            std::strcpy(buffer, buf); //-V2513
            Conversion(conv);
        }
    }
}


void String::Append(const char *str)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(static_cast<int>(old.Size() + std::strlen(str) + 1)); //-V2513

    std::strcpy(buffer, old.c_str()); //-V2513
    std::strcat(buffer, str); //-V2513
}


void String::Append(const char *str, int numSymbols)
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    int size = numSymbols + old.Size() + 1;

    Allocate(size);

    std::strcpy(buffer, old.c_str()); //-V2513
    std::memcpy(buffer + old.Size(), str, static_cast<uint>(numSymbols));
    buffer[size - 1] = '\0';
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(buffer);
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer);
        buffer = nullptr;
        Set(TypeConversionString::None, "");
    }
}


char *String::c_str() const
{
    return buffer;
}


bool String::Allocate(int size)
{
    std::free(buffer);
    buffer = static_cast<char *>(std::malloc(static_cast<uint>(size)));
    if (buffer)
    {
        return true;
    }

    return false;
}


int String::Draw(int x, int y, Color color) const
{
    color.SetAsCurrent();
    return Text(c_str()).Draw(x, y);
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


void String::RemoveFromBegin(int numSymbols)
{
    if (std::strlen(buffer) == static_cast<uint>(numSymbols)) //-V2513
    {
        Free();
    }
    else
    {
        String old(buffer);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        std::strcpy(buffer, old.c_str() + numSymbols); //-V2513
    }
}


void String::RemoveFromEnd()
{
    if(Size() > 0)
    {
        buffer[Size() - 1] = '\0';
    }
}


int String::Size() const
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return static_cast<int>(std::strlen(buffer)); //-V2513
}


char &String::operator[](int i) const
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i];
}
