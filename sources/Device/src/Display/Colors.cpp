#include "defines.h"
#include "common/Transceiver.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


extern uint GlobalColors[256] =
{
    /* 0  */    MAKE_COLOR(0x00, 0x00, 0x00),       // BLACK
    /* 1  */    MAKE_COLOR(0xff, 0xff, 0xff),       // WHITE
    /* 2  */    MAKE_COLOR(0x60, 0x60, 0x60),       // GRID
    /* 3  */    MAKE_COLOR(0x00, 0xdf, 0xff),       // DATA_A
    /* 4  */    MAKE_COLOR(0x00, 0xff, 0x00),       // DATA_B
    /* 5  */    MAKE_COLOR(0x90, 0x90, 0xa0),       // MENU_FIELD
    /* 6  */    MAKE_COLOR(31 * 8, 44 * 4, 0  * 8), // MENU_TITLE
    /* 7  */    MAKE_COLOR(0x00, 0x00, 0x80),       // MENU_TITLE_DARK
    /* 8  */    MAKE_COLOR(31 * 8, 63 * 4, 0  * 8), // MENU_TITLE_BRIGHT
    /* 9  */    MAKE_COLOR(26 * 8, 34 * 4, 0  * 8), // 
    /* 10 */    MAKE_COLOR(13 * 8, 17 * 4, 0  * 8), // MENU_ITEM_DARK
    /* 11 */    MAKE_COLOR(31 * 8, 51 * 4, 0  * 8), // MENU_ITEM_BRIGHT
    /* 12 */    MAKE_COLOR(0x00, 13,   0x00),       // DATA_WHITE_ACCUM_A
    /* 13 */    MAKE_COLOR(0x00, 25,   0x00),       // DATA_WHITE_ACCUM_B
    /* 14 */    MAKE_COLOR(0xff, 0x00, 0x00),       // RED
    /* 15 */    MAKE_COLOR(0x00, 0xff, 0x00),       // GREEN
    /* 16 */    MAKE_COLOR(0x00, 0x00, 0xff),       // BLUE
    /* 17 */    MAKE_COLOR(0x00, 0x00, 0x40),       // BLUE_25
    /* 18 */    MAKE_COLOR(0x00, 0x00, 0x80),       // BLUE_50
    /* 19 */    MAKE_COLOR(0x1a, 0x1a, 0x1a),       // GRAY_10
    /* 20 */    MAKE_COLOR(0x33, 0x33, 0x33),       // GRAY_20
    /* 21 */    MAKE_COLOR(0x00, 0x00, 0x1a),       // BLUE_10
    /* 22 */    MAKE_COLOR(0x80, 0x80, 0x80),       // GRAY_50
    /* 23 */    MAKE_COLOR(0xc0, 0xc0, 0xc0),       // GRAY_75
    /* 24 */    MAKE_COLOR(0x00, 0x6f, 0x80),       // DATA_HALF_A
    /* 25 */    MAKE_COLOR(0x00, 0x80, 0x00),       // DATA_HALF_B
    /* 26 */    MAKE_COLOR(0xc0, 0xc0, 0xc0),       // SEPARATOR
    /* 27 */    MAKE_COLOR(0xff, 0xff, 0x00)        // YELLOW
};


