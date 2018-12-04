#include "defines.h"
#include "MenuItems.h"
#include "Settings/Settings.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Hardware/Sound.h"
#include "Utils/Math.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
extern int8 gCurDigit;

#define NAME_FROM_INDEX(index) (names[index * 2 + LANG])


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const char *Choice::NameCurrentSubItem()
{
    return ((int8 *)cell == 0) ? "" : NAME_FROM_INDEX(*cell); // names[*((int8 *)cell)][LANG];
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
    if(numElement >= num)
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
const char *Choice::NameSubItem(int i)
{
    return NAME_FROM_INDEX(i);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Choice::ColorMenuField(const Choice *choice)
{
    // Измерения по частоте
    if(choice == PageFunction::PageFrequencyCounter::GetChoiceTimeF())
    {
        return Color(Color::WHITE);
    }
    // Измерения по периоду
    else if(choice == PageFunction::PageFrequencyCounter::GetChoiceFreqClc() || choice == PageFunction::PageFrequencyCounter::GetChoiceNumPeriods())
    {
        return Color(Color::YELLOW);
    }

    return Color::MENU_FIELD;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ShortPressOnItem(int numItem)
{
    ((Button *)items[numItem])->funcOnPress();
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
        Sound::RegulatorSwitchRotate();
        SetCurrentSubPage(CurrentSubPage() + 1);
    }
    else if (delta < 0 && CurrentSubPage() > 0)
    {
        Sound::RegulatorSwitchRotate();
        SetCurrentSubPage(CurrentSubPage() - 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Control::HeightOpened() const
{
    if (type == Control::Type::Page)
    {
        int numItems = ((const Page *)this)->NumItems() - ((Page *)this)->CurrentSubPage() * MENU_ITEMS_ON_DISPLAY;
        LIMITATION(numItems, 0, MENU_ITEMS_ON_DISPLAY);
        return Menu::Title::HEIGHT + Menu::Item::HEIGHT * numItems;
    }
    else if (type == Control::Type::Choice || type == Control::Type::ChoiceReg)
    {
        return MOI_HEIGHT_TITLE + ((Choice *)this)->NumSubItems() * MOSI_HEIGHT - 5;
    }
    return Menu::Item::HEIGHT;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsShade() const
{
    return keeper->CurrentItemIsOpened() && (this != Menu::OpenedItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Control::IsPressed() const
{
    return this == Menu::itemUnderKey;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::SetCurrent(bool active)
{
    Page *page = (Page *)keeper;
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
        return keeper->CurrentItemIsOpened();
    }
    return (MENU_POS_ACT_ITEM(keeper->name) & 0x80) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::Open(bool open)
{
    Page *parent = (Page *)keeper;
    parent->SetPosActItem(open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Control::Title() const
{
    return titleHint[LANG];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ProcessKey(KeyEvent event)
{
    if (!funcKey(event))
    {
        if (event.type == TypePress::Press)
        {
            ChangeSubPage((event.key == Key::Left) ? -1 : 1);
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::ProcessKey(KeyEvent event)
{
    Key::E key = event.key;

    switch (type)
    {
    case Control::Type::Choice:
    case Control::Type::ChoiceReg:
        if (event.type == TypePress::Press)
        {
            int delta = (key == Key::Down || key == Key::Right) ? 1 : -1;

            Choice *choice = (Choice *)this;

            choice->ChangeIndex(Menu::IsShown() ? delta : -delta);
        }
        break;
    case Control::Type::Page:
        ((Page *)this)->ProcessKey(event);
        break;
    case Control::Type::Governor:
        ((Governor *)this)->ChangeValue((key == Key::Left || key == Key::Down) ? -1 : 1);
        break;
    }
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
void Control::ShortPress()
{
    if(type == Control::Type::Choice)
    {
        Choice *choice = (Choice *)this;
        if(!IsAcitve())
        {
            choice->funcOnChanged(false);
        }
        else if(!IsOpened())
        {
            choice->SetCurrent(Menu::CurrentItem() != this);
            choice->StartChange(1);
        }
        else
        {
            choice->ChangeIndex(1);
        }
    }
    else if(type == Control::Type::Button)
    {
        if(IsAcitve())
        {
            SetCurrent(true);
            ((Button *)this)->funcOnPress();
        }
    }
    else if(type == Control::Type::Page)
    {
        Page *page = (Page *)this;
        page->funcOnEnterExit(true);
        page->SetAsCurrent();
    }
    else if(type == Control::Type::Governor)
    {
        if(IsAcitve())
        {
            Governor *governor = (Governor *)this;
            if(Menu::OpenedItem() == this)
            {
                governor->NextPosition();
            }
            else
            {
                governor->SetCurrent(Menu::CurrentItem() != this);
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
        if(IsAcitve())
        {
            GovernorColor *governor = (GovernorColor *)this;
            if(Menu::OpenedItem() == this)
            {
                Math::CircleIncrease<int8>(&governor->ct->currentField, 0, 3);
            }
            else
            {
                if(Menu::CurrentItem() != this)
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
        if(IsAcitve())
        {
            choice->SetCurrent(Menu::CurrentItem() != this);
        }
        else
        {
            choice->funcOnChanged(false);
        }
    }
    else if(type == Control::Type::DrawButton)
    {
        SButton *button = (SButton *)this;
        button->funcOnPress();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Control::LongPress()
{
    if(type == Control::Type::Button)
    {
        ((Button *)this)->ShortPress();
    }
    else if(type == Control::Type::Time)
    {
        if(Menu::CurrentItem() != this)
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
        if(Menu::CurrentItem() != this)
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
