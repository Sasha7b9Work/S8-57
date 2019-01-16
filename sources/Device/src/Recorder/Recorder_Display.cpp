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
static void DrawSettings();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Display::Update()
{
    Painter::BeginScene(Color::BLACK);

    Grid::Draw();

    DrawSettings();

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSettings()
{
    Region(50, 30).DrawBounded(0, 0, Color::BACK, Color::FILL);

    Text(RECORDER_SCALE_X.ToString()).Draw(2, 2);
}
