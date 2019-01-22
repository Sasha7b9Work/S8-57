#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageGrid;

const PageBase *PageDisplay::PageGrid::pointer = &pageGrid;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_CHOICE_4( cType, // -V206                                                                                                                                 //--- ДИСПЛЕЙ - СЕТКА - Тип ---
    "Тип", "Type",
    "Выбор типа сетки.",
    "Choice like Grid::",
    "Тип 1", "Type 1",
    "Тип 2", "Type 2",
    "Тип 3", "Type 3",
    "Тип 4", "Type 4",
    TYPE_GRID, pageGrid, FuncActive, Choice::EmptyChange, FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Grid_Brightness()
{
    PageDisplay::PageSettings::PageColors::colorTypeGrid.SetBrightness(BRIGHTNESS_GRID / 100.0F);
}

static void BeforeDraw_Grid_Brightness()
{
    PageDisplay::PageSettings::PageColors::colorTypeGrid.Init();
    //Grid::SetBrightness((int)(PageDisplay::PageSettings::PageColors::colorTypeGrid.brightness * 100.0F));
}

DEF_GOVERNOR( gBrightness,                                                                                                                                //--- ДИСПЛЕЙ - СЕТКА - Яркость ---
    "Яркость", "Brightness",
    "Устанавливает яркость сетки",
    "Adjust the brightness of the Grid",
    BRIGHTNESS_GRID, 0, 100, pageGrid, FuncActive, OnChanged_Grid_Brightness, BeforeDraw_Grid_Brightness
)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
DEF_PAGE_2( pageGrid, // -V641 // -V1027                                                                                                                            //--- ДИСПЛЕЙ - СЕТКА ---
    "СЕТКА", "GRID",
    "Содержит настройки отображения координатной сетки.",
    "Contains settings of display of a coordinate Grid::",
    &cType,                    ///< ДИСПЛЕЙ - СЕТКА - Тип
    &gBrightness,              ///< ДИСПЛЕЙ - СЕТКА - Яркость
    Page::Name::Display_Grid, PageDisplay::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
