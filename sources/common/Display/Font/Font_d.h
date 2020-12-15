#pragma once


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
        _UGO, //-V2573
        _UGO2, //-V2573
        _GOST28, //-V2573
        _GOST72bold,    // Используется для вывода показаний мультиметра //-V2573
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
    
    // Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void Set(const DTypeFont::E typeFont);
    
    static DTypeFont::E Current();
    
    // Восстанавливает шрифт, бывший текущим перед последним вызовом SetCurrent()
    static void Pop();
    
    static bool IsBig();
    
    static uint8 GetWidth(uint8 symbol);
    
    static uint8 GetWidth(char symbol);
    
    static uint8 GetHeight();
    
    static bool RowNotEmpty(uint8 symbol, int row);
    
    static bool BitIsExist(uint8 symbol, int row, int bit);
    
    static void SetMinWidth(uint8 width);
    
    // Устанавливает количество пустых пикселей между символами.
    static void SetSpacing(int spacing = 1);
    
    // Возвращает количество пустых пикселей между символами
    static int GetSpacing();
};