Color Color::BLACK(COLOR_BLACK);
Color Color::WHITE(COLOR_WHITE);
Color Color::MENU_FIELD(COLOR_MENU_FIELD);
Color Color::MENU_TITLE_DARK(COLOR_MENU_TITLE_DARK);
Color Color::MENU_TITLE_BRIGHT(COLOR_MENU_ITEM_BRIGHT);
Color Color::MENU_ITEM_DARK(COLOR_MENU_ITEM_DARK);
Color Color::MENU_ITEM_BRIGHT(COLOR_MENU_ITEM_BRIGHT);
Color Color::DATA_WHITE_ACCUM_A(COLOR_DATA_WHITE_ACCUM_A);
Color Color::DATA_WHITE_ACCUM_B(COLOR_DATA_WHITE_ACCUM_B);
Color Color::RED(COLOR_RED);
Color Color::GREEN(COLOR_GREEN);
Color Color::BLUE(COLOR_BLUE);
Color Color::BLUE_25(COLOR_BLUE_25);
Color Color::BLUE_50(COLOR_BLUE_50);
Color Color::GRAY_10(COLOR_GRAY_10);
Color Color::GRAY_20(COLOR_GRAY_20);
Color Color::BLUE_10(COLOR_BLUE_10);
Color Color::GRAY_50(COLOR_GRAY_50);
Color Color::GRAY_75(COLOR_GRAY_75);
Color Color::DATA_HALF_A(COLOR_DATA_HALF_A);
Color Color::DATA_HALF_B(COLOR_DATA_HALF_B);
Color Color::SEPARATOR(COLOR_SEPARATOR);
Color Color::YELLOW(COLOR_YELLOW);

Color Color::NUMBER(COLOR_NUMBER);
Color Color::FLASH_10(COLOR_FLASH_10);
Color Color::FLASH_01(COLOR_FLASH_01);

Color Color::CHAN[4] = {Color(COLOR_DATA_A), Color(COLOR_DATA_B), Color(COLOR_WHITE), Color(COLOR_WHITE)};
Color Color::FILL(COLOR_WHITE);
Color Color::BACK(COLOR_BLACK);
Color Color::GRID(COLOR_GRID);


Color Color::currentColor = Color::NUMBER;
static bool  inverseColor = false;


/// Записывает мигающй цвет в дисплей. Возвращает false, если текущий цвет немигающий
static bool WriteFlashColor();
/// Записывает цвет в дисплей
static void WriteToDisplay(Color color);


void Color::Log(Color color)
{
#define colorVal (COLOR(color.value))

    LOG_WRITE("Color %d R=%d, G=%d, B=%d", color.value, R_FROM_COLOR(colorVal), G_FROM_COLOR(colorVal), B_FROM_COLOR(colorVal));
}


bool operator!=(const Color &left, const Color &right)
{
    return left.value != right.value;
}


bool operator==(const Color &left, const Color &right)
{
    return left.value == right.value;
}


bool operator>(const Color &left, const Color &right)
{
    return left.value > right.value;
}


void ColorType::Init()
{
    if (!inititalized)
    {
        ReInit();
        inititalized = true;
    }
}


void ColorType::ReInit()
{
    uint colorValue = COLOR(color.value);

    red = (float)R_FROM_COLOR(colorValue);
    green = (float)G_FROM_COLOR(colorValue);
    blue = (float)B_FROM_COLOR(colorValue);

    SetBrightness();

    if (Math::IsEquals(red, 0.0F) &&
        Math::IsEquals(green, 0.0F) &&
        Math::IsEquals(blue, 0.0F))
    {
        stepRed = 0.31F;
        stepGreen = 0.63F;
        stepBlue = 0.31F;
    }
}


void ColorType::SetBrightness(float bright)
{
    if (Math::IsEquals(bright, -1.0F))
    {
        brightness = Math::Max(red / 255.0F, green / 255.0F, blue / 255.0F);

        CalcSteps();
    }
    else
    {
        int delta = static_cast<int>((bright + 0.0005F) * 100.0F) - static_cast<int>(brightness * 100.0F);

        if (delta > 0)
        {
            for (int i = 0; i < delta; i++)
            {
                BrightnessChange(1);
            }
        }
        else
        {
            for (int i = 0; i < -delta; i++)
            {
                BrightnessChange(-1);
            }
        }
    }
}


/*
Алгоритм изменения яркости.
1. Инициализация.
а. Рассчитать яркость по принципу - яркость равна относительной интенсивности самого яркого цветового канала
б. Рассчитать шаг изменения цветовой составляющей каждого канала на 1% яркости по формуле:
Шаг = Интенсивность канала * Яркость, где Яркость - относительная величина общей яркости
в. Если интенсивность всех каналов == 0, то дать полный шаг каждому каналу
2. При изменнении яркости на 1% менять интенсивность каждого канала на Шаг, расчитанный в предыдущем пункте.
3. При изменения интенсивности цветового канала пересчитывать яркость и шаг изменения каждого канала.
*/


