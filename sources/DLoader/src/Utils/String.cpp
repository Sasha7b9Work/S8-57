#include "defines.h"
#include "Display/Primitives.h"
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

    if (Allocate(std::strlen(rhs.c_str()) + 1)) //-V2513
    {
        std::strcpy(buffer, rhs.c_str()); //-V2513
    }
}


String::String(char symbol) : buffer(nullptr)
{
    Set(TypeConversionString::None, "");

    if (Allocate(2))
    {
        buffer[0] = symbol; //-V2563
        buffer[1] = 0; //-V2563
    }
}


String::String(const char *format, ...) : buffer(nullptr) //-V2506
{
    Set(TypeConversionString::None, "");

    if (format == nullptr)
    {
        return;
    }

    static const int SIZE = 500;
    char buf[SIZE + 1];

    std::va_list args;
    va_start(args, format); //-V2528 //-V2567
    int numSymbols = std::vsprintf(buf, format, args);
    va_end(args);

    if (numSymbols < 0 || numSymbols > SIZE)
    {
        std::strcpy(buffer, "Буфер слишком мал"); //-V2513
    }
    else if (Allocate(std::strlen(buf) + 1)) //-V2513
    {
        std::strcpy(buffer, buf); //-V2513
    }
    else
    {
        // здесь ничего
    }
}


void String::Set(TypeConversionString::E /*conv*/, const char *format, ...)
{
    Free();

    if(format)
    {
        static const int SIZE = 100;
        char buf[SIZE + 1];

        std::va_list args;
        va_start(args, format); //-V2528 //-V2567
        int numSymbols = std::vsprintf(buf, format, args);
        va_end(args);

        if(numSymbols < 0 || numSymbols > SIZE)
        {
            std::strcpy(buffer, "Буфер слишком мал"); //-V2513
        }
        else if(Allocate(std::strlen(buf) + 1)) //-V2513
        {
            std::strcpy(buffer, buf); //-V2513
        }
        else
        {
            // здесь ничего
        }
    }
}


void String::Append(const char *str) //-V2506
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    Allocate(old.Size() + std::strlen(str) + 1); //-V2513

    std::strcpy(buffer, old.c_str()); //-V2513
    std::strcat(buffer, str); //-V2513
}


void String::Append(const char *str, uint numSymbols) //-V2506
{
    if (!str || *str == '\0')
    {
        return;
    }

    String old(*this);

    Free();

    uint size = numSymbols + old.Size() + 1;

    Allocate(size);

    std::strcpy(buffer, old.c_str()); //-V2513
    std::memcpy(buffer + old.Size(), str, numSymbols); //-V2563
    buffer[size - 1] = '\0'; //-V2563
}


void String::Append(char symbol)
{
    char b[2] = { symbol, '\0' };
    Append(b);
}


String::~String()
{
    std::free(buffer); //-V2511
}


void String::Free()
{
    if(buffer)
    {
        std::free(buffer); //-V2511
        buffer = nullptr;
        Set(TypeConversionString::None, "");
    }
}


char *String::c_str() const
{
    return buffer;
}


bool String::Allocate(uint size) //-V2506
{
    std::free(buffer); //-V2511
    buffer = static_cast<char *>(std::malloc(size)); //-V2511
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


void String::RemoveFromBegin(uint numSymbols)
{
    if (std::strlen(buffer) == numSymbols) //-V2513
    {
        Free();
    }
    else
    {
        String old(buffer);

        Free();

        Allocate(old.Size() - numSymbols + 1);

        std::strcpy(buffer, old.c_str() + numSymbols); //-V2513 //-V2563
    }
}


void String::RemoveFromEnd()
{
    if(Size() > 0)
    {
        buffer[Size() - 1] = '\0'; //-V2563
    }
}


uint String::Size() const //-V2506
{
    if (buffer == nullptr)
    {
        return 0;
    }

    return std::strlen(buffer); //-V2513
}


char &String::operator[](uint i) //-V2506
{
    static char result = 0;

    if (buffer == nullptr || Size() < i)
    {
        return result;
    }

    return buffer[i]; //-V2563
}
