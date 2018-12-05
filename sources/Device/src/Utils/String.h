#pragma once
#include "Display/Colors.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class String
{
public:
    String();
    String(const String &);
    String(char symbol);
    String(const char *format, ...);
    ~String();

    char *CString() const;
    /// Отобразить текст на экране в заданнх координатах
    int Draw(int x, int y, Color color = Color::NUMBER) const;

private:

    bool Allocate(uint size);
    void Free();

    char *buffer;
};