void ColorType::BrightnessChange(int delta) // -V2506
{
    if ((delta > 0 && brightness == 1.0F) || (delta < 0 && brightness == 0.0F)) // -V550 //-V2550 //-V550
    {
        return;
    }

    int sign = Math::Sign(delta);

    brightness += sign * 0.01F;
    LIMITATION(brightness, 0.0F, 1.0F);

    red += sign * stepRed;
    green += sign * stepGreen;
    blue += sign * stepBlue;

    SetColor();

    if (stepRed < 0.01F && stepGreen < 0.01F && stepBlue < 0.01F)
    {
        stepRed = 2.55F;
        stepGreen = 2.55F;
        stepBlue = 2.55F;
    }
}


void ColorType::CalcSteps()
{
    stepRed = red / (brightness * 100.0F);
    stepGreen = green / (brightness * 100.0F);
    stepBlue = blue / (brightness * 100.0F);
}


void ColorType::SetColor()
{
    LIMITATION(red, 0.0F, 255.0F);
    LIMITATION(green, 0.0F, 255.0F);
    LIMITATION(blue, 0.0F, 255.0F);

    COLOR(color.value) = MAKE_COLOR(static_cast<int>(red), static_cast<int>(green), static_cast<int>(blue));
    color.LoadValueRGB();
}


void ColorType::ComponentChange(int delta)
{
    static const float maxs[4] = {0.0F, 255.0F, 255.0F, 255.0F};
    float * const pointers[4] = {0, &blue, &green, &red};
    int8 index = currentField;

    if (index >= 1 && index <= 3)
    {
        *(pointers[index]) += static_cast<float>(Math::Sign(delta));
        Math::Limitation<float>(pointers[index], 0.0F, maxs[index]);
    }

    SetColor();

    SetBrightness();
}


Color& Color::operator=(const Color &color)
{
    value = color.value;
    return *this;
}


Color Color::Trig()
{
    Color result = Color::FILL;

    if(TrigSource() == Chan::A)
    {
        result = CHAN[Chan::A];
    }
    else // set.trig.source == TrigSource::B
    {
        result = CHAN[Chan::B];
    }

    return result;
}


Color Color::GetCurent()
{
    return currentColor;
}


static bool WriteFlashColor()
{
    bool result = false;

    if (Color::GetCurent() == Color::FLASH_01)
    {
        WriteToDisplay(inverseColor ? Color::FILL : Color::BACK);
        result = true;
    }
    if (Color::GetCurent() == Color::FLASH_10)
    {
        WriteToDisplay(inverseColor ? Color::BACK : Color::FILL);
        result = true;
    }

    return result;
}


static void WriteToDisplay(Color color)
{
    static Color lastColor = Color::NUMBER;

    if (color != lastColor)
    {
        lastColor = color;

        Transceiver::Send(Command::Paint_SetColor, lastColor.value);
    }
}


void Color::LoadValueRGB()
{
    uint rgb = COLOR(value);

    uint8 buffer[6] = 
    {
        Command::Paint_SetPalette,
        value,
        static_cast<uint8>(rgb),
        static_cast<uint8>(rgb >> 8),
        static_cast<uint8>(rgb >> 16),
        static_cast<uint8>(rgb >> 24)
    };

    Transceiver::Send(buffer, 6);
}


void Color::SetAsCurrent()
{
    if(value != Color::NUMBER.value)
    {
        currentColor = *this;
    }

    if(!WriteFlashColor())
    {
        WriteToDisplay(currentColor);
    }
}


void Color::ChangeFlash(bool reset)
{
    static uint time = 0;

    if (reset)
    {
        inverseColor = true;
        time = 0;
    }

    if (TIME_MS - time < 500)
    {
        return;
    }

    time = TIME_MS;

    inverseColor = !inverseColor;
    WriteFlashColor();
}
