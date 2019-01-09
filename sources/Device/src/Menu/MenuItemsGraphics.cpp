#include "defines.h"
#include "log.h"
#include "MenuItems.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "Menu/Menu.h"
#include "Utils/Values.h"
#include "Data/DataSettings.h"
#include "Hardware/CPU.h"
#include "Settings/Settings.h"

#include <cstring>

#ifdef WIN32
#include <cstdio>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawCommonHiPart(Control *item, int x, int y, bool pressed, bool shade, bool opened);
/// Нарисовать значение регулятора в режиме поразрядной регулировки
/// setPosFromEnd - подсвеченный (активный) разряд, начиная с конца. Если selPosFromEnd == -1, подсвечивать не нужно
static void DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool fillNull);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////q
///Возвращает функциональную кнопку, находящуюся на данном X.
/// Расстояние между левой и правой границами выведенного меню делится на пять раввных промежутков и возвращается название той кнопки, которая
/// попадает в даннуй промежуток.
static Key::E GetFuncButtonFromX(int _x)
{
    int x = Display::WIDTH / 10;

    int step = Display::WIDTH / 5;

    Key::E button = Key::F1;

    for (int i = 0; i < 5; i++)
    {
        if (_x < x)
        {
            return button;
        }
        button = (Key::E)((int)button + 1);    // button++;
        x += step;
    }
    return  Key::F5;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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
    ct->Init(false);
    Painter::DrawRectangle(x - 1, y - 1, Menu::Item::HEIGHT + delta + 2, Menu::Item::HEIGHT + 2, Color::BLACK);
    Painter::DrawRectangle(x, y, Width() + delta, Menu::Item::HEIGHT, Color::MenuTitle(false));
    Painter::FillRegion(x + 1, y + 1, Menu::Item::Value::WIDTH + 2 + delta, Menu::Item::Value::HEIGHT + 3, Color::MenuItem(false));
    Painter::DrawHLine(y + Menu::Item::HEIGHT / 2 + 2, x, x + Width() + delta, Color::MenuTitle(false));
    Painter::DrawStringInCenterRect(x + (IsPressed() ? 2 : 1), y + (IsPressed() ? 2 : 1), Width() + delta, Menu::Item::HEIGHT / 2 + 2, 
                                    Title().CString(), Color::WHITE);
    DrawValue(x + 1, y + 19, delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawClosed(int x, int y)
{
    ct->Init(false);
    DrawCommonHiPart(this, x, y, IsPressed(), IsShade() || !IsAcitve(), false);
    Painter::FillRegion(x + 2, y + 20, Menu::Item::Value::WIDTH, Menu::Item::Value::HEIGHT - 1, ct->color);
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
    ct->Init(false);
    int16 vals[4] = {(int16)(ct->brightness * 100.0F), (int16)blue, (int16)green, (int16)red};

    Painter::FillRegion(x, y, Width() + delta - 2, Menu::Item::HEIGHT / 2 - 3, Color::BLACK);
    x += 92;

    for (int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Painter::FillRegion(x - 1, y + 1, 29, 10, colorBack);
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
    bool shade = IsShade() || !IsAcitve();
    DrawCommonHiPart(this, x, y, IsPressed(), shade, true);
    DrawLowPart(x, y + 13, shade);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawClosed(int x, int y)
{
    bool shade = IsShade() || !IsAcitve();
    DrawLowPart(x, y + 14, shade);
    DrawCommonHiPart(this, x, y, IsPressed(), shade, false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawValue(int x, int y)
{
    int startX = x + 40;
    int signGovernor = (*cell < 0) ? -1 : 1;
    
    int value = *cell;
    
    
    if (signGovernor == -1)
    {
        value = -value;
    }
    
    Painter::SetFont(Font::Type::_5);
    bool sign = (minValue < 0);
    
    if(maxValue < 65536)
    {
        Integer(maxValue).ToString(sign, 1).Draw(x + 55, y - 5, Color::WHITE);
        Integer(minValue).ToString(sign, 1).Draw(x + 55, y + 2);
    }
    
    Painter::SetFont(Font::Type::_8);

    DrawValueWithSelectedPosition(x + 2, y, value, NumDigits(), gCurDigit, true);

    if (sign)
    {
        Painter::DrawChar(startX - 30, y, signGovernor < 0 ? '\x9b' : '\x9a');
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawLowPart(int x, int y, bool shade)
{
    Color colorTextDown = Color::BLACK;

    Painter::FillRegion(x + 1, y - 1, Menu::Item::Value::WIDTH + 1, Menu::Item::Value::HEIGHT - 3, Color::MENU_FIELD);

    if (shade)
    {
        colorTextDown = Color::MenuItem(false);
    }

    if (Menu::OpenedItem() != this)
    {
        if (IsCurrentItem())
        {
            Painter::Draw4SymbolsInRect(x + Width() - 12, y - 1, GetSymbol(), Color::BACK);
        }
    
        x = Painter::DrawChar(x + 4, y, SYMBOL_GOVERNOR_LEFT, colorTextDown);
        
        int delta = (int)Step();
        if (delta == 0)
        {
            x = Integer(*cell).ToString(false, 1).Draw(x + 1, y);
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y - 2;
            int limWidth = Menu::Item::Value::WIDTH;
            int limHeight = Menu::Item::Value::HEIGHT - 1;
            Painter::SetColor(Color::BLACK);
            if (delta > 0)
            {
                x = Painter::DrawTextWithLimitation(drawX, y - delta, Integer(*cell).ToString(false, 1).CString(), limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitation(drawX, y + 10 - delta, Integer(NextValue()).ToString(false, 1).CString(), limX, limY, limWidth, limHeight);
            }
            if (delta < 0)
            {
                x = Painter::DrawTextWithLimitation(drawX, y - delta, Integer(*cell).ToString(false, 1).CString(), limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitation(drawX, y - 10 - delta, Integer(PrevValue()).ToString(false, 1).CString(), limX, limY, limWidth, limHeight);
            }
        }
        
        Painter::DrawChar(x + 1, y, SYMBOL_GOVERNOR_RIGHT, colorTextDown);
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
    
    Painter::DrawRectangle(x, y, Width(), height, Color::FILL);
    DrawCommonHiPart(this, x, y, IsPressed(), false, true);

    Painter::FillRegion(x + 1, y + MOI_HEIGHT_TITLE - 5, Width() - 2, height - MOI_HEIGHT_TITLE + 4, Color::BACK);
    int8 index = *cell;
    for (int i = 0; i < NumSubItems(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT - 7;
        bool pressed = i == index;
        if (pressed)
        {
            Painter::FillRegion(x + 1, yItem + 2, Width() - 2, MOSI_HEIGHT - 1, ColorMenuField(this));
        }
        NameSubItem(i).Draw(x + 4, yItem + 2, pressed ? Color::BLACK : ColorMenuField(this));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !funcOfActive();

    Painter::FillRegion(x + 1, y + Menu::Item::Value::HEIGHT, Menu::Item::Value::WIDTH + 1, Menu::Item::Value::HEIGHT - 3, ColorMenuField(this));

    int deltaY = (int)Step();
    Color colorText = shade ? Color::MenuItem(true) : Color::BLACK;
    Painter::SetColor(colorText);
    if (deltaY == 0)
    {
        NameCurrentSubItem().Draw(x + 4, y + Menu::Item::Value::HEIGHT + 1);
    }
    else
    {
        Painter::SetColor(Color::BACK);
        Painter::DrawTextWithLimitation(x + 4, y + Menu::Item::Value::HEIGHT - deltaY + 1, NameCurrentSubItem().CString(), x, y + 11, Menu::Item::Value::WIDTH, Menu::Item::Value::HEIGHT - 1);
        Painter::DrawHLine(y + (deltaY > 0 ? 24 : 19) - deltaY, x + 1, x + Menu::Item::Value::WIDTH + 2);
        Painter::DrawTextWithLimitation(x + 4, y + (deltaY > 0 ? (Menu::Item::Value::HEIGHT + 13) : 9) - deltaY, deltaY > 0 ? NameNextSubItem() : NamePrevSubItem(),
                                        x, y + 11, Menu::Item::Value::WIDTH, Menu::Item::Value::HEIGHT - 1);
    }
  
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
    DrawCommonHiPart(this, x, y, pressed, shade, false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !funcOfActive();

    Painter::DrawHLine(y + 1, x, x + Width(), Color::MenuTitle(shade));
    Color color = shade ? Color::MenuItem(true) : Color::WHITE;
    Painter::FillRegion(x + 1, y + 2, Width() - 2, Menu::Item::HEIGHT - 2, Color::MenuItem(false));
    Painter::FillRegion(x + 3, y + 4, Width() - 6, Menu::Item::HEIGHT - 6, Color::MenuItem(false));

    int delta = (pressed && (!shade)) ? 2 : 1;

    Painter::DrawStringInCenterRect(x + delta, y + delta, Width(), Menu::Item::HEIGHT, Title().CString(), color);

    funcForDraw(x, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::Draw(int x, int y)
{
    x += 22;
    y += 3;
    
    if (funcOfActive())
    {
        if (IsPressed())
        {
            Painter::FillRegion(x, y, WIDTH_SB, WIDTH_SB, Color::FILL);
            Painter::SetColor(Color::BLACK);
        }
        else
        {
            Painter::SetColor(Color::FILL);
        }
        funcForDraw(x, y);
    }
    else
    {
        Painter::SetColor(Color::FILL);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::Draw(int x, int y, bool opened)
{
    if(opened)
    {
        DrawTitle(x, y - Menu::Title::HEIGHT);

        DrawItems(x, y);
        if (CurrentItemIsOpened())
        {
            int8 posCurItem = PosCurrentItem();
            Control *item = Item(posCurItem);
            for (int i = 0; i < 5; i++)
            {
                if (Menu::ItemUnderButton((Key::E)(Key::F1 + i)) != item)
                {
                    Menu::SetItemUnderButton((Key::E)(Key::F1 + i), 0);
                }
            }
            x = ItemOpenedPosX(item);
            y = Menu::Y() - item->HeightOpened() + Menu::Item::HEIGHT + 1;

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
                ((TimeControl *)item)->Draw(x, y, true);
            }
            else
            {
                // остальные контролы не обрабатываем
            }
        }

        funcOnDraw();
    }
    else
    {
        Painter::FillRegion(x + 1, y + 2, Width() - 3, Menu::Item::HEIGHT - 2, Color::MenuItem(false));
        Painter::DrawStringInCenterRect(x, y + 1, Width(), Menu::Item::HEIGHT, Title().CString(), IsAcitve() ? Color::FILL : Color::MENU_TITLE_DARK);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawTitle(int x, int yTop)
{
    int eX = x;

    if (isPageSB)
    {
        SMALL_BUTTON_FROM_PAGE(this, 0)->Draw(x, yTop + 3);
        return;
    }

    bool shade = CurrentItemIsOpened();

    Painter::FillRegion(x - 1, yTop, Menu::Title::WIDTH + 2, Menu::Title::HEIGHT + 2, Color::BACK);

    Painter::DrawRectangle(x, yTop, Menu::Title::WIDTH + 1, Menu::Title::HEIGHT + 1, Color::BorderMenu(shade));

    if (shade)
    {
        Painter::FillRegion(x + 1, yTop + 1, Menu::Title::WIDTH - 1, Menu::Title::HEIGHT - 1, Color::MENU_TITLE_DARK);
        Painter::FillRegion(x + 4, yTop + 4, Menu::Title::WIDTH - 7, Menu::Title::HEIGHT - 7, Color::MENU_TITLE_DARK);
    }
    else
    {
        Painter::FillRegion(x + 1, yTop + 1, Menu::Title::WIDTH - 1, Menu::Title::HEIGHT - 1, Color::MenuTitle(false));
    }

    bool condDrawRSet = NumSubPages() > 1 &&
        NOT_CHOICE_REG(Menu::CurrentItem()) &&
        NOT_GOVERNOR(Menu::CurrentItem()) &&
        IS_PAGE(Menu::OpenedItem());

    int delta = condDrawRSet ? -10 : 0;
    Color colorText = Color::FILL;
    x = Painter::DrawStringInCenterRect(x, yTop, Menu::Title::WIDTH + 2 + delta, Menu::Title::HEIGHT, Title().CString(), colorText);
    if (condDrawRSet)
    {
        //Painter::Draw4SymbolsInRect(x + 4, yTop + 11, Governor::GetSymbol(CurrentSubPage()), colorText);
    }

    Menu::SetItemUnderButton(GetFuncButtonFromX(yTop), this);

    Painter::SetColor(Color::GRAY_75);
    DrawPagesUGO(eX + Menu::Title::WIDTH - 3, yTop + Menu::Title::HEIGHT);
    DrawNestingPage(eX + 5, yTop + Menu::Title::HEIGHT - 6);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawItems(int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        /// \todo Надо бы не делать это для пунктов меню, которые существуют и всё равно отрисовываться будут - зачем зря грузить процессор

        Painter::DrawRectangle(x, y + 1, Width() - 1, Menu::Item::HEIGHT, Color::FILL);
        Painter::FillRegion(x + 1, y + 2, Width() - 3, Menu::Item::HEIGHT - 2, Color::BACK);

        Control *item = Item(PosItemOnLeft() + i);
        if (item)
        {
            item->Draw(x, y, false);
            Key::E button = GetFuncButtonFromX(x);
            Menu::SetItemUnderButton(button, item);
        }

        x += Width();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::Draw(int x, int y, bool opened)
{
    if (type == Control::Type::Choice || type == Control::Type::ChoiceReg)
    {
        ((Choice *)this)->Draw(x, y, opened);
    }
    else if (type == Control::Type::Button)
    {
        ((Button *)this)->Draw(x, y);
    }
    else if (type == Control::Type::Page)
    {
        ((Page *)this)->Draw(x, y, opened);
    }
    else if (type == Control::Type::Governor)
    {
        ((Governor *)this)->Draw(x, y, opened);
    }
    else if (type == Control::Type::Time)
    {
        ((TimeControl *)this)->Draw(x, y, opened);
    }
    else if (type == Control::Type::GovernorColor)
    {
        ((GovernorColor *)this)->Draw(x, y, opened);
    }
    else if (type == Control::Type::DrawButton)
    {
        ((SButton *)this)->Draw(x, y);
    }
    else
    {
        // остальные типы контролов не обрабатываем
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeControl::Draw(int x, int y, bool opened)
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
void TimeControl::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade();
    DrawCommonHiPart(this, x, y, pressed, shade, false);

    Painter::FillRegion(x + 1, y + 17, Menu::Item::Value::WIDTH + 2, Menu::Item::Value::HEIGHT + 3, Color::MENU_FIELD);

    int deltaField = 10;
    int deltaSeparator = 2;
    int startX = 3;
    y += 21;
    PackedTime time = CPU::RTC_::GetPackedTime();
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
void TimeControl::DrawOpened(int x, int y)
{
    int width = Menu::Item::Value::WIDTH + 3;
    int height = 61;
    Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 3, Color::BACK);
    DrawCommonHiPart(this, x - 1, y - 1, IsPressed(), false, false);

    Painter::DrawRectangle(x - 1, y, width + 1, height + 1, Color::MenuTitle(false));

    Painter::DrawHLine(y + MOI_HEIGHT_TITLE - 1, x, x + Width());
    Painter::FillRegion(x, y + MOI_HEIGHT_TITLE, Width() - 1, height - MOI_HEIGHT_TITLE, Color::BLACK);

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
            Painter::FillRegion(x + strPaint[i].x - 1, y + strPaint[i].y, strPaint[i].width, 8, Color::FLASH_10);
        }
        String(strI[i]).Draw(x + strPaint[i].x, y + strPaint[i].y, *curField == i ? Color::FLASH_01 : Color::WHITE);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawCommonHiPart(Control *item, int x, int y, bool pressed, bool shade, bool opened)
{
    int delta = pressed && !shade ? 1 : 0;
    int width = Menu::Item::Value::WIDTH;

    Color color = shade ? Color::MENU_TITLE_DARK : Color::WHITE;

    Painter::FillRegion(x + 1, y + (opened ? 1 : 2), width + (opened ? 2 : 1), Menu::Item::Value::HEIGHT - (opened ? 2 : 3), Color::MenuItem(false));

    item->Title().Draw(x + delta + (opened ? 4 : 6), y + delta + (opened ? 2 : 3), color);

    if (opened)
    {
        Painter::DrawHLine(y + Menu::Item::Value::HEIGHT, x + 1, x + width + 3, Color::FILL);
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
            TimeControl* time = (TimeControl*)item;
            if ((Menu::OpenedItem() == item) && (*time->curField != iEXIT) && (*time->curField != iSET))
            {
                symbol = time->GetSymbol();
            }
        }
        else
        {
            // для остальных контролов не нужно
        }

        Painter::Draw4SymbolsInRect(x + item->Width() - 13, y + (item->IsOpened() ? 0 : 13), symbol, shade ? color : Color::BLACK);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawValueWithSelectedPosition(int x, int y, int value, uint numDigits, int selPosFromEnd, bool fillNull)
{
    Stack<uint8> stack(numDigits);
    
    for(int i = 0; i < numDigits; i++)
    {
        stack.Push(value % 10);
        value /= 10;
    }
    
    int height = 8;
    
    Color fill = Color::BACK;
    Color back = Color::FILL;
    
    
    for (int i = 0; i < numDigits; i++)
    {
        if (selPosFromEnd == (numDigits - i - 1))
        {
            Painter::FillRegion(x - 1, y, 5, height, back);
        }
        
        uint8 val = stack.Pop();
        
        Painter::DrawChar(x, y, (char)(val + 48), fill);
        
        x += 6;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::ItemOpenedPosX(Control *item)
{
    Page *page = (Page *)KEEPER(item);
    return (page->PosCurrentItem() % MENU_ITEMS_ON_DISPLAY) * item->Width();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawPagesUGO(int right, int bottom)
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
            Painter::FillRegion(x, top, size, size);
        }
        else
        {
            Painter::DrawRectangle(x, top, size, size);
        }
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawNestingPage(int left, int bottom)
{
    if (this != (Page *)Menu::pageMain)
    {
        int nesting = 0;

        Page *page = this;

        PageBase *parent = KEEPER(page);

        while (parent != Menu::pageMain)
        {
            page = (Page *)parent;
            parent = KEEPER(page);
            nesting++;                                  // -V127
        }

        int size = 4;
        int delta = 2;

        for (int i = 0; i <= nesting; i++)
        {
            int x = left + i * (size + delta);
            Painter::DrawRectangle(x, bottom, size, size);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::DrawHints(int x, int y, int width)
{
    if (numHints == 0)
    {
        return;
    }
    Painter::FillRegion(x, y, width, 239 - y, Color::BACK);
    Painter::DrawRectangle(x, y, width, 239 - y, Color::FILL);
    const StructHelpDrawButton *structHelp = &hintUGO[0];
    x += 3;
    y += 3;
    for (int i = 0; i < numHints; i++)
    {
        Painter::DrawRectangle(x, y, WIDTH_SB, WIDTH_SB);
        structHelp->funcDrawUGO(x, y);
        int yNew = Painter::DrawTextInRectWithTransfers(x + 23, y + 1, width - 30, 20, structHelp->helpUGO[LANG]);
        y = ((yNew - y) < 22) ? (y + 22) : yNew;
        structHelp++;
    }
}
