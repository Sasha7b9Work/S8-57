#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Values.h"
#include "Math.h"
#include "Settings/Settings.h"
#include <cstring>
#include <cmath>
#include <stdlib.h>
#include <limits>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Float::Float(float val) : m_val(val)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Hex::ToBin(int depth, char buffer[36]) const
{
    int byte = 3;       /// — этого байта начинаем вывод. “.к. в начале строки - старший байт, в конце - младший

    switch (depth)
    {
        case 8: byte = 0;  break;
        case 16: byte = 1; break;
    }

    char *pointer = buffer;

    while (byte >= 0)
    {
        BinToString8((uint8)(value >> (byte * 8)), pointer);
        if (byte > 0)
        {
            *(pointer + 8) = ' ';
        }
        pointer += 9;
        byte--;
    }

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Hex::BinToString8(uint8 val, char buffer[9]) const
{
    for (int bit = 0; bit < 8; bit++)
    {
        buffer[7 - bit] = _GET_BIT(val, bit) ? '1' : '0';
    }
    buffer[8] = '\0';
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Hex::ToHex(int depth, char buffer[9]) const
{
    switch (depth)
    {
        case 8:     sprintf(buffer, "%02X", value); break;
        case 16:    sprintf(buffer, "%04X", value); break;
        case 32:    sprintf(buffer, "%08X", value); break;
        default:                                    break;
    }

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Hex::DigitInPosition(int pos) const
{
    uint val = value;

    while (pos > 0)
    {
        val /= 10;
        pos--;
    }

    return (char)((val % 10) | 0x30);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Integer::ToString(bool alwaysSign, int numMinFields, char buffer[20]) const
{
    const int SIZE = 20;
    char format[SIZE] = "%";
    snprintf(&(format[1]), SIZE, "0%d", numMinFields);
    std::strcat(format, "d");
    if (alwaysSign && value >= 0)
    {
        buffer[0] = '+';
        snprintf(buffer + 1, SIZE - 1, format, value);
    }
    else
    {
        snprintf(buffer, SIZE, format, value);
    }
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Frequency::ToString(char bufferOut[20]) const
{
    float freq = value;

    bufferOut[0] = 0;
    const char *suffix = 0;
    if (Math::IsEquals(freq, ERROR_VALUE_FLOAT))
    {
        std::strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6f)
    {
        suffix = LANG_RU ? "ћ√ц" : "MHz";
        freq /= 1e6f;
    }
    else if (freq >= 1e3f)
    {
        suffix = LANG_RU ? "к√ц" : "kHz";
        freq /= 1e3f;
    }
    else
    {
        suffix = LANG_RU ? "√ц" : "Hz";
    }
    char buffer[20];
    std::strcat(bufferOut, Float(freq).ToString(false, 4, buffer));
    std::strcat(bufferOut, suffix);
    return bufferOut;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Frequency::ToStringAccuracy(char bufferOut[20], int numDigits) const
{
    float freq = value;

    bufferOut[0] = 0;
    const char *suffix = LANG_RU ? "√ц" : "Hz";
    if (Math::IsEquals(freq, ERROR_VALUE_FLOAT))
    {
        std::strcat(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    if (freq >= 1e6f)
    {
    suffix = LANG_RU ? "ћ√ц" : "MHz";
    freq /= 1e6f;
    }
    else if (freq >= 1e3f)
    {
    suffix = LANG_RU ? "к√ц" : "kHz";
    freq /= 1e3f;
    }
    char buffer[20];
    std::strcat(bufferOut, Float(freq).ToString(false, numDigits, buffer));
    std::strcat(bufferOut, suffix);
    return bufferOut;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Time::ToString(bool alwaysSign, char buffer[20]) const
{
    float time = value;

    if (Math::IsEquals(time, ERROR_VALUE_FLOAT))
    {
        std::strcpy(buffer, ERROR_STRING_VALUE);
        return buffer;
    }

    pString suffix[2][4] =
    {
        {"нс", "мкс", "мс", "с"},
        {"ns", "us",  "ms", "s"}
    };

    static const float factor[4] = { 1e9f, 1e6f, 1e3f, 1.0f };

    float absTime = std::fabsf(time);

    int num = 0;

    if (absTime + 0.5e-10f < 1e-6f)
    {
    }
    else if (absTime + 0.5e-7f < 1e-3f)
    {
        num = 1;
    }
    else if (absTime + 0.5e-3f < 1.0f)
    {
        num = 2;
    }
    else
    {
        num = 3;
    }

    char bufferOut[20];
    std::strcpy(buffer, Float(time * factor[num]).ToString(alwaysSign, 4, bufferOut));
    std::strcat(buffer, suffix[LANG][num]);
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Time::ToStringAccuracy(bool alwaysSign, char buffer[20], int numDigits) const
{
    float time = value;

    buffer[0] = 0;
    const char *suffix = LANG_RU ? "с" : "s";

    float fabsTime = std::fabsf(time);

    if (Math::IsEquals(time, ERROR_VALUE_FLOAT))
    {
        std::strcat(buffer, ERROR_STRING_VALUE);
        return buffer;
    }
    else if (fabsTime + 0.5e-10f < 1e-6f)
    {
        suffix = LANG_RU ? "нс" : "ns";
        time *= 1e9f;
    }
    else if (fabsTime + 0.5e-7f < 1e-3f)
    {
        suffix = LANG_RU ? "мкс" : "us";
        time *= 1e6f;
    }
    else if (fabsTime + 0.5e-3f < 1.0f)
    {
        suffix = LANG_RU ? "мс" : "ms";
        time *= 1e3f;
    }

    char bufferOut[20];
    std::strcat(buffer, Float(time).ToString(alwaysSign, numDigits, bufferOut));
    std::strcat(buffer, suffix);

    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Voltage::ToString(bool alwaysSign) const
{
    char buffer[20];
    return String(ToString(alwaysSign, buffer));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Voltage::ToString(bool alwaysSign, char buffer[20]) const
{
    float voltage = value;

    if (Math::IsEquals(voltage, ERROR_VALUE_FLOAT))
    {
        std::strcpy(buffer, ERROR_STRING_VALUE);
        return buffer;
    }

    pString suf[2][4] =
    {
        {"\x10мк¬", "\x10м¬", "\x10¬", "\x10к¬"},
        {"\x10uV",  "\x10mV", "\x10V", "\x10kV"}
    };

    static const float factor[4] = {1e6f, 1e3f, 1.0f, 1e-3f};

    int num = 0;
    float absValue = std::fabsf(voltage) + 0.5e-4f;

    if (absValue < 1e-3f)
    {
        num = 0;
    }
    else if (absValue < 1.0f)
    {
        num = 1;
    }
    else if (absValue < 1e3f)
    {
        num = 2;
    }
    else
    {
        num = 3;
    }

    CHAR_BUF(bufferOut, 20);

    Float(voltage * factor[num]).ToString(alwaysSign, 4, bufferOut);

    std::strcpy(buffer, bufferOut);
    std::strcat(buffer, suf[LANG][num]);
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Current::ToString(char buffer[50]) const
{
    float current = value;

    if (Math::IsEquals(current, ERROR_VALUE_FLOAT))
    {
        std::strcpy(buffer, ERROR_STRING_VALUE);
        return buffer;
    }

    pString suf[2][4] =
    {
            { "\x10мкј", "\x10мј", "\x10ј", "\x10кј" },
        { "\x10uј",  "\x10mј", "\x10ј", "\x10kј"}
    };

    static const float factor[4] = { 1e6f, 1e3f, 1.0f, 1e-3f };

    int num = 0;

    float absValue = std::fabsf(current) + 0.5e-4f;

    if (absValue < 1e-3f)
    {
        num = 0;
    }
    else if (absValue < 1.0f)
    {
        num = 1;
    }
    else if (absValue < 1e3f)
    {
        num = 2;
    }
    else
    {
        num = 3;
    }

    CHAR_BUF(bufferOut, 20);

    Float(current * factor[num]).ToString(true, 4, bufferOut);

    std::strcpy(buffer, bufferOut);
    std::strcat(buffer, suf[LANG][num]);
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Phase::ToString(char bufferOut[20]) const
{
    char buffer[20];
    sprintf(bufferOut, "%s\xa8", Float(value).ToString(false, 4, buffer));
    return bufferOut;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char *Float::ToString(bool alwaysSign, int numDigits, char bufferOut[20]) const
{
    float _value = m_val;
    
    if(_value == std::numeric_limits<float>::infinity())
    {
        _value = _value;
    }

    if (Math::IsEquals(_value, ERROR_VALUE_FLOAT))
    {
        std::strcpy(bufferOut, ERROR_STRING_VALUE);
        return bufferOut;
    }
    
    _value = Math::RoundFloat(_value, numDigits);
    
    char *pBuffer = bufferOut;
    
    if (_value < 0)
    {
        *pBuffer++ = '-';
    }
    else if (alwaysSign)
    {
        *pBuffer++ = '+';
    }
    
    char format[10] = "%4.2f\0\0";
    
    format[1] = (char)numDigits + 0x30;
    
    int numDigitsInInt = Math::DigitsInIntPart(_value);
    
    format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
    if (numDigits == numDigitsInInt)
    {
        format[5] = '.';
    }
    
    float absValue = std::fabsf(_value);
    sprintf(pBuffer, (char *)format, (double)absValue);
    
    float val = (float)atof(pBuffer);
    
    if (Math::DigitsInIntPart(val) != numDigitsInInt)
    {
        numDigitsInInt = Math::DigitsInIntPart(val);
        format[3] = (char)((numDigits - numDigitsInInt) + 0x30);
        if (numDigits == numDigitsInInt)
        {
            format[5] = '.';
        }
        sprintf(pBuffer, format, (double)_value);
    }
    
    bool signExist = alwaysSign || _value < 0;
    while (std::strlen(bufferOut) < (size_t)(numDigits + (signExist ? 2 : 1)))
    {
        std::strcat(bufferOut, "0");
    }
    
    return bufferOut;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Hex::NumDigits() const
{
    uint val = value;

    int num = 1;
    while ((val /= 10) > 0)
    {
        num++;
    }
    return num;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Integer::NumDigits() const
{
    int val = Math::Abs(value);
    int num = 1;
    while ((val /= 10) > 0)
    {
        num++;
    }
    return num;
}
