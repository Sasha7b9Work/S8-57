#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "Display/Painter.h"
#include "Hardware/FSMC.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cmath>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern uint GlobalColors[256] =
{
    /* 0  */    MAKE_COLOR(0x00, 0x00, 0x00),       // BLACK
    /* 1  */    MAKE_COLOR(0xff, 0xff, 0xff),       // WHITE
    /* 2  */    MAKE_COLOR(0x60, 0x60, 0x60),       // GRID
    /* 3  */    MAKE_COLOR(0x00, 0xdf, 0xff),       // DATA_A
    /* 4  */    MAKE_COLOR(0x00, 0xff, 0x00),       // DATA_B
    /* 5  */    MAKE_COLOR(0x90, 0x90, 0xa0),       // MENU_FIELD
    /* 6  */    MAKE_COLOR(31 * 8, 44 * 4, 0  * 8), // MENU_TITLE
    /* 7  */    MAKE_COLOR(0x00, 0x00, 0x40),       // MENU_TITLE_DARK
    /* 8  */    MAKE_COLOR(31 * 8, 63 * 4, 0  * 8), // MENU_TITLE_BRIGHT
    /* 9  */    MAKE_COLOR(26 * 8, 34 * 4, 0  * 8), // MENU_ITEM
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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


static Color currentColor = Color::NUMBER;
static bool  inverseColor = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Color::InitGlobalColors()
{
    Color::BACK.value = BACKGROUND_BLACK ? Color::BLACK.value : Color::WHITE.value;
    Color::FILL.value = BACKGROUND_BLACK ? Color::WHITE.value : Color::BLACK.value;
    Color::CHAN[Chan::A].value = BACKGROUND_BLACK ? Color::CHAN[Chan::A].value : Color::DATA_WHITE_ACCUM_A.value;
    Color::CHAN[Chan::B].value = BACKGROUND_BLACK ? Color::CHAN[Chan::B].value : Color::DATA_WHITE_ACCUM_B.value;
    //Color::CHAN[A_B].value = Color::CHAN[MathCh].value = BACKGROUND_BLACK ? Color::WHITE.value : Color::BLACK.value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Color::Log(Color color)
{
#define colorVal (COLOR(color.value))

    LOG_WRITE("Color %d R=%d, G=%d, B=%d", color.value, R_FROM_COLOR(colorVal), G_FROM_COLOR(colorVal), B_FROM_COLOR(colorVal));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::Cursors(Chan::E ch)
{
    return CHAN[ch];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::BorderMenu(bool /*shade*/)
{
    return Color::FILL;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::MenuTitle(bool shade)
{
    return shade ? Color::GRAY_10 : Color::BLUE_10;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::LightShadingText()
{
    return MenuTitle(false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::MenuItem(bool shade)
{
    return MenuTitle(shade);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::Contrast(Color color) // -V2506
{
    uint colorValue = COLOR(color.value);

#define HALF_RED   16
#define HALF_GREEN 32
#define HALF_BLUE  16

    if (R_FROM_COLOR(colorValue) > HALF_RED ||
        G_FROM_COLOR(colorValue) > HALF_GREEN ||
        B_FROM_COLOR(colorValue) > HALF_BLUE)
    {
        return Color(COLOR_BLACK);
    }
    return Color(COLOR_WHITE);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool operator!=(const Color &left, const Color &right)
{
    return left.value != right.value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool operator==(const Color &left, const Color &right)
{
    return left.value == right.value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool operator>(const Color &left, const Color &right)
{
    return left.value > right.value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ColorType::Init(bool forced)
{
    if (forced)
    {
        alreadyUsed = false;
    }

    if (!alreadyUsed)
    {
        alreadyUsed = true;                  // ѕризнак того, что начальные установки уже произведены

        uint colorValue = COLOR(color.value);

        red = (float)R_FROM_COLOR(colorValue);
        green = (float)G_FROM_COLOR(colorValue);
        blue = (float)B_FROM_COLOR(colorValue);

        SetBrightness();
        
        if (red == 0.0F && green == 0.0F && blue == 0.0F)   // -V550
        {
            stepRed = 0.31F;
            stepGreen = 0.63F;
            stepBlue = 0.31F;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ColorType::SetBrightness(float bright)
{
    if (Math::IsEquals(bright, -1.0F))
    {
        brightness = Math::Max(red / 31.0F, green / 63.0F, blue / 31.0F);

        CalcSteps();
    }
    else
    {
        int delta = (int)((bright + 0.0005F) * 100.0F) - (int)(brightness * 100.0F);

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
јлгоритм изменени€ €ркости.
1. »нициализаци€.
а. –ассчитать €ркость по принципу - €ркость равна относительной интенсивности самого €ркого цветового канала
б. –ассчитать шаг изменени€ цветовой составл€ющей каждого канала на 1% €ркости по формуле:
Ўаг = »нтенсивность канала * яркость, где яркость - относительна€ величина общей €ркости
в. ≈сли интенсивность всех каналов == 0, то дать полный шаг каждому каналу
2. ѕри изменнении €ркости на 1% мен€ть интенсивность каждого канала на Ўаг, расчитанный в предыдущем пункте.
3. ѕри изменени€ интенсивности цветового канала пересчитывать €ркость и шаг изменени€ каждого канала.
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ColorType::BrightnessChange(int delta) // -V2506
{
    if ((delta > 0 && brightness == 1.0F) || (delta < 0 && brightness == 0.0F)) // -V550
    {
        return;
    }

    int sign = Math::Sign(delta);

    brightness += sign * 0.01F;
    LIMITATION(brightness, 0.0F, 1.0F); // -V2516

    red += sign * stepRed;
    green += sign * stepGreen;
    blue += sign * stepBlue;

    SetColor();

    if (stepRed < 0.01F && stepGreen < 0.01F && stepBlue < 0.01F)
    {
        stepRed = 0.31F;
        stepGreen = 0.63F;
        stepBlue = 0.31F;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ColorType::CalcSteps()
{
    stepRed = red / (brightness * 100.0F);
    stepGreen = green / (brightness * 100.0F);
    stepBlue = blue / (brightness * 100.0F);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ColorType::SetColor()
{
    COLOR(color.value) = MAKE_COLOR((int)red, (int)green, (int)blue);
    Painter::SetColorValue(color, COLOR(color.value));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ColorType::ComponentChange(int delta)
{
    static const float maxs[4] = {0.0F, 31.0F, 63.0F, 31.0F};
    float * const pointers[4] = {0, &blue, &green, &red};
    int8 index = currentField;

    if (index >= 1 && index <= 3)
    {
        *(pointers[index]) += (float)Math::Sign(delta);
        Math::Limitation<float>(pointers[index], 0.0F, maxs[index]);
    }

    SetColor();

    SetBrightness();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color& Color::operator=(const Color &color)
{
    value = color.value;
    return *this;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::Channel(Chan::E ch)
{
    return Color::CHAN[ch];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::ChanHalf(Chan::E ch)
{
    return Chan(ch).IsA() ? Color::DATA_HALF_A : Color::DATA_HALF_B;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::Trig()
{
    Color result = Color::FILL;

    if(TRIG_SOURCE_IS_A)
    {
        result = Channel(Chan::A);
    }
    else if(TRIG_SOURCE_IS_B)
    {
        result = Channel(Chan::B);
    }
    else
    {
        // нет действий
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::ChanAccum(Chan ch)
{
    return ch.IsA() ? Color(COLOR_DATA_WHITE_ACCUM_A) : Color(COLOR_DATA_WHITE_ACCUM_B);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Color::SetCurrent(Color color)
{
    currentColor = color;
    if (!WriteFlashColor())
    {
        WriteToDisplay(currentColor);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Color::GetCurent()
{
    return currentColor;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Color::WriteFlashColor()
{
    if (currentColor == Color::FLASH_01)
    {
        WriteToDisplay(inverseColor ? Color::FILL : Color::BACK);
        return true;
    }
    if (currentColor == Color::FLASH_10)
    {
        WriteToDisplay(inverseColor ? Color::BACK : Color::FILL);
        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Color::ResetFlash()
{
    Timer::SetAndEnable(Timer::Type::FlashDisplay, OnTimerFlashDisplay, 500);
    inverseColor = false;
    WriteFlashColor();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Color::OnTimerFlashDisplay()
{
    inverseColor = !inverseColor;
    WriteFlashColor();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Color::WriteToDisplay(Color color)
{
    static Color lastColor = Color::NUMBER;

    if (color != lastColor)
    {
        lastColor = color;
        FSMC::WriteToPanel2bytes(Command::Paint_SetColor, lastColor.value);
    }
}
