#include "defines.h"
#include "log.h"
#include "MenuItems.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Menu/Menu.h"
#include "Utils/Values.h"
#include "Data/DataSettings.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Hardware/HAL/HAL.h"

#ifdef WIN32
#include <cstdio>
#endif


using namespace Display::Primitives;
using namespace Hardware;

using Utils::Stack;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawCommonHiPart(Item *item, int x, int y, bool pressed, bool shade, bool opened);
/// Нарисовать значение регулятора в режиме поразрядной регулировки
/// setPosFromEnd - подсвеченный (активный) разряд, начиная с конца. Если selPosFromEnd == -1, подсвечивать не нужно
static void DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool fillNull);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////q
void GovernorColor::Draw(int x, int y, bool opened)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawOpened(int x, int y)
{
    static const int delta = 43;
    x -= delta;
    ct->Init();
    Rectangle(Item::HEIGHT + delta + 2, Item::HEIGHT + 2).Draw(x - 1, y - 1, Color::BACK);
    Rectangle(Width() + delta, Item::HEIGHT).Draw(x, y, Color::MenuTitleText(false));
    Region(Item::Value::WIDTH + 2 + delta, Item::Value::HEIGHT + 3).Fill(x + 1, y + 1, Color::MenuItem(false));

    HLine(Width() + delta).Draw(x, y + Item::HEIGHT / 2 + 2, Color::MenuTitleText(false));

    Text(Title().CString()).DrawInCenterRect(x + (IsPressed() ? 2 : 1), y + (IsPressed() ? 2 : 1), Width() + delta, Item::HEIGHT / 2 + 2, Color::WHITE);

    DrawValue(x + 1, y + 19, delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawClosed(int x, int y)
{
    ct->Init();
    DrawCommonHiPart(this, x, y, IsPressed(), IsShade() || !IsActive(), false);
    Region(Item::Value::WIDTH + 1, Item::Value::HEIGHT - 3).Fill(x + 1, y + 13, ct->color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawValue(int x, int y, int delta)
{
    int8 field = ct->currentField;
    const pString texts[4] = {"Яр", "Сн", "Зл", "Кр"};

    uint color = COLOR(ct->color.value);
    uint red = R_FROM_COLOR(color);
    uint green = G_FROM_COLOR(color);
    uint blue = B_FROM_COLOR(color);
    ct->Init();
    int16 vals[4] = {(int16)(ct->brightness * 100.0F), (int16)blue, (int16)green, (int16)red};

    Region(Width() + delta - 2, Item::HEIGHT / 2 - 3).Fill(x, y, Color::BLACK);
    x += 92;

    for (int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Region(29, 10).Fill(x - 1, y + 1, colorBack);
        String(texts[i]).Draw(x, y + 2, colorDraw);
        Integer(vals[i]).ToString(false, 1).Draw(x + 14, y + 2);
        x -= 30;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::Draw(int x, int y, bool opened)
{
    if (funcBeforeDraw)
    {
        funcBeforeDraw();
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawOpened(int x, int y)
{
    bool shade = IsShade() || !IsActive();
    DrawCommonHiPart(this, x, y, IsPressed(), shade, true);
    DrawLowPart(x, y + 13, shade);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawClosed(int x, int y)
{
    bool shade = IsShade() || !IsActive();
    DrawLowPart(x, y + 14, shade);
    DrawCommonHiPart(this, x, y, IsPressed(), shade, false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawValue(int x, int y)
{
    int startX = x + 40;
    int signGovernor = (GetValue() < 0) ? -1 : 1;
    
    int value = GetValue();
    
    
    if (signGovernor == -1)
    {
        value = -value;
    }
    
    Font::SetCurrent(Font::Type::_5);
    bool sign = (minValue < 0);
    
    //if(maxValue < 65536)
    {
        Integer(maxValue).ToString(sign, 1).Draw(x + 55, y - 5, Color::WHITE);
        Integer(minValue).ToString(sign, 1).Draw(x + 55, y + 2);
    }
    
    Font::SetCurrent(Font::Type::_8);

    DrawValueWithSelectedPosition(x + 2, y, value, NumDigits(), gCurDigit, true);

    if (sign)
    {
        Char(signGovernor < 0 ? '\x9b' : '\x9a').Draw(startX - 30, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawLowPart(int x, int y, bool shade)
{
    Color colorTextDown = Color::BLACK;

    Region(Item::Value::WIDTH + 1, Item::Value::HEIGHT - 3).Fill(x + 1, y - 1, Color::MenuItemField(shade));

    if (shade)
    {
        colorTextDown = Color::MENU_TITLE_DARK;
    }

    if (Menu::OpenedItem() != this)
    {
        if (IsCurrentItem())
        {
            Char(GetSymbol()).Draw4SymbolsInRect(x + Width() - 12, y - 1, Color::BACK);
        }
    
        x = Char(SYMBOL_GOVERNOR_LEFT).Draw(x + 4, y, colorTextDown);
        
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
            int limWidth = Item::Value::WIDTH;
            int limHeight = Item::Value::HEIGHT - 1;
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
        
        Char(SYMBOL_GOVERNOR_RIGHT).Draw(x + 1, y, colorTextDown);
    }
    else
    {
        DrawValue(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::Draw(int x, int y, bool opened)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawOpened(int x, int y)
{
    int height = HeightOpened();
    
    Rectangle(Width(), height).Draw(x, y, Color::FILL);
    DrawCommonHiPart(this, x, y, IsPressed(), false, true);

    Region(Width() - 2, height - MOI_HEIGHT_TITLE + 4).Fill(x + 1, y + MOI_HEIGHT_TITLE - 5, Color::BACK);
    int8 index = *cell;
    for (int i = 0; i < NumSubItems(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT - 7;
        bool pressed = i == index;
        if (pressed)
        {
            Region(Width() - 2, MOSI_HEIGHT - 1).Fill(x + 1, yItem + 2, ColorMenuField(this));
        }
        NameSubItem(i).Draw(x + 4, yItem + 2, pressed ? Color::BLACK : ColorMenuField(this));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();

    Region(Item::Value::WIDTH + 1, Item::Value::HEIGHT - 3).Fill(x + 1, y + Item::Value::HEIGHT, ColorMenuField(this));

    int deltaY = (int)Step();
    Color colorText = shade ? Color::MenuItem(true) : Color::BLACK;
    colorText.SetAsCurrent();
    if (deltaY == 0)
    {
        NameCurrentSubItem().Draw(x + 4, y + Item::Value::HEIGHT + 1);
    }
    else
    {
        Color::BACK.SetAsCurrent();
		Text(NameCurrentSubItem()).DrawWithLimitation(x + 4, y + Item::Value::HEIGHT - deltaY + 1, x, y + 11, Item::Value::WIDTH, Item::Value::HEIGHT - 1);

        HLine(Item::Value::WIDTH + 1).Draw(x + 1, y + (deltaY > 0 ? 24 : 19) - deltaY);

		Text(deltaY > 0 ? NameNextSubItem() : NamePrevSubItem()).DrawWithLimitation(x + 4, y + (deltaY > 0 ? (Item::Value::HEIGHT + 13) : 9) - deltaY, x, y + 11,
            Item::Value::WIDTH, Item::Value::HEIGHT - 1);
    }
  
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
    
    DrawCommonHiPart(this, x, y, pressed, shade, false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y) const
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsActive();

    Color color = shade ? Color::MenuItem(true) : Color::WHITE;
    
    Region(Width() - 2, Item::HEIGHT - 2).Fill(x + 1, y + 2, Color::MenuItem(false));
    Region(Width() - 6, Item::HEIGHT - 6).Fill(x + 3, y + 4, Color::MenuItem(false));

    int delta = (pressed && (!shade)) ? 2 : 1;

    Text(Title().CString()).DrawInCenterRect(x + delta, y + delta, Width(), Item::HEIGHT, color);

    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::Draw(int x, int y) const
{
    x += 22;
    y += 3;
    
    if (IsActive())
    {
        if (IsPressed())
        {
            Region(WIDTH_SB, WIDTH_SB).Fill(x, y, Color::FILL);
            Color::BLACK.SetAsCurrent();
        }
        else
        {
            Color::FILL.SetAsCurrent();
        }
        funcForDraw(x, y);
    }
    else
    {
        Color::FILL.SetAsCurrent();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::Draw(int x, int y, bool opened) const
{
    if(opened)
    {
        if (CurrentItemIsOpened())
        {
            Item *item = GetItem(PosCurrentItem());

            x = ItemOpenedPosX(item);
            y = Menu::Y0() - item->HeightOpened() + Item::HEIGHT + 1;

            if (IS_CHOICE(item) || IS_CHOICE_REG(item))
            {
                ((Choice *)item)->Draw(x, y, true);
            }
            else if (IS_GOVERNOR(item))
            {
                ((Governor *)item)->Draw(x, y, true);
            }
            else if (IS_GOVERNOR_COLOR(item))
            {
                ((GovernorColor *)item)->Draw(x, y, true);
            }
            else if (IS_TIME(item))
            {
                ((TimeItem *)item)->Draw(x, y, true);
            }
            else
            {
                // остальные контролы не обрабатываем
            }
        }
        else
        {
            DrawTitle(x, y - Menu::Title::HEIGHT);

            DrawItems(x, y);
        }

        if (funcOnDraw)
        {
            funcOnDraw();
        }
    }
    else
    {
        Region(Width() - 3, Item::HEIGHT - 2).Fill(x + 1, y + 2, Color::MenuItem(false));
        Text(Title().CString()).DrawInCenterRect(x, y + 1, Width(), Item::HEIGHT, IsActive() ? Color::FILL : Color::MENU_TITLE_DARK);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawTitle(int x, int yTop) const
{
    int eX = x;

    if (isPageSB)
    {
        SMALL_BUTTON_FROM_PAGE(this, 0)->Draw(x, yTop + 3);
        return;
    }

    bool shade = CurrentItemIsOpened();

    Region(Menu::Title::WIDTH + 2, Menu::Title::HEIGHT + 2).Fill(x - 1, yTop, Color::BACK);

    Rectangle(Menu::Title::WIDTH + 1, Menu::Title::HEIGHT + 1).Draw(x, yTop, Color::FILL);

    if (shade)
    {
        Region(Menu::Title::WIDTH - 1, Menu::Title::HEIGHT - 1).Fill(x + 1, yTop + 1, Color::MENU_TITLE_DARK);
        Region(Menu::Title::WIDTH - 7, Menu::Title::HEIGHT - 7).Fill(x + 4, yTop + 4, Color::MENU_TITLE_DARK);
    }
    else
    {
        Region(Menu::Title::WIDTH - 1, Menu::Title::HEIGHT - 1).Fill(x + 1, yTop + 1, Color::MenuTitleText(false));
    }

    bool condDrawRSet = NumSubPages() > 1 &&
        NOT_CHOICE_REG(Menu::CurrentItem()) &&
        NOT_GOVERNOR(Menu::CurrentItem()) &&
        IS_PAGE(Menu::OpenedItem());

    int delta = condDrawRSet ? -10 : 0;
    Color colorText = Color::FILL;

    x = Text(Title().CString()).DrawInCenterRect(x, yTop, Menu::Title::WIDTH + 2 + delta, Menu::Title::HEIGHT, colorText);

    Color::GRAY_75.SetAsCurrent();
    DrawPagesUGO(eX + Menu::Title::WIDTH - 3, yTop + Menu::Title::HEIGHT);
    DrawNestingPage(eX + 5, yTop + Menu::Title::HEIGHT - 6);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawItems(int x, int y) const
{
    for (int i = 0; i < 5; i++)
    {
        /// \todo Надо бы не делать это для пунктов меню, которые существуют и всё равно отрисовываться будут - зачем зря грузить процессор

        Rectangle(Width() - 1, Item::HEIGHT).Draw(x, y + 1, Color::FILL);
        Region(Width() - 3, Item::HEIGHT - 2).Fill(x + 1, y + 2, Color::BACK);
        
        Item *item = GetItem(PosItemOnLeft() + i);
        
        if (item)
        {
            item->Draw(x, y, false);
        }

        x += Width();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const Item *Menu::ItemForFuncKey(Key::E key)
{
    if (!Key(key).IsFunctional())
    {
        return &emptyControl;
    }

    Item *item = Menu::OpenedItem();

    if (IS_PAGE(item))
    {
        return ((Page *)item)->ItemForFuncKey(key);
    }

    return &emptyControl;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::Draw(int x, int y, bool opened) const
{
    if (type == Item::Type::Choice || type == Item::Type::ChoiceReg)
    {
        ((Choice *)this)->Draw(x, y, opened);
    }
    else if (type == Item::Type::Button)
    {
        ((Button *)this)->Draw(x, y);
    }
    else if (type == Item::Type::Page)
    {
        ((Page *)this)->Draw(x, y, opened);
    }
    else if (type == Item::Type::Governor)
    {
        ((Governor *)this)->Draw(x, y, opened);
    }
    else if (type == Item::Type::Time)
    {
        ((TimeItem *)this)->Draw(x, y, opened);
    }
    else if (type == Item::Type::GovernorColor)
    {
        ((GovernorColor *)this)->Draw(x, y, opened);
    }
    else if (type == Item::Type::DrawButton)
    {
        ((SButton *)this)->Draw(x, y);
    }
    else
    {
        // остальные типы контролов не обрабатываем
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeItem::Draw(int x, int y, bool opened)
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeItem::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade();
    DrawCommonHiPart(this, x, y, pressed, shade, false);

    Region(Item::Value::WIDTH + 2, Item::Value::HEIGHT + 3).Fill(x + 1, y + 17, Color::MenuItemField(shade));

    int deltaField = 10;
    int deltaSeparator = 2;
    int startX = 3;
    y += 21;
    PackedTime time = Clock::GetTime();
    Integer((int)time.hours).ToString(false, 2).Draw(x + startX, y, shade ? Color::MenuItem(true) : Color::BLACK);
    String(':').Draw(x + startX + deltaField, y);
    Integer((int)time.minutes).ToString(false, 2).Draw(x + startX + deltaField + deltaSeparator, y);
    String(':').Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Integer((int)time.seconds).ToString(false, 2).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);

    startX = 44;
    Integer((int)time.day).ToString(false, 2).Draw(x + startX, y);
    String(':').Draw(x + startX + deltaField, y);
    Integer((int)time.month).ToString(false, 2).Draw(x + startX + deltaField + deltaSeparator, y);
    String(':').Draw(x + startX + 2 * deltaField + deltaSeparator, y);
    Integer((int)time.year).ToString(false, 2).Draw(x + startX + 2 * deltaField + 2 * deltaSeparator, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeItem::DrawOpened(int x, int y)
{
    int width = Item::Value::WIDTH + 3;
    int height = 61;
    Rectangle(width + 2, height + 3).Draw(x - 1, y - 1, Color::BACK);
    DrawCommonHiPart(this, x - 1, y - 1, IsPressed(), false, false);

    Rectangle(width + 1, height + 1).Draw(x - 1, y, Color::MenuTitleText(false));

    HLine(Width()).Draw(x, y + MOI_HEIGHT_TITLE - 1);

    Region(Width() - 1, height - MOI_HEIGHT_TITLE).Fill(x, y + MOI_HEIGHT_TITLE, Color::BLACK);

    int y0 = 21;
    int y1 = 31;

    typedef struct
    {
        int x;
        int y;
        int width;
    } StructPaint;

    int y2 = 41;
    int y3 = 51;
    int dX = 13;
    int wS = 10;
    int x0 = 41;
    StructPaint strPaint[8] =
    {
        {3,             y3, 60},    // Не сохранять
        {x0,            y0, wS},    // день
        {x0 + dX,       y0, wS},    // месяц
        {x0 + 2 * dX,   y0, wS},    // год
        {x0,            y1, wS},    // часы
        {x0 + dX,       y1, wS},    // мин
        {x0 + 2 * dX,   y1, wS},    // сек
        {3,             y2, 46}     // Сохранить
    };

    char strI[8][20];
    std::strcpy(strI[iEXIT],  "Не сохранять");
    std::strcpy(strI[iDAY],   Integer(*day).ToString(false, 2).CString());
    std::strcpy(strI[iMONTH], Integer(*month).ToString(false, 2).CString());
    std::strcpy(strI[iYEAR],  Integer(*year).ToString(false, 2).CString());
    std::strcpy(strI[iHOURS], Integer(*hours).ToString(false, 2).CString());
    std::strcpy(strI[iMIN],   Integer(*minutes).ToString(false, 2).CString());
    std::strcpy(strI[iSEC],   Integer(*seconds).ToString(false, 2).CString());
    std::strcpy(strI[iSET],   "Сохранить");

    String("д м г - ").Draw(x + 3, y + y0, Color::WHITE);
    String("ч м с - ").Draw(x + 3, y + y1);

    for (int i = 0; i < 8; i++)
    {
        if (*curField == i)
        {
            Region(strPaint[i].width, 8).Fill(x + strPaint[i].x - 1, y + strPaint[i].y, Color::FLASH_10);
        }
        String(strI[i]).Draw(x + strPaint[i].x, y + strPaint[i].y, *curField == i ? Color::FLASH_01 : Color::WHITE);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawCommonHiPart(Item *item, int x, int y, bool pressed, bool shade, bool opened)
{
    int delta = pressed && !shade ? 1 : 0;
    int width = Item::Value::WIDTH;

    Color color = shade ? Color::MENU_TITLE_DARK : Color::WHITE;

    Region(width + (opened ? 2 : 1), Item::Value::HEIGHT - (opened ? 2 : 3)).Fill(x + 1, y + (opened ? 1 : 2), Color::MenuItem(false));

    item->Title().Draw(x + delta + (opened ? 4 : 6), y + delta + (opened ? 2 : 3), color);

    if (opened)
    {
        HLine(width + 2).Draw(x + 1, y + Item::Value::HEIGHT, Color::FILL);
    }

    if(item->IsCurrentItem())
    {
        char symbol = 0;
   
        if (IS_CHOICE_REG(item) || (item->IsOpened() && IS_CHOICE(item)))
        {
            symbol = item->GetSymbol();
        }
        else if (IS_TIME(item))
        {
            TimeItem* time = (TimeItem*)item;
            if ((Menu::OpenedItem() == item) && (*time->curField != iEXIT) && (*time->curField != iSET))
            {
                symbol = time->GetSymbol();
            }
        }
        else
        {
            // для остальных контролов не нужно
        }

        Char(symbol).Draw4SymbolsInRect(x + item->Width() - 13, y + (item->IsOpened() ? 0 : 13), shade ? color : Color::BLACK);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool)
{
    Stack<uint8> stack(numDigits);
    
    for(uint i = 0; i < numDigits; i++)
    {
        stack.Push((uint8)(value % 10));
        value /= 10;
    }
    
    int height = 8;
    
    Color fill = Color::BACK;
    Color back = Color::FILL;
    
    
    for (uint i = 0; i < numDigits; i++)
    {
        if (selPosFromEnd == ((int)numDigits - (int)i - 1))
        {
            Region(5, height).Fill(x - 1, y, back);
        }
        
        uint8 val = stack.Pop();
        
        Char((char)(val + 48)).Draw(x, y, fill);
        
        x += 6;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::ItemOpenedPosX(const Item *item)
{
    const Page *page = item->Keeper();
    return (page->PosCurrentItem() % MENU_ITEMS_ON_DISPLAY) * item->Width();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawPagesUGO(int right, int bottom) const
{
    int size = 4;
    int delta = 2;

    int allPages = (NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::DrawHints(int x, int y, int width) const
{
    if (numHints == 0)
    {
        return;
    }
    Region(width, 239 - y).Fill(x, y, Color::BACK);
    Rectangle(width, 239 - y).Draw(x, y, Color::FILL);
    const StructHelpDrawButton *structHelp = &hintUGO[0];
    x += 3;
    y += 3;
    for (int i = 0; i < numHints; i++)
    {
        Rectangle(WIDTH_SB, WIDTH_SB).Draw(x, y);
        structHelp->funcDrawUGO(x, y);

        int yNew = Text(structHelp->helpUGO).DrawInRectWithTransfers(x + 23, y + 1, width - 30, 20);

        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}
