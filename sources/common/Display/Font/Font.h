#pragma once
#include "defines.h"


struct Symbol
{
    uchar width;
    uchar bytes[8];
};

struct TypeFont
{
    enum E
    {
        _5,
        _8,
        _UGO,
        _UGO2,
        _GOST28,
        _GOST72bold,
        Count,
        None
    } value;
};

struct Font
{
    int _height;
    Symbol symbols[256];

public:
    static int  GetLengthText(pString text);
    /// Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void Set(const TypeFont::E typeFont);
    static TypeFont::E Current();
    /// Восстанавливает шрифт, бывший текущим перед последним вызовом SetCurrent()
    static void Pop();
    static bool IsBig();
    static uint8 GetWidth(uint8 symbol);
    static uint8 GetWidth(char symbol);
    static uint8 GetHeight();
    static bool RowNotEmpty(uint8 symbol, int row);
    static bool BitIsExist(uint8 symbol, int row, int bit);
    static void SetMinWidth(uint8 width);
    /// Устанавливает количество пустых пикселей между символами.
    static void SetSpacing(int spacing);
    /// Возвращает количество пустых пикселей между символами
#ifndef PANEL
    static int GetSpacing();
#endif
};
