#pragma once
#include "Display/Colors.h"


struct TypeConversionString
{
    enum E
    {
        None,           // Преобразование строки не производится
        FirstUpper      // Первый символ - в вернем регистре, остальные - в нижнем
    };
};


class String // -V690
{
public:
    explicit String();
             String(const String &);
    explicit String(char symbol);
    explicit String(const char *format, ...);
    ~String();

    void Set(TypeConversionString::E conv, const char *format, ...);

    char *CString() const;
    /// Отобразить текст на экране в заданнх координатах
    int Draw(int x, int y, Color color = Color::NUMBER) const;

    static const char * const ERROR;

    void Free();

    void Append(const char *str);
    /// Удаляет numSymbols из начала строки
    void RemoveFromBegin(uint numSymbols);

    uint Size() const;

    char &operator[](uint i);

private:

    bool Allocate(uint size);
    void Conversion(TypeConversionString::E conv);

    char *buffer;
};
