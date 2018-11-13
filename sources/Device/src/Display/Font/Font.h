#pragma once

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
            Number,
            None
        } value;
    };

    int height;
    Symbol symbol[256];

    static int  GetSize();
    static int  GetLengthText(const char *text);
    static int  GetHeightSymbol(char symbol);
    static int  GetLengthSymbol(char symbol);
    static void SetFont(Font::Type::E typeFont);
};


extern const Font *font;
extern const Font *fonts[Font::Type::Number];

extern const uchar font5display[3080];
extern const uchar font8display[3080];
extern const uchar fontUGOdisplay[3080];
extern const uchar fontUGO2display[3080];
