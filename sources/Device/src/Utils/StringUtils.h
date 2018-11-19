#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Word
{
    char   *address;
    int8    numSymbols;
    uint8   notUsed0;
    uint8   notUsed1;
    uint8   notUsed2;
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class SU
{
public:
    /// Эта команда сразу преобразует к верхенму регистру слово.
    static bool GetWord(const char *string, Word *word, const int numWord);

    static bool WordEqualZeroString(Word *word, char* string);

    static char *Db2String(float value, int numDigits, char bufferOut[20]);
    /// Сравнивает две строки. Число символов берётся из str1
    static bool EqualsStrings(char *str1, char *str2);

    static bool EqualsStrings(char *str1, char *str2, int size);

    static bool EqualsZeroStrings(char *str1, char *str2);

private:
    /// Возвращает false, если выбор невозможен - строка кончилась.
    static bool ChooseSymbols(const char **string);
    /// Возвращает false, если выбор невозможен - строка кончилась.
    static bool ChooseSpaces(const char **string);
};
