#pragma once
#include "Settings/SettingsChannel.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Color
{
private:
    static Color MENU_FIELD;

public:
    static Color BLACK;
    static Color WHITE;
    static Color GRID;
    static Color MENU_TITLE;
    static Color MENU_TITLE_DARK;
    static Color MENU_TITLE_BRIGHT;
    static Color MENU_ITEM_DARK;
    static Color MENU_ITEM_BRIGHT;
    static Color DATA_WHITE_ACCUM_A;
    static Color DATA_WHITE_ACCUM_B;
    static Color RED;
    static Color GREEN;
    static Color BLUE;
    static Color BLUE_25;
    static Color BLUE_50;
    static Color GRAY_10;
    static Color GRAY_20;
    static Color BLUE_10;
    static Color GRAY_50;
    static Color GRAY_75;
    static Color DATA_HALF_A;
    static Color DATA_HALF_B;
    static Color SEPARATOR;
    static Color YELLOW;

    static Color NUMBER;

    static Color FILL;
    static Color BACK;
    static Color FLASH_10;
    static Color FLASH_01;

    explicit Color(uint8 val) : value(val) { }
    Color(const Color &color) : value(color.value) { }

    static Color Channel(Chan::E ch);
    static Color Cursors(Chan::E ch);
    static Color Trig();
    static Color MenuItem();        ///< Цвет элемента меню.
    static Color MenuItemField();   ///< Цвет пункта меню с выбранным значением.
    static Color MenuTitleText();   ///< Цвет заголовка страницы или элемента меню.
    /// Устанавливает текущий цвет отрисовки
    void SetAsCurrent();
    static void SetCurrent(Color color);
    /// Возвращает текущий цвет отрисовки
    static Color GetCurent();

    static void ResetFlash();
    /// Это системная функция. Вызывать самому нельзя
    static void OnTimerFlashDisplay();

    void SetValue(uint value);
    
    uint8 value;

    static void InitGlobalColors();
    static void Log(Color color);

    Color& operator=(const Color &color);

private:

    static Color CHAN[4];
    
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
        _NONE = 9,
        COLOR_MENU_ITEM_DARK = 10,
        COLOR_MENU_ITEM_BRIGHT = 11,
        COLOR_DATA_WHITE_ACCUM_A = 12,   ///< Используется как для отрисовки канала на белом фоне, так и для отрисовки накопленных сигналов
        COLOR_DATA_WHITE_ACCUM_B = 13,
        COLOR_RED = 14,
        COLOR_GREEN = 15,
        COLOR_BLUE = 16,
        COLOR_BLUE_25 = 17,
        COLOR_BLUE_50 = 18,
        COLOR_GRAY_10 = 19,
        COLOR_GRAY_20 = 20,
        COLOR_BLUE_10 = 21,
        COLOR_GRAY_50 = 22,
        COLOR_GRAY_75 = 23,
        COLOR_DATA_HALF_A = 24,
        COLOR_DATA_HALF_B = 25,
        COLOR_SEPARATOR = 26,
        COLOR_YELLOW = 27,

        COLOR_NUMBER = 32,

        COLOR_FLASH_10 = 33,
        COLOR_FLASH_01 = 34,
        COLOR_INVERSE = 35
    };

    /// Записывает мигающй цвет в дисплей. Возвращает false, если текущий цвет немигающий
    static bool WriteFlashColor();
    /// Записывает цвет в дисплей
    static void WriteToDisplay(Color color);

    static Color currentColor;

public:

    struct Scheme
    {
        enum E
        {
            WhiteLetters,   ///< В этом случае заголовки элементов меню пишутся белым - не очень хорошо видно снизу
            BlackLetters    ///< В этом случае заголовки элементов меню пишутся чёрным - не очень красиво выглядит
        } value;
        explicit Scheme(E v) : value(v) {}
    };
};

bool operator!=(const Color &left, const Color &right);
bool operator==(const Color &left, const Color &right);
bool operator>(const Color &left, const Color &right);


/// Структура используется для визуальной настройки цвета
class ColorType
{
public:
    /// Красная составляющая цвета [0.0...1.0]
    float   red;
    /// Зелёная составляющая цвета [0.0...1.0]
    float   green;
    /// Синяя составляющая цвета [0.0...1.0]
    float   blue;
    /// Шаг изменения красной составляющей цвета
    float   stepRed;
    /// Шаг изменения зелёной составляющей цвета
    float   stepGreen;
    /// Шаг изменения синей составляющей цвета
    float   stepBlue;
    /// Яркость [0...1]
    float   brightness;
    Color   color;
    int8    currentField;
    /// true, если структура уже инициализирована
    bool    inititalized;

    /// Инициализация. При повторном вызове не работает. При повторных вызовах вызывать ReInit()
    void Init();
    void ReInit();
    void SetBrightness(float brightness = -1.0F);
    void BrightnessChange(int delta);
    void ComponentChange(int delta);
private:
    void CalcSteps();
    void SetColor();
};

#define DEF_COLOR_TYPE(name, r, g, b, sR, sG, sB, bright, col) ColorType name = {r, g, b, sR, sG, sB, bright, col, 0, false, false};
#define COLOR_TYPE(r, g, b, sR, sG, sB, bright, col) {r, g, b, sR, sG, sB, bright, col, 0, false};


#define MAKE_COLOR(r, g, b) ((uint)((b) + ((g) << 8) + ((r) << 16)))
#define R_FROM_COLOR(color) (((uint)(color) >> 16) & 0xff)
#define G_FROM_COLOR(color) (((uint)(color) >> 8)  & 0xff)
#define B_FROM_COLOR(color) (((uint)(color))       & 0xff)

#define COLOR(x) GlobalColors[x]

extern uint GlobalColors[256];
