// 2023/11/20 16:47:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "log.h"
#include "CommonFunctions.h"
#include <cstdlib>
#include <cstring>
#include <algorithm>
#include <cstdio>



void CF::LogBufferU8_HEX(const uint8 *buffer, int num) //-V2506
{
    char *message = static_cast<char *>(std::malloc(static_cast<uint>(num * 3)));

    if (message == nullptr)
    {
        return;
    }

    message[0] = 0;

    char value[10];

    for (int i = 0; i < num; ++i)
    {
        std::sprintf(value, "%02x ", buffer[i]);
        std::strcat(message, value); //-V2513
    }

    message[num * 3 - 1] = 0;

    LOG_WRITE(message);

    std::free(message);
}


void CF::LogBufferU8_DEC(const uint8 *buffer, int num) //-V2506
{
    char *message = static_cast<char *>(std::malloc(static_cast<uint>(num * 3)));

    if (message == nullptr)
    {
        LOG_WRITE("Нет памяти");
        return;
    }

    message[0] = 0;

    char value[10];

    for (int i = 0; i < num; ++i)
    {
        std::sprintf(value, "%03d ", buffer[i]);
        std::strcat(message, value); //-V2513
    }

    message[num * 3 - 1] = 0;

    LOG_WRITE(message);

    std::free(message);
}


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


char *CF::GetWord(char *string, int n, char *out, int size) //-V2506
{
    if (n >= NumWords(string))
    {
        return 0;
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
        return reinterpret_cast<char *>(0xffffffffU);   // Не хватит места в выходном буфере - выходим с соответствующим кодом //-V566 //-V2571
    }

    for (int i = 0; i < length; i++)
    {
        out[i] = string[i];
    }
    out[length] = 0;

    return out;
}



BufferMax5::BufferMax5()
{
    Clear();
}


void BufferMax5::Clear()
{
    numElements = 0;
    buffer[0] = 0;
    buffer[1] = 0;
    buffer[2] = 0;
    buffer[3] = 0;
    buffer[4] = 0;
}


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


uint16 BufferMax5::GetValue(int index) //-V2506
{
    if (index >= numElements)
    {
        return 0xffff;
    }
    return buffer[index];
}
