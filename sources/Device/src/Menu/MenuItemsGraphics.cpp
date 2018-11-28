#include "defines.h"
#include "log.h"
#include "MenuItems.h"
#include "Osci/Grid.h"
#include "Display/Painter.h"
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
static void DrawGovernorChoiceColorFormulaHiPart(Control *item, int x, int y, bool pressed, bool shade, bool opened = false);
static void GovernorIpCommon_DrawOpened(Control *item, int x, int y, int dWidth);
static void DrawValueWithSelectedPosition(int x, int y, int value, int numDigits, int selPos, bool hLine, bool fillNull);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////q
///Возвращает функциональную кнопку, находящуюся на данном X.
/// Расстояние между левой и правой границами выведенного меню делится на пять раввных промежутков и возвращается название той кнопки, которая
/// попадает в даннуй промежуток.
static Key::E GetFuncButtonFromX(int _x)
{
    int x = Grid::Left() + Grid::Width() / 10;

    int step = Grid::Width() / 5;

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
    Painter::DrawRectangle(x, y, Menu::Item::WIDTH + delta, Menu::Item::HEIGHT, Color::MenuTitle(false));
    Painter::FillRegion(x + 1, y + 1, Menu::Item::Value::WIDTH + 2 + delta, Menu::Item::Value::HEIGHT + 3, Color::MenuItem(false));
    Painter::DrawHLine(y + Menu::Item::HEIGHT / 2 + 2, x, x + Menu::Item::WIDTH + delta, Color::MenuTitle(false));
    Painter::DrawStringInCenterRect(x + (IsPressed() ? 2 : 1), y + (IsPressed() ? 2 : 1), Menu::Item::WIDTH + delta, Menu::Item::HEIGHT / 2 + 2, 
                                    Title(), Color::WHITE);
    DrawValue(x + 1, y + 19, delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawClosed(int x, int y)
{
    ct->Init(false);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, IsPressed(), IsShade() || !IsAcitve());
    Painter::FillRegion(x + 2, y + 20, Menu::Item::Value::WIDTH, Menu::Item::Value::HEIGHT - 1, ct->color);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::DrawValue(int x, int y, int delta)
{
    char buffer[20];

    int8 field = ct->currentField;
    char *texts[4] = {"Яр", "Сн", "Зл", "Кр"};

    uint color = COLOR(ct->color.value);
    uint red = R_FROM_COLOR(color);
    uint green = G_FROM_COLOR(color);
    uint blue = B_FROM_COLOR(color);
    ct->Init(false);
    int16 vals[4] = {(int16)(ct->brightness * 100.0f), (int16)blue, (int16)green, (int16)red};

    Painter::FillRegion(x, y, Menu::Item::WIDTH + delta - 2, Menu::Item::HEIGHT / 2 - 3, Color::BLACK);
    x += 92;

    for (int i = 0; i < 4; i++)
    {
        Color colorBack = (field == i) ? Color::WHITE : Color::BLACK;
        Color colorDraw = (field == i) ? Color::BLACK : Color::WHITE;
        Painter::FillRegion(x - 1, y + 1, 29, 10, colorBack);
        Painter::DrawText(x, y + 2, texts[i], colorDraw);
        Painter::DrawText(x + 14, y + 2, Integer(vals[i]).ToString(false, 1, buffer));
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
    GovernorIpCommon_DrawOpened(this, x, y, 0);
    DrawValue(x, y + 22);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawClosed(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !IsAcitve();
    DrawLowPart(x, y, pressed, shade);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawValue(int x, int y)
{
    char buffer[20];

    int startX = x + 40;
    int16 value = *cell;
    int signGovernor = *cell < 0 ? -1 : 1;
    if (signGovernor == -1)
    {
        value = -value;
    }
    Painter::SetFont(Font::Type::_5);
    bool sign = minValue < 0;
    Painter::DrawText(x + 55, y - 5, Integer(maxValue).ToString(sign, 1, buffer), Color::WHITE);
    Painter::DrawText(x + 55, y + 2, Integer(minValue).ToString(sign, 1, buffer));
    Painter::SetFont(Font::Type::_8);

    DrawValueWithSelectedPosition(startX, y, value, NumDigits(), gCurDigit, true, true);

    if (sign)
    {
        Painter::DrawChar(startX - 30, y, signGovernor < 0 ? '\x9b' : '\x9a');
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::DrawLowPart(int x, int y, bool, bool shade)
{
    char buffer[20];

    Color colorTextDown = Color::BLACK;

    Painter::FillRegion(x + 1, y + 17, Menu::Item::Value::WIDTH + 2, Menu::Item::Value::HEIGHT + 3, Color::MENU_FIELD);
    if (shade)
    {
        colorTextDown = Color::MenuItem(false);
    }

    x = Painter::DrawText(x + 4, y + 21, "\x80", colorTextDown);
    if (Menu::OpenedItem() != this)
    {
        int delta = (int)Step();
        if (delta == 0)
        {
            x = Painter::DrawText(x + 1, y + 21, Integer(*cell).ToString(false, 1, buffer));
        }
        else
        {
            int drawX = x + 1;
            int limX = x + 1;
            int limY = y + 19;
            int limWidth = Menu::Item::Value::WIDTH;
            int limHeight = Menu::Item::Value::HEIGHT - 1;
            Painter::SetColor(Color::BLACK);
            if (delta > 0)
            {
                x = Painter::DrawTextWithLimitation(drawX, y + 21 - delta, Integer(*cell).ToString(false, 1, buffer), limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitation(drawX, y + 21 + 10 - delta, Integer(NextValue()).ToString(false, 1, buffer), limX, limY, limWidth, limHeight);
            }
            if (delta < 0)
            {
                x = Painter::DrawTextWithLimitation(drawX, y + 21 - delta, Integer(*cell).ToString(false, 1, buffer), limX, limY, limWidth, limHeight);
                Painter::DrawTextWithLimitation(drawX, y + 21 - 10 - delta, Integer(PrevValue()).ToString(false, 1, buffer), limX, limY, limWidth, limHeight);
            }
        }
    }
    else
    {
        x = Painter::DrawText(x + 1, y + 21, Integer(*cell).ToString(false, 1, buffer), Color::WHITE);
    }
    Painter::DrawText(x + 1, y + 21, "\x81", colorTextDown);
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
    
    Painter::DrawRectangle(x, y, Menu::Item::WIDTH, height, Color::FILL);
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, IsPressed(), false, true);

    Painter::FillRegion(x + 1, y + MOI_HEIGHT_TITLE - 5, Menu::Item::WIDTH - 2, height - MOI_HEIGHT_TITLE + 4, Color::BACK);
    int8 index = *cell;
    for (int i = 0; i < NumSubItems(); i++)
    {
        int yItem = y + MOI_HEIGHT_TITLE + i * MOSI_HEIGHT - 7;
        bool pressed = i == index;
        if (pressed)
        {
            Painter::FillRegion(x + 1, yItem + 2, Menu::Item::WIDTH_OPENED - 2, MOSI_HEIGHT - 1, ColorMenuField(this));
        }
        Painter::DrawText(x + 4, yItem + 2, NameSubItem(i), pressed ? Color::BLACK : ColorMenuField(this));
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
        Painter::DrawText(x + 4, y + Menu::Item::Value::HEIGHT + 1, NameCurrentSubItem());
    }
    else
    {
        Painter::SetColor(Color::BACK);
        Painter::DrawTextWithLimitation(x + 4, y + Menu::Item::Value::HEIGHT - deltaY + 1, NameCurrentSubItem(), x, y + 11, Menu::Item::Value::WIDTH, Menu::Item::Value::HEIGHT - 1);
        Painter::DrawHLine(y + (deltaY > 0 ? 24 : 19) - deltaY, x + 1, x + Menu::Item::Value::WIDTH + 2);
        Painter::DrawTextWithLimitation(x + 4, y + (deltaY > 0 ? (Menu::Item::Value::HEIGHT + 13) : 9) - deltaY, deltaY > 0 ? NameNextSubItem() : NamePrevSubItem(),
                                        x, y + 11, Menu::Item::Value::WIDTH, Menu::Item::Value::HEIGHT - 1);
    }
  
    if (funcForDraw)
    {
        funcForDraw(x, y);
    }
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::Draw(int x, int y)
{
    bool pressed = IsPressed();
    bool shade = IsShade() || !funcOfActive();

    Painter::DrawHLine(y + 1, x, x + Menu::Item::WIDTH, Color::MenuTitle(shade));
    Color color = shade ? Color::MenuItem(true) : Color::WHITE;
    Painter::FillRegion(x + 1, y + 2, Menu::Item::WIDTH - 2, Menu::Item::HEIGHT - 2, Color::MenuItem(false));
    Painter::FillRegion(x + 3, y + 4, Menu::Item::WIDTH - 6, Menu::Item::HEIGHT - 6, Color::MenuItem(false));

    int delta = (pressed && (!shade)) ? 2 : 1;

    Painter::DrawStringInCenterRect(x + delta, y + delta, Menu::Item::WIDTH, Menu::Item::HEIGHT, Title(), color);

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
                if (Menu::itemUnderButton[i + Key::F1] != item)
                {
                    Menu::itemUnderButton[i + Key::F1] = 0;
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
        }

        funcOnDraw();
    }
    else
    {
        Painter::FillRegion(x + 1, y + 2, Menu::Item::WIDTH - 3, Menu::Item::HEIGHT - 2, Color::MenuItem(false));
        Painter::DrawStringInCenterRect(x, y + 1, Menu::Item::WIDTH, Menu::Item::HEIGHT, Title(), IsAcitve() ? Color::FILL : Color::MENU_TITLE_DARK);
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

    bool condDrawRSet = NumSubPages() > 1 && NOT_CHOICE_REG(Menu::CurrentItem()) &&
        NOT_GOVERNOR(Menu::CurrentItem()) && IS_PAGE(Menu::OpenedItem());
    int delta = condDrawRSet ? -10 : 0;
    Color colorText = Color::FILL;
    x = Painter::DrawStringInCenterRect(x, yTop, Menu::Title::WIDTH + 2 + delta, Menu::Title::HEIGHT, Title(), colorText);
    if (condDrawRSet)
    {
        //Painter::Draw4SymbolsInRect(x + 4, yTop + 11, Governor::GetSymbol(CurrentSubPage()), colorText);
    }

    Menu::itemUnderButton[GetFuncButtonFromX(yTop)] = this;

    delta = 0;

    Painter::SetColor(Color::GRAY_75);
    DrawPagesUGO(eX + Menu::Title::WIDTH - 3 + delta, yTop + Menu::Title::HEIGHT + delta);
    DrawNestingPage(eX + 5, yTop + Menu::Title::HEIGHT - 6);
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::DrawItems(int x, int y)
{
    for (int i = 0; i < 5; i++)
    {
        /// \todo Надо бы не делать это для пунктов меню, которые существуют и всё равно отрисовываться будут - зачем зря грузить процессор

        Painter::DrawRectangle(x, y + 1, Menu::Item::WIDTH - 1, Menu::Item::HEIGHT, Color::FILL);
        Painter::FillRegion(x + 1, y + 2, Menu::Item::WIDTH - 3, Menu::Item::HEIGHT - 2, Color::BACK);

        Control *item = Item(PosItemOnLeft() + i);
        if (item)
        {
            item->Draw(x, y, false);
            Menu::itemUnderButton[GetFuncButtonFromX(x)] = item;
        }

        x += Menu::Item::WIDTH;
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
    char buffer[20];

    bool pressed = IsPressed();
    bool shade = IsShade();
    DrawGovernorChoiceColorFormulaHiPart(this, x, y, pressed, shade);

    Painter::FillRegion(x + 1, y + 17, Menu::Item::Value::WIDTH + 2, Menu::Item::Value::HEIGHT + 3, Color::MENU_FIELD);

    int deltaField = 10;
    int deltaSeparator = 2;
    int startX = 3;
    y += 21;
    PackedTime time = CPU::RTC_::GetPackedTime();
    Painter::DrawText(x + startX, y, Integer((int)time.hours).ToString(false, 2, buffer), shade ? Color::MenuItem(true) : Color::BLACK);
    Painter::DrawText(x + startX + deltaField, y, ":");
    Painter::DrawText(x + startX + deltaField + deltaSeparator, y, Integer((int)time.minutes).ToString(false, 2, buffer));
    Painter::DrawText(x + startX + 2 * deltaField + deltaSeparator, y, ":");
    Painter::DrawText(x + startX + 2 * deltaField + 2 * deltaSeparator, y, Integer((int)time.seconds).ToString(false, 2, buffer));

    startX = 44;
    Painter::DrawText(x + startX, y, Integer((int)time.day).ToString(false, 2, buffer));
    Painter::DrawText(x + startX + deltaField, y, ":");
    Painter::DrawText(x + startX + deltaField + deltaSeparator, y, Integer((int)time.month).ToString(false, 2, buffer));
    Painter::DrawText(x + startX + 2 * deltaField + deltaSeparator, y, ":");
    Painter::DrawText(x + startX + 2 * deltaField + 2 * deltaSeparator, y, Integer((int)time.year).ToString(false, 2, buffer));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeControl::DrawOpened(int x, int y)
{
    char buffer[20];

    int width = Menu::Item::Value::WIDTH + 3;
    int height = 61;
    Painter::DrawRectangle(x - 1, y - 1, width + 2, height + 3, Color::BACK);
    DrawGovernorChoiceColorFormulaHiPart(this, x - 1, y - 1, IsPressed(), false);

    Painter::DrawRectangle(x - 1, y, width + 1, height + 1, Color::MenuTitle(false));

    Painter::DrawHLine(y + MOI_HEIGHT_TITLE - 1, x, x + Menu::Item::WIDTH_OPENED);
    Painter::FillRegion(x, y + MOI_HEIGHT_TITLE, Menu::Item::WIDTH_OPENED - 1, height - MOI_HEIGHT_TITLE, Color::BLACK);

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
    std::strcpy(strI[iDAY],   Integer(*day).ToString(false, 2, buffer));
    std::strcpy(strI[iMONTH], Integer(*month).ToString(false, 2, buffer));
    std::strcpy(strI[iYEAR],  Integer(*year).ToString(false, 2, buffer));
    std::strcpy(strI[iHOURS], Integer(*hours).ToString(false, 2, buffer));
    std::strcpy(strI[iMIN],   Integer(*minutes).ToString(false, 2, buffer));
    std::strcpy(strI[iSEC],   Integer(*seconds).ToString(false, 2, buffer));
    std::strcpy(strI[iSET],   "Сохранить");

    Painter::DrawText(x + 3, y + y0, "д м г - ", Color::WHITE);
    Painter::DrawText(x + 3, y + y1, "ч м с - ");

    for (int i = 0; i < 8; i++)
    {
        if (*curField == i)
        {
            Painter::FillRegion(x + strPaint[i].x - 1, y + strPaint[i].y, strPaint[i].width, 8, Color::FLASH_10);
        }
        Painter::DrawText(x + strPaint[i].x, y + strPaint[i].y, strI[i], *curField == i ? Color::FLASH_01 : Color::WHITE);
    }
}

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void DrawGovernorChoiceColorFormulaHiPart(Control *item, int x, int y, bool pressed, bool shade, bool opened)
{
    int delta = pressed && !shade ? 1 : 0;
    int width = Menu::Item::Value::WIDTH;

    Color color = shade ? Color::MENU_TITLE_DARK : Color::WHITE;

    Painter::FillRegion(x + 1, y + (opened ? 1 : 2), width + (opened ? 2 : 1), Menu::Item::Value::HEIGHT - (opened ? 2 : 3), Color::MenuItem(false));

    Painter::DrawText(x + delta + (opened ? 4 : 6), y + delta + (opened ? 2 : 3), item->Title(), color, true);

    if (opened)
    {
        Painter::DrawHLine(y + Menu::Item::Value::HEIGHT, x + 1, x + width + 3, Color::FILL);
    }

    if(Menu::CurrentItem() == item)
    {
        char symbol = 0;
   
        if (IS_GOVERNOR(item))
        {
            symbol = Governor::GetSymbol(*((Governor*)item)->cell);
        }
        else if (IS_GOVERNOR(item) || IS_CHOICE_REG(item) ||  (item->IsOpened() && IS_CHOICE(item)))
        {
            symbol = Governor::GetSymbol(*((Choice*)item)->cell);
        }
        else if (IS_TIME(item))
        {
            TimeControl* time = (TimeControl*)item;
            if ((Menu::OpenedItem() == item) && (*time->curField != iEXIT) && (*time->curField != iSET))
            {
                int8 values[7] =
                {
                    0,
                    *time->day,
                    *time->month,
                    *time->year,
                    *time->hours,
                    *time->minutes,
                    *time->seconds
                };
                symbol = Governor::GetSymbol(values[*time->curField]);
            }
        }

        Painter::Draw4SymbolsInRect(x + Menu::Item::WIDTH - 13, y + 5 + (item->IsOpened() ? 0 : 15), symbol, shade ? color : Color::BLACK);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Если selPos == -1, подсвечивать не нужно
static void DrawValueWithSelectedPosition(int x, int y, int value, int numDigits, int selPos, bool hLine, bool fillNull)
{
    int firstValue = value;
    int height = hLine ? 9 : 8;
    for (int i = 0; i < numDigits; i++)
    {
        int rest = value % 10;
        value /= 10;
        if (selPos == i)
        {
            Painter::FillRegion(x - 1, y, 5, height, Color::FILL);
        }
        if (!(rest == 0 && value == 0) || (firstValue == 0 && i == 0))
        {
            Painter::DrawChar(x, y, (char)(rest + 48), selPos == i ? Color::BLACK : Color::WHITE);
        }
        else if (fillNull)
        {
            Painter::DrawChar(x, y, '0', selPos == i ? Color::BLACK : Color::WHITE);
        }
        if (hLine)
        {
            Painter::DrawLine(x, y + 9, x + 3, y + 9, Color::WHITE);
        }
        x -= 6;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void GovernorIpCommon_DrawOpened(Control *item, int x, int y, int dWidth)
{
    int height = 34;
    Control *control = (Control *)item;
    Painter::DrawRectangle(x - 2, y - 1, Menu::Title::WIDTH + 3 + dWidth, height + 3, Color::BACK);
    Painter::DrawRectangle(x - 1, y, Menu::Title::WIDTH + 1 + dWidth, height + 1, Color::MenuTitle(false));
    Painter::DrawHLine(y + MOI_HEIGHT_TITLE - 1, x, x + Menu::Item::WIDTH_OPENED + dWidth);
    DrawGovernorChoiceColorFormulaHiPart(item, x - 1, y - 1, control->IsPressed(), false);
    Painter::FillRegion(x, y + MOI_HEIGHT_TITLE, Menu::Item::WIDTH_OPENED - 1 + dWidth, height - MOI_HEIGHT_TITLE, Color::BLACK);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::ItemOpenedPosX(Control *item)
{
    Page *page = (Page *)KEEPER(item);
    return (page->PosCurrentItem() % MENU_ITEMS_ON_DISPLAY) * Menu::Item::WIDTH;
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
            nesting++;
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
