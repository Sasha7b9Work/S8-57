#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "log.h"
#include "device.h"
#include "Grid.h"
#include "Display/Display_Primitives.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include "Menu/Menu.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::HLine;
using Display::VLine;
using Display::Rectangle;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Grid
{
    /// Нарисовать сетку для режима осциллографа
    static void DrawOsci();
    /// Нарисовать сетку для режима тестер-компонента
    static void DrawTester();

    static void DrawGridSignal(int left, int top, int width, int height);

    static void DrawGridSpectrum();
    /// Возвращает расстояние между 
    static int  DeltaHforLineGrid();

    static int  DeltaVforLineGrid();

    static void DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY);

    static void DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY);

    static void DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX);

    static float DeltaY();

    static float DeltaX();

    static int MathTop();
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Grid::Left()
{
    static const int left[Device::Mode::Number] = { 20, 0, 0, 0 };

    return left[Device::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Top()
{
    static const int top[Device::Mode::Number] = { 19, 0, 0, 0 };

    return top[Device::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Width()
{
    static const int width[Device::Mode::Number] = { 280, Display::WIDTH - 1, 0, 0 };

    return width[Device::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Height()
{
    static const int height[Device::Mode::Number] = { 200, Display::HEIGHT - 1, 0, 0 };

    return height[Device::CurrentMode()];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Bottom()
{
    return Top() + Height();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::FullBottom()
{
    return Bottom();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::Right()
{
    return Left() + Width();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Grid::MathTop()
{
    return Top();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::MathBottom()
{
    return FullBottom();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::MathHeight()
{
    return Height();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::ChannelBottom()
{
    return (Display::IsSeparate()) ? (Top() + Height() / 2) : FullBottom();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Grid::ChannelCenterHeight()
{
    return (Top() + Bottom()) / 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::Draw()
{
    static const struct StructDraw { pFuncVV func; } draw[Device::Mode::Number] =
    {
        DrawOsci,
        DrawTester,
        EmptyFuncVV,
        EmptyFuncVV
    };


    pFuncVV func = draw[Device::CurrentMode()].func;

    if (func)
    {
        func();
    }
    else
    {
        LOG_ERROR("Нету обработчика");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawOsci()
{
    if (Display::IsSeparate())
    {
        DrawGridSignal(Left(), Top(), Width(), Height() / 2);

        if (FFT_ENABLED)
        {
            DrawGridSpectrum();
        }

        if (FUNC_MODE_DRAW_IS_ENABLED)
        {
            DrawGridSignal(Left(), Top() + Height() / 2, Width(), Height() / 2);
        }

		HLine(Width()).Draw(Left(), Top() + Height() / 2, Color::FILL);
    }
    else
    {
        DrawGridSignal(Left(), Top(), Width(), Height());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridSignal(int left, int top, int width, int height)
{
    int right = left + width;
    int bottom = top + height;

    Painter::SetColor(Color::FILL);

    if (top == Top())
    {
        // Painter::DrawHLine(top, 1, left - 2);
		HLine(left - 1).Draw(1, top);

        // Painter::DrawHLine(top, right + 2, Display::WIDTH - 2);
		HLine(Display::WIDTH - right - 4).Draw(right + 2, top);

        if (!Menu::IsMinimize() || !Menu::IsShown())
        {
            VLine line(bottom - top - 4);

            //Painter::DrawVLine(1, top + 2, bottom - 2);
            line.Draw(1, top + 2);

            //Painter::DrawVLine(318, top + 2, bottom - 2);
            line.Draw(318, top + 2);
        }
    }

    float deltaX = DeltaX() * (float)width / width;
    float deltaY = DeltaY() * (float)height / height;
    float stepX = deltaX / 5;
    float stepY = deltaY / 5;

    float centerX = (float)(left + width / 2);
    float centerY = (float)(top + height / 2);

    Painter::SetColor(Color::GRID);

    if (TYPE_GRID_1)
    {
        DrawGridType1(left, top, right, bottom, centerX, centerY, deltaX, deltaY, stepX, stepY);
    }
    else if (TYPE_GRID_2)
    {
        DrawGridType2(left, top, right, bottom, (int)deltaX, (int)deltaY, (int)stepX, (int)stepY);
    }
    else if (TYPE_GRID_3)
    {
        DrawGridType3(left, top, right, bottom, (int)centerX, (int)centerY, (int)deltaX, (int)deltaY, (int)stepX);
    }
    else
    {
        // для других типов сетки ничего делать не нужно
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Grid::DrawGridSpectrum()
{
    if (SCALE_FFT_IS_LOG)
    {
        static const int nums[] = {4, 6, 8};
        static pString strs[] = {"0", "-10", "-20", "-30", "-40", "-50", "-60", "-70"};
        int numParts = nums[MAX_DB_FFT];
        float scale = (float)MathHeight() / numParts;
        for (int i = 1; i < numParts; i++)
        {
            int y = MathTop() + (int)(i * scale);

            //Painter::DrawHLine(y, Left(), Left() + 256, Color::GRID);
			HLine(256).Draw(Left(), y, Color::GRID);

            if (!Menu::IsMinimize())
            {
                Painter::SetColor(Color::FILL);
                String((char *)strs[i]).Draw(3, y - 4);
            }
        }
        if (!Menu::IsMinimize())
        {
            Painter::SetColor(Color::FILL);
            String("дБ").Draw(5, MathTop() + 1);
        }
    }
    else // SCALE_FFT_IS_LINEAR
    {
        static pString strs[] = {"1.0", "0.8", "0.6", "0.4", "0.2"};
        float scale = (float)MathHeight() / 5;
        for (int i = 1; i < 5; i++)
        {
            int y = MathTop() + (int)(i * scale);

            //Painter::DrawHLine(y, Left(), Left() + 256, Color::GRID);
			HLine(256).Draw(Left(), y, Color::GRID);

            if (!Menu::IsMinimize())
            {
                String((char *)strs[i]).Draw(5, y - 4, Color::FILL);
            }
        }
    }

    //Painter::DrawVLine(Left() + 256, MathTop(), MathBottom(), Color::FILL);
    VLine(MathBottom() - MathTop()).Draw(Left() + 256, MathTop(), Color::FILL);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float Grid::DeltaY()
{
    float delta = (FullBottom() - Top()) / 10.0F;
    return Display::IsSeparate() ? (delta / 2.0F) : delta;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float Grid::DeltaX()
{
    float delta = (Right() - Left()) / 14.0F;
    return delta;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawGridType1(int left, int top, int right, int bottom, float centerX, float centerY, float deltaX, float deltaY, float stepX, float stepY)
{
    uint16 masX[17];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 7; i++)
    {
        masX[i] = (uint16)(left + (int)(deltaX * i));
    }
    for (int i = 7; i < 10; i++)
    {
        masX[i] = (uint16)((int)centerX - 8 + i);
    }
    for (int i = 10; i < 16; i++)
    {
        masX[i] = (uint16)(centerX + deltaX * (i - 9)); //-V2004
    }
    masX[16] = (uint16)(right - 1);

    Painter::DrawMultiVPointLine(17, top + (int)stepY, masX, (int)stepY, DeltaVforLineGrid(), Color::GRID);

    uint8 mas[13];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 5; i++)
    {
        mas[i] = (uint8)(top + (int)(deltaY * i));
    }
    for (int i = 5; i < 8; i++)
    {
        mas[i] = (uint8)((int)(centerY)-6 + i);
    }
    for (int i = 8; i < 12; i++)
    {
        mas[i] = (uint8)((int)centerY + (int)(deltaY * (i - 7)));
    }
    mas[12] = (uint8)(bottom - 1);

    Painter::DrawMultiHPointLine(13, left + (int)stepX, mas, (int)stepX, DeltaHforLineGrid(), Color::GRID);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawGridType2(int left, int top, int right, int bottom, int deltaX, int deltaY, int stepX, int stepY)
{
    uint16 masX[15];
    masX[0] = (uint16)(left + 1);
    for (int i = 1; i < 14; i++)
    {
        masX[i] = (uint16)(left + (int)(deltaX * i));
    }
    masX[14] = (uint16)(right - 1);
    Painter::DrawMultiVPointLine(15, top + stepY, masX, stepY, DeltaVforLineGrid(), Color::GRID);

    uint8 mas[11];
    mas[0] = (uint8)(top + 1);
    for (int i = 1; i < 10; i++)
    {
        mas[i] = (uint8)(top + (int)(deltaY * i));
    }
    mas[10] = (uint8)(bottom - 1);
    Painter::DrawMultiHPointLine(11, left + stepX, mas, stepX, DeltaHforLineGrid(), Color::GRID);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawGridType3(int left, int top, int right, int bottom, int centerX, int centerY, int deltaX, int deltaY, int stepX)
{
    Painter::DrawHPointLine(centerY, left + stepX, right, (float)stepX);
    uint8 masY[6] = {(uint8)(top + 1), (uint8)(top + 2), (uint8)(centerY - 1), (uint8)(centerY + 1), (uint8)(bottom - 2), (uint8)(bottom - 1)};
    Painter::DrawMultiHPointLine(6, left + deltaX, masY, deltaX, (right - top) / deltaX, Color::GRID);
    Painter::SetColor(Color::GRID);
    Painter::DrawVPointLine(centerX, top + stepX, bottom - stepX, (float)stepX);
    uint16 masX[6] = {(uint16)(left + 1), (uint16)(left + 2), (uint16)(centerX - 1), (uint16)(centerX + 1), (uint16)(right - 2), (uint16)(right - 1)};
    Painter::DrawMultiVPointLine(6, top + deltaY, masX, deltaY, (bottom - top) / deltaY, Color::GRID);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Grid::DeltaVforLineGrid()
{
    if (SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_1_5)
        {
            return VIEW_MEASURES_BOTH ? 55 : 59;
        }
        if (NUM_MEASURES_IS_2_5)
        {
            return VIEW_MEASURES_BOTH ? 69 : 51;
        }
        if (NUM_MEASURES_IS_3_5)
        {
            return VIEW_MEASURES_BOTH ? 54 : 68;
        }
    }

    return 49;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Grid::DeltaHforLineGrid()
{
    if (MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if (NUM_MEASURES_IS_6_1)
        {
            return 73;
        }
        if (NUM_MEASURES_IS_6_2)
        {
            return 83;
        }
    }
    return 69;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Grid::DrawTester()
{
    Painter::SetColor(Color::FILL);

    Rectangle(Display::WIDTH - 1, Display::HEIGHT - 1).Draw(0, 0);

    float x0 = 0;

    float y0 = 0;

    Painter::SetColor(Color::GRID);

    int x = (int)(x0 + Display::WIDTH / 2);
    int y = (int)(y0 + Display::HEIGHT / 2);

    //Painter::DrawVLine(x, 0, Display::HEIGHT);
    VLine(Display::HEIGHT).Draw(x, 0);

    //Painter::DrawHLine(y, 0, Display::WIDTH);
	HLine(Display::WIDTH).Draw(0, y);

    Painter::SetColor(Color::GRID);

    int deltaX = 32;
    int deltaY = 24;

    x += deltaX;

    float deltaPoint = 5.0F;

    while (x < Display::WIDTH)
    {
        Painter::DrawVPointLine(x, 0, Display::HEIGHT, deltaPoint);
        x += deltaX;
    }

    x = (int)(x0 + Display::WIDTH / 2 - deltaX);

    while (x > 0)
    {
        Painter::DrawVPointLine(x, 0, Display::HEIGHT, deltaPoint);
        x -= deltaX;
    }

    y += deltaY;

    while (y < Display::HEIGHT)
    {
        Painter::DrawHPointLine(y, 0, Display::WIDTH, deltaPoint);
        y += deltaY;
    }

    y = (int)(y0 + Display::HEIGHT / 2 - deltaY);

    while (y > 0)
    {
        Painter::DrawHPointLine(y, 0, Display::WIDTH, deltaPoint);
        y -= deltaY;
    }
}
