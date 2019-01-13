#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Display_Primitives.h"
#include "Display/Font/Font.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#endif
#include "Menu/Pages/Include/HelpContentPages.h"
#include "Menu/Pages/Include/HelpContent.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
using Display::Rectangle;
using Display::Region;
using Display::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static int currentParagraph = 0;   // Если TypePage(currentPage) == TypePage_Content, то указывает не текущий раздел оглавления
static const PageHelp *currentPage = &helpMain;


static const int WIDTH = 295;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawPageContent()
{
    int y = 50;

    //Painter::DrawStringInCenterRect(0, y, WIDTH, 10, currentPage->titleHint[LANG]);
    Text(currentPage->titleHint[LANG]).DrawInCenterRect(0, y, WIDTH, 10);

    int numPage = 0;

    y += 40;

    while(currentPage->pages[numPage])
    {
        Page *page = (Page *)currentPage->pages[numPage];
        const char *title = page->titleHint[LANG];
        if(currentParagraph == numPage)
        {
            Painter::DrawStringInCenterRectOnBackgroundC(0, y, WIDTH, 10, title, Color::BACK, 2, Color::FILL);
        }
        else
        {
            //Painter::DrawStringInCenterRect(0, y, WIDTH, 10, title, Color::FILL);
            Text(title).DrawInCenterRect(0, y, WIDTH, 10, Color::FILL);
        }
        y += 16;
        numPage++;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPageDescription()
{
    //Painter::DrawStringInCenterRect(0, 3, WIDTH, 10, currentPage->titleHint[LANG]);
    Text(currentPage->titleHint[LANG]).DrawInCenterRect(0, 3, WIDTH, 10);

    Painter::DrawTextInRectWithTransfers(2, 15, WIDTH - 5, 240, currentPage->titleHint[2 + LANG]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HelpContent_Draw()
{
    Region(319 - Grid::Right(), 20).Draw(Grid::Right(), 0, Color::BACK);
    Region(319 - Grid::Right(), 21).Draw(Grid::Right(), 219);
    Region(WIDTH, 237).Draw(1, 1);
    Rectangle(WIDTH + 1, 239).Draw(1, 0, Color::FILL);   /** \todo Здесь непонятно, почему так. Координаты верхнего левого угла должны быть
                                                                0, 0, но в таком случае левой вертикальной полосы не видно */

    /*
    uint16 *addr1 = (uint16 *)(0x08000000 + (rand() % 65535));
    uint8 *addr2 = (uint8 *)(0x08000000 + (rand() % 65535));

    Color::SetCurrent(Color::FILL);
    for (int i = 0; i < 10000; i++)
    {
        Painter::SetPoint((*addr1) % WIDTH, Math_LimitationInt(*addr2, 0, 239));
        addr1++;
        addr2++;
    }
    */

    if(currentPage->type == TypePage_Content)
    {
        DrawPageContent();
    }
    else if(currentPage->type == TypePage_Description)
    {
        DrawPageDescription();
    }
    else
    {
        // здесь непонятно что делать
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int NumParagraphs(const PageHelp *page)
{
    int retValue = 0;
    while(page->pages[retValue])
    {
        retValue++;
    }
    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HelpContent_NextParagraph()
{
    if(currentPage->type == TypePage_Content)
    {
        Math::CircleIncrease<int>(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HelpContent_PrevParagraph()
{
    if(currentPage->type == TypePage_Content)
    {
        Math::CircleDecrease<int>(&currentParagraph, 0, NumParagraphs(currentPage) - 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HelpContent_EnterParagraph()
{
    if(currentPage->type == TypePage_Content)
    {
        currentPage = (const PageHelp *)currentPage->pages[currentParagraph];
    }
    currentParagraph = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HelpContent_LeaveParagraph()
{
    currentParagraph = 0;
    if(currentPage->parent)
    {
        currentPage = (const PageHelp *)currentPage->parent;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool HelpContent_LeaveParagraphIsActive()
{
    return currentPage->parent != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool HelpContent_EnterParagraphIsActive()
{
    return currentPage->type == TypePage_Content;
}
