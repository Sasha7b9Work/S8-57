#pragma once
#include "defines.h"


struct DSymbol
{
    uchar width;
    uchar bytes[8];
};

struct DTypeFont
{
    enum E
    {
        _5,
        _8,
        _UGO,
        _UGO2,
        _GOST28,
        _GOST72bold,    // Используется для вывода показаний мультиметра
        _OMEGA72,       // Используется в мультиметре для вывода обозначения ом. Имеет только один символ
        Count,
        None
    };
};

struct DFont
{
    int _height;
    DSymbol symbols[256];

public:
    static int  GetLengthText(pString text);
    /// Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void Set(const DTypeFont::E typeFont);
    static DTypeFont::E Current();
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
    static void SetSpacing(int spacing = 1);
    /// Возвращает количество пустых пикселей между символами
    static int GetSpacing();
};


struct WorkerLengthText
{
    static int Run(pString text);
    static void SetLength(uint8);
private:
    /// Используется для приёма длины текста от панели
    static int recvLength;
};
