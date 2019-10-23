#pragma once
#include "defines.h"
#include "Settings/SettingsTypes.h"



class Color
{
public:
    static Color BLACK;
    static Color WHITE;
    static Color MENU_FIELD;
    static Color MENU_TITLE_DARK;
    static Color MENU_TITLE_BRIGHT;
    static Color MENU_ITEM_DARK;
    static Color MENU_ITEM_BRIGHT;
    static Color DATA_WHITE_ACCUM_A;
    static Color DATA_WHITE_ACCUM_B;
    static Color NUMBER;
    static Color FLASH_10;
    static Color FLASH_01;
    static Color BLUE_10;
    static Color GREEN_10;
    static Color RED_10;
    static Color BLUE_25;
    static Color GREEN_25;
    static Color RED_25;
    static Color GRAY;
    static Color GRAY_25;
    static Color GRAY_10;
    static Color GRAY_20;
    static Color RED_50;
    static Color BLUE_50;
    static Color GREEN_50;

    static Color FILL;
    static Color BACK;
    static Color GRID;
    static Color CHAN[4];

    explicit Color(uint8 val = COLOR_BLACK) : value(val) { }
    Color(const Color &color) : value(color.value) { }

    static Color Cursors(Chan ch);
    static Color Trig();
    static Color ChanAccum(Chan ch);     ///< Цвет канала в режиме накопления
    static Color MenuItem(bool shade);      ///< Цвет элемента меню.
    static Color MenuTitle(bool shade);     ///< Цвет заголовка страницы. inShade == true, если страница затенена
    static Color BorderMenu(bool shade);    ///< Цвет окантовки меню
    static Color LightShadingText();        ///< Светлый цвет в тени.
    static Color Contrast(Color color);     ///< Возвращает цвет, контрастный к color. Может быть белым или чёрным.
    
    uint8 value;

    static void Log(Color color);

    Color& operator=(const Color &color);

    enum
    {
        COLOR_BLACK = 0,
        COLOR_WHITE = 1,
        COLOR_GRID = 2,
        COLOR_DATA_A = 3,
        COLOR_DATA_B = 4,
        COLOR_MENU_FIELD = 5,
        COLOR_MENU_TITLE = 6,
        COLOR_MENU_TITLE_DARK = 7,
        COLOR_MENU_TITLE_BRIGHT = 8,
        COLOR_MENU_ITEM = 9,
        COLOR_MENU_ITEM_DARK = 10,
        COLOR_MENU_ITEM_BRIGHT = 11,
        COLOR_DATA_WHITE_ACCUM_A = 12,   ///< Используется как для отрисовки канала на белом фоне, так и для отрисовки накопленных сигналов
        COLOR_DATA_WHITE_ACCUM_B = 13,
        COLOR_GRID_WHITE = 14,
        COLOR_EMPTY = 15,
        COLOR_FLASH_10 = 16,
        COLOR_FLASH_01 = 17,
        COLOR_INVERSE = 18,
        COLOR_GREEN_10 = 19,
        COLOR_BLUE_10 = 20,
        COLOR_RED_10 = 21,
        COLOR_GREEN_25 = 22,
        COLOR_BLUE_25 = 23,
        COLOR_RED_25 = 24,
        COLOR_GRAY = 25,
        COLOR_GRAY_25 = 26,
        COLOR_GRAY_10 = 27,
        COLOR_GRAY_20 = 28,
        COLOR_RED_50 = 29,
        COLOR_BLUE_50 = 30,
        COLOR_GREEN_50 = 31,
        COLOR_NUMBER = 32
    };
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable : 4623)
#endif

class ColorType
{
public:
    float   red;
    float   green;
    float   blue;
    float   stepRed;
    float   stepGreen;
    float   stepBlue;
    float   brightness;
    Color   color;
    int8    currentField;
    bool    alreadyUsed;
    bool    notUsed;

    /// Если forced == false, то инициализация происходит только при первом вызове функции
    void Init(bool forced);
    void SetBrightness(float brightness = -1.0F);
    void BrightnessChange(int delta);
    void ComponentChange(int delta);
private:
    void CalcSteps();
    void SetColor();
};

#define DEF_COLOR_TYPE(name, r, g, b, sR, sG, sB, bright, col) ColorType name = {r, g, b, sR, sG, sB, bright, col, 0, false, false};
#define COLOR_TYPE(r, g, b, sR, sG, sB, bright, col) {r, g, b, sR, sG, sB, bright, col, 0, false, false};

#ifdef _WIN32
#pragma warning(pop)
#endif

#define MAKE_COLOR(r, g, b) (static_cast<uint>(static_cast<uint>(b) + (static_cast<uint>(g) << 8) + (static_cast<uint>(r) << 16)))
#define R_FROM_COLOR(color) ((static_cast<uint>(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) ((static_cast<uint>(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) ((static_cast<uint>(color))       & 0xff)

extern uint colors[256];

#define COLOR(x) colors[x]
