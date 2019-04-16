#include "defines.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/MenuItems.h"
#include "Settings/Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern const PageBase pageColors;

const PageBase *PageDisplay::PageSettings::PageColors::pointer = &pageColors;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void OnPress_ResetColors()
{
}

DEF_BUTTON( bReset,                                                                                                                          //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить ---
    "Сбросить",
    "Сброс всех цветов на значения по умолчанию",
    pageColors, EmptyFuncBtV, OnPress_ResetColors, EmptyFuncVII
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DEF_CHOICE_2( cScheme,                                                                                                                 //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема ---
    "Цвет схема",
    "Изменение цветовой схемы",
    "Схема 1",
    "Схема 2",
    set.serv_colorScheme, pageColors, FuncActive, Choice::EmptyChange, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeA = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::Channel(Chan::A))

DEF_GOVERNOR_COLOR( gcChannelA,                                                                                                               //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1 ---
    "Канал 1",
    "Выбор цвета канала 1",
    PageDisplay::PageSettings::PageColors::colorTypeA, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeB = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::Channel(Chan::B))

DEF_GOVERNOR_COLOR( gcChannelB,                                                                                                               //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2 ---
    "Канал 2",
    "Выбор цвета канала 1",
    PageDisplay::PageSettings::PageColors::colorTypeB, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ColorType PageDisplay::PageSettings::PageColors::colorTypeGrid = COLOR_TYPE(0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, 0.0F, Color::GRID)

DEF_GOVERNOR_COLOR( gcGrid,                                                                                                                     //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка ---
    "Сетка",
    "Устанавливает цвет сетки",
    PageDisplay::PageSettings::PageColors::colorTypeGrid, pageColors
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Settings_Colors_Background(bool)
{
    Color::InitGlobalColors();

    gcChannelA.ct->color = Color::Channel(Chan::A);
    gcChannelB.ct->color = Color::Channel(Chan::B);
    gcGrid.ct->color = Color::GRID;

    gcChannelA.ct->ReInit();
    gcChannelB.ct->ReInit();
    gcGrid.ct->ReInit();
}

DEF_CHOICE_2( cBackground,                                                                                                                        //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон ---
    "Фон",
    "Выбор цвета фона",
    "Чёрный",
    "Белый",
    BACKGROUND, pageColors, FuncActive, OnChanged_Settings_Colors_Background, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageDisplay::PageSettings::PageColors::Init()
{
    OnChanged_Settings_Colors_Background(true);
}


/// \todo Добавить дополнительные цвета 1-го и 2-го каналов
DEF_PAGE_6( pageColors, // -V641 // -V1027                                                                                                              //--- ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА ---
    "ЦВЕТА",
    "Выбор цветов дисплея",
    &bReset,              ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сбросить
    &cScheme,             ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Цветовая схема
    &gcChannelA,          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 1
    &gcChannelB,          ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Канал 2
    &gcGrid,              ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Сетка
    &cBackground,         ///< ДИСПЛЕЙ - НАСТРОЙКИ - ЦВЕТА - Фон
    Page::Name::Display_Settings_Colors, PageDisplay::PageSettings::pointer, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
