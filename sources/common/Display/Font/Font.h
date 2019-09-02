#pragma once
#include "defines.h"


struct BigSymbol
{
    uint8 code;     ///< ASCII-код символа
    uint8 width;    ///< Ширина символа в битах
    uint16 offset;  ///< Смещение первого байта символа относительно начала массива данных
};

struct FullSymbol
{
    BigSymbol symbol;
    uint8     *offset;  /// Указать на первый байт глифа
    bool RowNotEmpty(int row) const;
    bool BitIsExist(int row, int bit) const;
private:
    /// Возвращает указатель на первый байт строки
    uint8 *GetRow(int row) const;
    uint8 BytesInRow() const;
};

struct BigFont
{
    uint8           height;       ///< Высота ссимволов
    uint8           numSymbols;   ///< Число символов в шрифте
    const uint8     *data;        ///< Массив данных символов
    const BigSymbol *symbols;     ///< Ссылка на начало массива символов

    uint8 GetWidth(uint8 symbol) const;
    bool GetFullSymbol(FullSymbol &symbol, uint8 code) const;
};


struct Symbol
{
    uchar width;
    uchar bytes[8];
};

struct Font
{
    struct Type
    {
        enum E
        {
            _5,
            _8,
            _UGO,
            _UGO2,
            _Big64,
            Size,
            None
        } value;
    };

    int _height;
    Symbol symbols[256];

public:
    static int  GetLengthText(pString text);
    static int  GetHeightSymbol(char symbol);
    static int  GetLengthSymbol(char symbol);
    /// Устанавливает текущий шрифт. Ранее установленный можно восстановить функцией Pop()
    static void SetCurrent(Font::Type::E typeFont);
    /// Восстанавливает шрифт, бывший текущим перед последним вызовом SetCurrent()
    static void Pop();
    static bool IsBig();
    static uint8 GetWidth(uint8 symbol);
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


extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];
extern const BigFont fontDigits64;
