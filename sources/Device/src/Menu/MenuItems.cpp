#include "defines.h"
#include "Display/Symbols.h"
#include "MenuItems.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Hardware/Beeper.h"
#include "Utils/Math.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern int8 gCurDigit;

#define NAME_FROM_INDEX(index) (names[index])


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
Control *Page::Item(int numElement) const
{
    if (numElement >= num)
    {
        return 0;
    }

    return (Control *)items[numElement + (isPageSB ? 1 : 0)];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::PosItemOnLeft()
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
char TimeControl::GetSymbol()
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
char Control::GetSymbol()
{
    if (type == Control::Type::Governor)
    {
        return ((Governor *)this)->GetSymbol();
    }
    else if (type == Control::Type::Choice)
    {
        return ((Choice *)this)->GetSymbol();
    }
    else if (type == Control::Type::Time)
    {
        return ((TimeControl *)this)->GetSymbol();
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
void Page::ShortPressOnItem(uint numItem)
{
    ((Button *)items[numItem])->HandlePress();
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
int Control::HeightOpened() const
{
    if (type == Control::Type::Page)
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->CurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, 0, MENU_ITEMS_ON_DISPLAY); // -V2516
        return Menu::Title::HEIGHT + Menu::Item::HEIGHT * numItems;
    }
    else if (type == Control::Type::Choice || type == Control::Type::ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * MOSI_HEIGHT - 5;
    }
    else
    {
        // здесь ничего не делаем
    }

    return Menu::Item::HEIGHT;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsShade() const
{
    return (*keeper)->CurrentItemIsOpened() && (this != Menu::OpenedItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPressed() const
{
    return this == Menu::itemUnderKey;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::SetCurrent(bool active)
{
    Page *page = (Page *)*keeper;
    if (!active)
    {
        page->SetPosActItem(0x7f);
    }
    else
    {
        for (int i = 0; i < page->NumItems(); i++)
        {
            if (page->Item(i) == this)
            {
                page->SetPosActItem((int8)i);
                return;
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsOpened() const
{
    if (type == Control::Type::Page)
    {
        return (*keeper)->CurrentItemIsOpened();
    }
    return (MENU_POS_ACT_ITEM((*keeper)->name) & 0x80) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::Open(bool open)
{
    Page *parent = (Page *)keeper;
    parent->SetPosActItem(open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
    if(!open)
    {
        SetCurrent(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Control::Title() const
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
bool Control::ProcessKey(KeyEvent event)
{
    bool result = false;
    
    switch (type)
    {
    case Control::Type::Choice:
    case Control::Type::ChoiceReg:
        result = ((Choice *)this)->ProcessKey(event);
        break;

    case Control::Type::Page:
        result = ((Page *)this)->ProcessKey(event);
        break;

    case Control::Type::Governor:
        result = ((Governor *)this)->ProcessKey(event);
        break;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define CURRENT_ITEM_IS_OPENED return _GET_BIT(MENU_POS_ACT_ITEM(name), 7) == 1

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool PageBase::CurrentItemIsOpened() const
{
    return ((Page *)this)->CurrentItemIsOpened();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::CurrentItemIsOpened() const
{
    CURRENT_ITEM_IS_OPENED;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsCurrentItem() const
{
    return (this == Menu::CurrentItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::ShortPress()
{
    if(type == Control::Type::Choice)
    {
        Choice *choice = (Choice *)this;
        if(!IsActive())
        {
            choice->Change(false);
        }
        else if(!IsOpened())
        {
            choice->StartChange(1);
        }
        else
        {
            choice->ChangeIndex(1);
        }
    }
    else if(type == Control::Type::Button)
    {
        if(IsActive())
        {
            SetCurrent(true);
            ((Button *)this)->HandlePress();
        }
    }
    else if(type == Control::Type::Page)
    {
        Page *page = (Page *)this;
        if (page->funcOnEnterExit)
        {
            page->funcOnEnterExit(true);
        }
        page->SetAsCurrent();
    }
    else if(type == Control::Type::Governor)
    {
        if(IsActive())
        {
            Governor *governor = (Governor *)this;
            if(Menu::OpenedItem() == this)
            {
                governor->NextPosition();
            }
            else
            {
                governor->SetCurrent(!IsCurrentItem());
            }
        }
    }
    else if(type == Control::Type::Time)
    {
        TimeControl *time = (TimeControl *)this;
        if(!IsOpened())
        {
            SetCurrent(true);
            time->SetOpened();
            Open(true);
        }
        else
        {
            time->SelectNextPosition();
        }
    }
    else if(type == Control::Type::GovernorColor)
    {
        if(IsActive())
        {
            GovernorColor *governor = (GovernorColor *)this;
            if(Menu::OpenedItem() == this)
            {
                Math::CircleIncrease<int8>(&governor->ct->currentField, 0, 3);
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
    }
    else if(type == Control::Type::ChoiceReg)
    {
        Choice *choice = (Choice *)this;
        if(IsActive())
        {
            choice->SetCurrent(!IsCurrentItem());
        }
        else
        {
            choice->Change(false);
        }
    }
    else if(type == Control::Type::DrawButton)
    {
        SButton *button = (SButton *)this;
        button->funcOnPress();
    }
    else
    {
        // остальные типы контролов не обрабатываются
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::LongPress()
{
    if(!IsActive())
    {
        return;
    }
    
    if(type == Control::Type::Button)
    {
        ((Button *)this)->ShortPress();
    }
    else if(type == Control::Type::Time)
    {
        if(!IsCurrentItem())
        {
            SetCurrent(true);
        }
        TimeControl *time = (TimeControl *)this;
        if(IsOpened() && (*time->curField == iSET))
        {
            time->SetNewTime();
        }
        Open(!IsOpened());
        time->SetOpened();
    }
    else if(type == Control::Type::DrawButton)
    {
        SButton *button = (SButton *)this;
        button->funcOnPress();
        Menu::itemUnderKey = this;
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
void Page::SetPosActItem(int8 pos)
{
    MENU_POS_ACT_ITEM(name) = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetCurrentSubPage(int8 pos)
{
    set.menu_currentSubPage[name] = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::LogInfo() const
{
    LOG_WRITE("%x %s", this, Title().CString());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPage() const
{
    return (type == Control::Type::Page);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::ExistKeeper(const Page *_keeper)
{
    const Page *item = *keeper;
    while (item)
    {
        if (item == _keeper)
        {
            return true;
        }
        item = *item->keeper;
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
    Page *keep = KEEPER(this);

    while (keep)
    {
        if (keep == parent)
        {
            return true;
        }

        keep = KEEPER(keep);
    }

    return false;
}
