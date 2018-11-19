#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "CommonFunctions.h"
#include <stdlib.h>
#include <cstring>
#include <algorithm>
#include <stdio.h>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void CF::LogBufferU8(uint8 *buffer, int num)
{
    char *message = (char *)malloc((uint)(num * 3));

    message[0] = 0;

    char value[10];

    for (int i = 0; i < num; ++i)
    {
        sprintf(value, "%02x ", buffer[i]);
        std::strcat(message, value);
    }

    message[num * 3 - 1] = 0;

    LOG_WRITE(message);

    free(message);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CF::NumWords(char *string)
{
    int num = 0;

    do
    {
        if (*string != ' ' && *string != 0)             // Если очередной символ - не пробел
        {
            ++num;                                      // то нашли очередное слово
            while (*string != ' ' && *string != 0)      // Выбираем символы этоого слова
            {
                ++string;
            }
        }
        while (*string == ' ')                          // Выбираем пробелы
        {
            ++string;
        }
    } while(*string);

    return num;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *CF::GetWord(char *string, int n, char *out, int size)
{
    if (n >= NumWords(string))
    {
        return 0;
    }

    if (n == 1)
    {
        n = 1;
    }

    // Находим начало искомого слова
    int currentWord = 0;
    do
    {
        if (*string != ' ')                         // Если текущий символ - не пробел, то нашли новое слово
        {
            if (currentWord == n)                   // Если текущее слово - наше
            {
                break;                              // то выходим - string указаывает на искомое слово
            }
            ++currentWord;
            while (*string != ' ' && *string != 0)     // Выбираем буквы этого слова
            {
                ++string;
            }
        }
        while (*string == ' ')
        {
            ++string;                   // Выбираем пробелы
        }
    } while(*string);

    // Находим конец искомого символа
    char *end = string;

    while (*end != ' ' && *end != 0)
    {
        ++end;                          // Ищем конец слова
    }

    int length = end - string;

    if (length + 1 > size)
    {
        return (char *)0xffffffff;              // Не хватит места в выходном буфере - выходим с соответствующим кодом
    }

    for (int i = 0; i < length; i++)
    {
        out[i] = string[i];
    }
    out[length] = 0;

    return out;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
BufferMax5::BufferMax5()
{
    Clear();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BufferMax5::Clear()
{
    numElements = 0;
    buffer[0] = buffer[1] = buffer[2] = buffer[3] = buffer[4] = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BufferMax5::Push(uint16 value)
{
    if (numElements < 5)
    {
        buffer[numElements++] = value;
    }
    else
    {
        if (value > buffer[0])
        {
            buffer[0] = value;
            std::sort(&buffer[0], &buffer[5]);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 BufferMax5::GetValue(int index)
{
    if (index >= numElements)
    {
        return 0xffff;
    }
    return buffer[index];
}
