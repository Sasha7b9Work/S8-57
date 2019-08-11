#include "defines.h"
#include "Display/Symbols.h"
#include "MenuItems.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Hardware/Beeper.h"
#include "Utils/Math.h"


extern int8 gCurDigit;

#define NAME_FROM_INDEX(index) (names[index])

Item emptyControl = { Item::Type::None };


String Choice::NameCurrentSubItem() const
{
    return ((int8 *)cell == 0) ? String("") : String(NAME_FROM_INDEX(*cell));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameNextSubItem()
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) + 1;

    if (index == NumSubItems())
    {
        index = 0;
    }
    return NAME_FROM_INDEX(index);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NamePrevSubItem()
{
    if (cell == 0)
    {
        return "";
    }

    int index = *((int8 *)cell) - 1;

    if (index < 0)
    {
        index = NumSubItems() - 1;
    }
    return NAME_FROM_INDEX(index);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Item *Page::GetItem(int numItem) const
{
    if (numItem >= num)
    {
        return nullptr;
    }

    return (Item *)items[numItem + (isPageSB ? 1 : 0)];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::PosItemOnLeft() const
{
    return CurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Choice::NameSubItem(int i)
{
    return String(NAME_FROM_INDEX(i));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Choice::GetSymbol()
{
    return ((Governor*)this)->GetSymbol();  // -V1027
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char TimeItem::GetSymbol()
{
    int8 values[7] =
    {
        0,
        *day,
        *month,
        *year,
        *hours,
        *minutes,
        *seconds
    };

    int8 value = values[*curField];

#define NUM_POSITIONS 4

    static const char chars[NUM_POSITIONS] =
    {
        SYMBOL_GOVERNOR_SHIFT_0,
        SYMBOL_GOVERNOR_SHIFT_1,
        SYMBOL_GOVERNOR_SHIFT_2,
        SYMBOL_GOVERNOR_SHIFT_3
    };

    return chars[value % NUM_POSITIONS];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Item::GetSymbol() const
{
    if (type == Item::Type::Governor)
    {
        return ((Governor *)this)->GetSymbol();
    }
    else if (type == Item::Type::Choice)
    {
        return ((Choice *)this)->GetSymbol();
    }
    else if (type == Item::Type::Time)
    {
        return ((TimeItem *)this)->GetSymbol();
    }
    else
    {
        // здесь ничего не делаем
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Choice::ColorMenuField(const Choice *choice)
{
    // Измерения по частоте
    if(choice == PageFrequencyCounter::GetChoiceTimeF())
    {
        return Color(Color::WHITE);
    }
    // Измерения по периоду
    else if(choice == PageFrequencyCounter::GetChoiceFreqClc() || choice == PageFrequencyCounter::GetChoiceNumPeriods())
    {
        return Color(Color::YELLOW);
    }
    else
    {
        // здесь ничего не делаем
    }

    return Color::MenuItemField(choice->IsShade());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(name) & 0x7f;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ChangeSubPage(int delta)
{
    if (delta > 0 && CurrentSubPage() < NumSubPages() - 1)
    {
        Beeper::RegulatorSwitchRotate();
        SetCurrentSubPage(CurrentSubPage() + 1);
    }
    else if (delta < 0 && CurrentSubPage() > 0)
    {
        Beeper::RegulatorSwitchRotate();
        SetCurrentSubPage(CurrentSubPage() - 1);
    }
    else
    {
        // здесь ничего не делаем
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::HeightOpened() const
{
    if (type == Item::Type::Page)
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->CurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, 0, MENU_ITEMS_ON_DISPLAY); // -V2516
        return Menu::Title::HEIGHT + HEIGHT * numItems;
    }
    else if (type == Item::Type::Choice || type == Item::Type::ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * MOSI_HEIGHT - 5;
    }
    else
    {
        // здесь ничего не делаем
    }

    return HEIGHT;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsShade() const
{
    return Keeper()->CurrentItemIsOpened() && (this != Menu::OpenedItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsPressed() const
{
    return this == Menu::pressedItem;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::SetCurrent(bool active) const
{
    Page *page = (Page *)Keeper();

    if (!active)
    {
        page->SetPosActItem(0x7f);
    }
    else
    {
        for (int i = 0; i < page->NumItems(); i++)
        {
            if (page->GetItem(i) == this)
            {
                page->SetPosActItem((int8)i);
                return;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsOpened() const
{
    if (type == Item::Type::Page)
    {
        const Page *page = Keeper();

        return page->CurrentItemIsOpened();
    }
    return (MENU_POS_ACT_ITEM(Keeper()->name) & 0x80) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::Open(bool open) const
{
    Page *parent = (Page *)Keeper();
    parent->SetPosActItem(open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
    if(!open)
    {
        SetCurrent(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Item::Title() const
{
    return String(titleHint[0]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::ProcessKey(KeyEvent event)
{
    if (funcKey && funcKey(event))
    {
        return true;
    }
    else if (event.type == TypePress::Press)
    {
        ChangeSubPage((event.key == Key::Left) ? -1 : 1);
        return true;
    }
    else
    {
        // остальные типы событий не обрабатываются
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Governor::ProcessKey(KeyEvent event)
{
    if(event.IsLeft())
    {
        if(event.IsPress())
        {
            NextPosition();
        }
    }
    else if(event.IsRight())
    {
        if(event.IsPress())
        {
            PrevPosition();
        }
    }
    else if(event.IsUp() || event.IsDown())
    {
        if(event.IsPress() || event.IsRepeat())
        {
            ChangeValue(event.IsUp() ? 1 : -1);
        }
    }
    else
    {
        // остальные случаи не обрабатываем
    }
    
    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Choice::ProcessKey(KeyEvent event)
{
    if (event.type == TypePress::Press)
    {
        Key::E key = event.key;

        int delta = (key == Key::Down || key == Key::Right) ? 1 : -1;

        Choice *choice = (Choice *)this;

        choice->ChangeIndex(Menu::IsShown() ? delta : -delta);

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::ProcessKey(KeyEvent event)
{
    bool result = false;
    
    switch (type)
    {
    case Item::Type::Choice:
    case Item::Type::ChoiceReg:
        result = ((Choice *)this)->ProcessKey(event);
        break;

    case Item::Type::Page:
        result = ((Page *)this)->ProcessKey(event);
        break;

    case Item::Type::Governor:
        result = ((Governor *)this)->ProcessKey(event);
        break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CURRENT_ITEM_IS_OPENED return _GET_BIT(MENU_POS_ACT_ITEM(name), 7) == 1

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
//bool Page::CurrentItemIsOpened() const
//{
//    return ((Page *)this)->CurrentItemIsOpened();
//}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsCurrentItem() const
{
    return (this == Menu::CurrentItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::ShortPress() const
{
    if(type == Item::Type::Choice)
    {
        ((Choice *)this)->ShortPress();
    }
    else if (type == Item::Type::ChoiceReg)
    {
        ((Choice *)this)->ShortPress();
    }
    else if(type == Item::Type::Button)
    {
        ((Button *)this)->ShortPress();
    }
    else if(type == Item::Type::Page)
    {
        ((Page *)this)->ShortPress();
    }
    else if(type == Item::Type::Governor)
    {
        ((Governor *)this)->ShortPress();
    }
    else if(type == Item::Type::Time)
    {
        ((TimeItem *)this)->ShortPress();
    }
    else if(type == Item::Type::GovernorColor)
    {
        ((GovernorColor *)this)->ShortPress();
    }
    else if(type == Item::Type::DrawButton)
    {
        ((SButton *)this)->ShortPress();
    }
    else
    {
        // остальные типы контролов не обрабатываются
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::LongPress() const
{
    if(!IsActive())
    {
        return;
    }
    
    if(type == Item::Type::Button)
    {
        ((Button *)this)->ShortPress();
    }
    else if(type == Item::Type::Time)
    {
        if(!IsCurrentItem())
        {
            SetCurrent(true);
        }
        TimeItem *time = (TimeItem *)this;
        if(IsOpened() && (*time->curField == iSET))
        {
            time->SetNewTime();
        }
        Open(!IsOpened());
        time->SetOpened();
    }
    else if(type == Item::Type::DrawButton)
    {
        SButton *button = (SButton *)this;
        button->funcOnPress();
        Menu::pressedItem = this;
    }
    else
    {
        if(!IsCurrentItem())
        {
            SetCurrent(true);
        }
        Open(!IsOpened());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::CurrentSubPage() const
{
    return set.menu_currentSubPage[name];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetPosActItem(int8 pos) const
{
    MENU_POS_ACT_ITEM(name) = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetCurrentSubPage(int8 pos) const
{
    set.menu_currentSubPage[name] = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::LogInfo() const
{
    LOG_WRITE("%x %s", this, Title().CString());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsPage() const
{
    return (type == Item::Type::Page);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::ExistKeeper(const Page *_keeper) const
{
    const Page *item = Keeper();
    while (item)
    {
        if (item == _keeper)
        {
            return true;
        }
        item = item->Keeper();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::GetValue()
{
    return *cell;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::SetValue(int16 v)
{
    *cell = v;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::IsSubPage(const Page *parent)
{
    const Page *keep = Keeper();

    while (keep)
    {
        if (keep == parent)
        {
            return true;
        }

        keep = ((Item *)keep)->Keeper();
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::ShortPress() const
{
    if (IsActive())
    {
        SetCurrent(true);

        if (funcOnPress)
        {
            funcOnPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ShortPress() const
{
    if (funcOnEnterExit)
    {
        funcOnEnterExit(true);
    }

    SetAsCurrent();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::ShortPress() const
{
    if (IsActive())
    {
        if (Menu::OpenedItem() == this)
        {
            NextPosition();
        }
        else
        {
            SetCurrent(!IsCurrentItem());
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeItem::ShortPress() const
{
    if (!IsOpened())
    {
        SetCurrent(true);
        SetOpened();
        Open(true);
    }
    else
    {
        SelectNextPosition();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::ShortPress() const
{
    if (IsActive())
    {
        if (Menu::OpenedItem() == this)
        {
            Math::CircleIncrease<int8>(&ct->currentField, 0, 3);
        }
        else
        {
            if (!IsCurrentItem())
            {
                SetCurrent(true);
            }
            Open(!IsOpened());
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ShortPress() const
{
    if (IS_CHOICE(this))
    {
        if (!IsActive())
        {
            Change(false);
        }
        else if (!IsOpened())
        {
            StartChange(1);
        }
        else
        {
            ChangeIndex(1);
        }
    }
    else if (IS_CHOICE_REG(this))
    {
        if (IsActive())
        {
            SetCurrent(!IsCurrentItem());
        }
        else
        {
            Change(false);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void SButton::ShortPress() const
{
    funcOnPress();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const Item *Page::ItemForFuncKey(Key::E key)
{
    return GetItem(PosItemOnLeft() + key - Key::F1);
}
