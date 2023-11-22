// 2023/11/22 21:13:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};


namespace SU
{
    // Эта команда сразу преобразует к верхенму регистру слово.
    bool GetWord(const char *string, Word *word, const int numWord);

    bool WordEqualZeroString(Word *word, char* string);

    char *Db2String(float value, int numDigits, char bufferOut[20]);
    // Сравнивает две строки. Число символов берётся из str1
    bool EqualsStrings(const char *str1, const char *str2);

    bool EqualsStrings(void *str1, void *str2, int size);

    bool EqualsStrings(uint8 *str1, const char * const str2, int size);

    bool EqualsZeroStrings(char *str1, char *str2);
    // Возвращает позицию первого символа, который не является цифрой либо точкой
    int FirstNotNumeral(char *buffer);
    // Преобразует строку в float. Обрабатываются символы до первого нецифрового символа
    float StringToFloat(const char *string);

    char *ToUpper(char *str, int size);

    char *ToUpper(char *str);

    char ToUpper(char symbol);

    char ToLower(char symbol);

    char *DoubleToString(double value);

    bool StringToDouble(double *value, const char *buffer);

    bool String2Int(const char *buffer, int *value, char **end);
};


#ifndef GUI

int strcpy_s(char *dest, uint dest_size, const char *src);

#endif
