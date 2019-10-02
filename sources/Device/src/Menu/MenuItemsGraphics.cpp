#include "defines.h"
#include "log.h"
#include "MenuItems.h"
#include "Display/Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Menu/Menu.h"
#include "Utils/Values.h"
#include "Data/DataSettings.h"
#include "Settings/Settings.h"
#include "Hardware/HAL/HAL.h"
#include <cstring>

#ifdef WIN32
#include <cstdio>
#endif


using namespace Display::Primitives;
using namespace Hardware;

using Utils::Stack;


void GovernorColor::Draw(int x, int y, bool opened) const
{
    if (opened)
    {
        x = (x + Width() / 2) - widthOpened / 2;

        if (x < 0)
        {
            x = 0;
        }

        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}


void GovernorColor::DrawOpened(int x, int y) const
{
    int width = widthOpened;
    int height = heightOpened;
    OwnData()->ct->Init();
    Rectangle(width + 2, height + 2).Draw(x - 1, y - 1, ColorTitleBackground());
    Rectangle(width, height).Draw(x, y, ColorFrame());
    Region(width - 2, height / 2 - 2).Fill(x + 1, y + 1, ColorTitleBackground());
    HLine(width).Draw(x, y + Height() / 2 + 2, ColorFrame());
    Text(Title().CString()).DrawInCenterRect(x +  1, y - 1, width, Height() / 2 + 2, ColorTitleText());
    DrawValue(x + 1, y + 14);
}


void GovernorColor::DrawClosed(int x, int y) const
{
    OwnData()->ct->Init();
    DrawCommonHiPart(x, y, false);
    Region(Width() + 1, Value::HEIGHT - 3).Fill(x + 1, y + 13, OwnData()->ct->color);
}


void GovernorColor::DrawValue(int x, int y) const
{
    ColorType *ct = OwnData()->ct;

    int8 field = ct->currentField;
    const pString texts[4] = {"ßð", "Ñí", "Çë", "Êð"};

    uint color = COLOR(ct->color.value);
    uint red = R_FROM_COLOR(color);
    uint green = G_FROM_COLOR(color);
    uint blue = B_FROM_COLOR(color);
    ct->Init();
    int16 vals[4] = {(int16)(ct->brightness * 100.0F), (int16)blue, (int16)green, (int16)red};

    Region(widthOpened - 2, 12).Fill(x, y, ColorTitleBackground());
    x += 98;

    for (int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Region(29, 10).Fill(x - 1, y + 1, colorBack);
        String(texts[i]).Draw(x, y + 2, colorDraw);
        Integer(vals[i]).ToString(false, 1).Draw(x + 14, y + 2);
        x -= 32;
    }
}


void Governor::Draw(int x, int y, bool opened) const
{
    if (!IsActive())
    {
        return;
    }

    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}


void Governor::DrawOpened(int x, int y) const
{
    DrawCommonHiPart(x, y, true);
    Rectangle(Width() - 1, HeightOpened() - 1).Draw(x, y + 1, ColorFrame());
    DrawLowPart(x, y + 13);
}


void Governor::DrawClosed(int x, int y) const
{
    DrawLowPart(x, y + 14);
    DrawCommonHiPart(x, y, false);
}


void Governor::DrawValue(int x, int y) const
{
    int startX = x + 40;
    int signGovernor = (GetValue() < 0) ? -1 : 1;
    
    int value = GetValue();
    
    
    if (signGovernor == -1)
    {
        value = -value;
    }
    
    Font::SetCurrent(TypeFont::_5);
    bool sign = (OwnData()->min < 0);
    
    Integer(OwnData()->max).ToString(sign, 1).Draw(x + 50, y - 5, Color::WHITE);
    Integer(OwnData()->min).ToString(sign, 1).Draw(x + 50, y + 1);
    
    Font::SetCurrent(TypeFont::_8);

    DrawValueWithSelectedPosition(x + 3, y, value, NumDigits(), currentDigit, true);

    if (sign)
    {
        Char(signGovernor < 0 ? '\x9b' : '\x9a').Draw(startX - 30, y);
    }
}


void Governor::DrawLowPart(int x, int y) const
{
    Region(Width() - 3, Value::HEIGHT - 3).Fill(x + 1, y - 1, ColorBackground(this));

    if (Menu::OpenedItem() != this)
    {
        Color colorTextDown = Color::BLACK;

        if (IsCurrentItem())
        {
            Char(GetSymbol()).Draw4SymbolsInRect(x + Width() - 12, y - 1, Color::BACK);
        }
    
        x = Char(Symbol8::GOVERNOR_LEFT).Draw(x + 4, y, colorTextDown);
        
        int delta = (int)Step();
        if (delta == 0)
        {
            x = Integer(GetValue()).ToString(false, 1).Draw(x + 1, y);
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y - 2;
            int limWidth = Width();
            int limHeight = Value::HEIGHT - 1;
            Color::BLACK.SetAsCurrent();
            if (delta > 0)
            {
				x = Text(Integer(GetValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - delta, limX, limY, limWidth, limHeight);
				Text(Integer(NextValue()).ToString(false, 1)).DrawWithLimitation(drawX, y + 10 - delta, limX, limY, limWidth, limHeight);
            }
            if (delta < 0)
            {
				x = Text(Integer(GetValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - delta, limX, limY, limWidth, limHeight);
				Text(Integer(PrevValue()).ToString(false, 1)).DrawWithLimitation(drawX, y - 10 - delta, limX, limY, limWidth, limHeight);
            }
        }
        
        Char(Symbol8::GOVERNOR_RIGHT).Draw(x + 1, y, colorTextDown);
    }
    else
    {
        DrawValue(x, y);
    }
}


void Choice::Draw(int x, int y, bool opened) const
{
    if (opened)
    {
        DrawOpened(x, y);
    }
    else
    {
        DrawClosed(x, y);
    }
}


void Choice::DrawOpened(int x, int y) const
{
    int height = HeightOpened();
    
    Rectangle(Width() - 1, height - 1).Draw(x, y + 1, ColorFrame());
    DrawCommonHiPart(x, y + 1, true);

    Region(Width() - 3, height - MOI_HEIGHT_TITLE + 4).Fill(x + 1, y + MOI_HEIGHT_TITLE - 5, Color::BACK);
    int8 index = *OwnData()->cell;
    for (int i = 0; i < NumChoices(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT - 7;
        bool pressed = i == index;
        if (pressed)
        {
            Region(Width() - 3, MOSI_HEIGHT - 1).Fill(x + 1, yItem + 2, ColorBackground(this));
        }
        NameSubItem(i).Draw(x + 4, yItem + 2, pressed ? Color::BLACK : ColorBackground(this));
    }
}


void Choice::DrawClosed(int x, int y) const
{
    if (IsActive())
    {
        Region(Width() - 3, Value::HEIGHT - 3).Fill(x + 1, y + Value::HEIGHT, ColorBackground(this));

        int deltaY = (int)Step();
        Color colorText = Color::BLACK;
        colorText.SetAsCurrent();
        if (deltaY == 0)
        {
            NameCurrentSubItem().Draw(x + 4, y + Value::HEIGHT + 1);
        }
        else
        {
            Color::BACK.SetAsCurrent();
            Text(NameCurrentSubItem()).DrawWithLimitation(x + 4, y + Value::HEIGHT - deltaY + 1, x, y + 11, Width(), Value::HEIGHT - 1);

            HLine(Item::Width() + 1).Draw(x + 1, y + (deltaY > 0 ? 24 : 19) - deltaY);

            Text(deltaY > 0 ? NameNextSubItem() : NamePrevSubItem()).DrawWithLimitation(x + 4, y + (deltaY > 0 ? (Value::HEIGHT + 13) : 9) - deltaY, x, y + 11,
                Item::Width(), Value::HEIGHT - 1);
        }

        OwnData()->funcAfterDraw(x, y);
    }
    
    DrawCommonHiPart(x, y, false);
}


void Button::Draw(int x, int y, bool) const
{
    Region(Width() - 5, Height() - 4).Fill(x + 2, y + 3, ColorTitleBackground());

    Text(Title().CString()).DrawInCenterRect(x + 2, y, Width(), Height(), ColorTitleText());
}


void GraphButton::Draw(int x, int y, bool) const
{
    x += 2;
    y += 3;
    
    Region(GraphButton::Width() - 5, GraphButton::Height() - 4).Fill(x, y, ColorTitleBackground());

    ColorTitleText().SetAsCurrent();

    OwnData()->funcDraw(x + 20, y);
}


void Page::Draw(int x, int y, bool opened) const
{
    if(opened)
    {
        OwnData()->funcBeforeDraw();

        if (CurrentItemIsOpened())
        {
            Item *item = GetItem(PosCurrentItem());

            item->Draw(item->PositionOnScreenX(), Menu::Y0() - item->HeightOpened() + Item::Height() + 1, true);
        }
        else
        {
            DrawTitle(x, y - Menu::Title::HEIGHT);

            DrawItems(x, y);
        }
    }
    else
    {
        Region(Width() - 5, Height() - 4).Fill(x + 2, y + 3, ColorTitleBackground());

        Text(Title().CString()).DrawInCenterRect(x, y + 1, Width(), Height(), ColorTitleText());
    }
}


void Page::DrawTitle(int x, int y) const
{
    int eX = x;

    Rectangle(Menu::Title::WIDTH + 1, Menu::Title::HEIGHT + 1).Draw(x, y, ColorFrame());

    Region(Menu::Title::WIDTH - 1, Menu::Title::HEIGHT - 1).Fill(x + 1, y + 1, Color::BACK);

    bool condDrawRSet = NumSubPages() > 1 &&
        !Menu::CurrentItem()->Is(Item::Type::Governor) &&
        Menu::OpenedItem()->Is(Item::Type::Page);

    int delta = condDrawRSet ? -10 : 0;

    x = Text(Title().CString()).DrawInCenterRect(x, y, Menu::Title::WIDTH + 2 + delta, Menu::Title::HEIGHT, ColorTitleText());

    Color::GRAY_75.SetAsCurrent();
    DrawPagesUGO(eX + Menu::Title::WIDTH - 3, y + Menu::Title::HEIGHT);
    DrawNestingPage(eX + 5, y + Menu::Title::HEIGHT - 6);
}

void Page::DrawItems(int x, int y) const
{
    for (int i = 0; i < 5; i++)
    {
        Rectangle(Width(i) - 1, Height()).Draw(x, y + 1, ColorFrame());

        Region(Width(i) - 3, Height() - 2).Fill(x + 1, y + 2, Color::BACK);
        
        Item *item = GetItem(PosItemOnLeft() + i);
        
        if (item)
        {
            item->Draw(x, y, false);
        }

        x += Width(i) - 1;
    }
}



void Item::DrawCommonHiPart(int x, int y, bool opened) const
{
    int width = Width() - 3;

    Color colorFill = (IsPressed() && IsActive()) ? Color::WHITE : Color::BLACK;
    Color colorText = ColorTitleText();

    Region(width, Item::Value::HEIGHT - 3).Fill(x + 1, y + (opened ? 1 : 2), colorFill);

    Title().Draw(x + 6, y + (opened ? 2 : 3), colorText);
    
    if (opened)
    {
        HLine(width).Draw(x + 1, y + Item::Value::HEIGHT - 1, ColorFrame());
    }
}


void Governor::DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool) const
{
    Stack<uint8> stack(numDigits);
    
    for(uint i = 0; i < numDigits; i++)
    {
        stack.Push((uint8)(value % 10));
        value /= 10;
    }
    
    int height = 8;
    
    for (uint i = 0; i < numDigits; i++)
    {
        if (selPosFromEnd == ((int)numDigits - (int)i - 1))
        {
            Region(5, height).Fill(x - 1, y, Color::FILL);
        }
        
        uint8 val = stack.Pop();
        
        Char((char)(val + 48)).Draw(x, y, Color::BACK);
        
        x += 6;
    }
}


void Page::DrawPagesUGO(int right, int bottom) const
{
    int size = 4;
    int delta = 2;

    int allPages = (NumItems() - 1) / Item::NUM_ON_DISPLAY + 1;
    int currentPage = CurrentSubPage();

    int left = right - (size + 1) * allPages - delta + (3 - allPages);
    int top = bottom - size - delta;

    for (int p = 0; p < allPages; p++)
    {
        int x = left + p * (size + 2);
        if (p == currentPage)
        {
            Region(size, size).Fill(x, top);
        }
        else
        {
            Rectangle(size, size).Draw(x, top);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawNestingPage(int left, int bottom) const
{
    Page *parent = (Page *)Keeper();

    if (parent == nullptr)
    {
        return;
    }

    int nesting = 0;

    while (parent != nullptr)
    {
        const Page *page = (Page *)parent;
        parent = (Page *)page->Keeper();
        nesting++;                                  // -V127
    }

    int size = 4;
    int delta = 2;

    for (int i = 0; i <= nesting; i++)
    {
        int x = left + i * (size + delta);
        Rectangle(size, size).Draw(x, bottom);
    }
}


int GraphButton::NumHints() const
{
    if (OwnData()->hintUGO == nullptr)
    {
        return 0;
    }

    const StructHelpDrawButton *hint = OwnData()->hintUGO;

    int result = 0;

    while (hint++)
    {
        result++;
    }

    return result;
}


void GraphButton::DrawHints(int x, int y, int width) const
{
    if (NumHints() == 0)
    {
        return;
    }
    Region(width, 239 - y).Fill(x, y, ColorTitleBackground());
    Rectangle(width, 239 - y).Draw(x, y, ColorFrame());
    const StructHelpDrawButton *structHelp = &OwnData()->hintUGO[0];
    x += 3;
    y += 3;
    for (int i = 0; i < NumHints(); i++)
    {
        Rectangle(GraphButton::Width(), GraphButton::Height()).Draw(x, y);
        structHelp->funcDrawUGO(x, y);

        int yNew = Text(structHelp->helpUGO).DrawInRectWithTransfers(x + 23, y + 1, width - 30, 20);

        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}
