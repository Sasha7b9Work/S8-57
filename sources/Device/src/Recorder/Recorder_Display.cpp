#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Settings.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Menu/Menu.h"
#endif


using namespace Display::Primitives;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Изобразить установленные настройки
static void DrawSettings(int x, int y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Display::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    DrawSettings(270, 0);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSettings(int x, int y)
{
    Region(50, 30).DrawBounded(x, y, Color::BACK, Color::FILL);

    Text(RECORDER_SCALE_X.ToString()).Draw(x + 2, y + 2);
}
