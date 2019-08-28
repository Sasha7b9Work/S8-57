#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Hardware/Beeper.h"
#include "Osci/Display/Osci_Display.h"
#include "Hardware/Timer.h"
#include "Hardware/Clock.h"
#include "Display/Symbols.h"
#include "Menu/Pages/Include/PageFunction.h"


typedef enum
{
    NONE,
    INCREASE,
    DECREASE
} DIRECTION;

/// Структура используется для анимации элементов ГУИ Choice и Governor
typedef struct
{
    void       *address;    ///< Адрес элемента. Если 0 - не движется
    uint        timeStart;  ///< Время начала анимации в миллисекундах
    DIRECTION   dir;        ///< Направление изменения значения
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
} TimeStruct;

static TimeStruct tsChoice = { 0, 0, NONE, 0, 0, 0 };

static TimeStruct tsGovernor = { 0, 0, NONE, 0, 0, 0 };

DataItem Item::emptyData =
{
    Item::Type::None,
    "",
    "",
    nullptr,
    E_BtV
};

Item Item::empty;

int8 Governor::currentDigit = 0;

#define NAME_FROM_INDEX(index) (OwnData()->names[index])

/// Здесь хранится адрес итема, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0,  если ни одна кнопка не нажата.
static const Item *pressedItem = nullptr;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Item::Item(const DataItem * const _data) : data(_data)
{
    if (data == nullptr)
    {
        data = &emptyData;
    }
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::KeyPress() const
{
    pressedItem = this;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::KeyRelease() const
{
    pressedItem = nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::KeyAutoRelease() const
{
    pressedItem = nullptr;

    if (!IsActive())
    {
        return;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::HeightOpened() const
{
    return Height();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsCurrentItem() const
{
    return (this == Menu::CurrentItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::Open(bool open) const
{
    Page *parent = (Page *)Keeper();
    if (parent)
    {
        parent->SetPosActItem(open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
        if (!open)
        {
            SetCurrent(false);
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Item::IsOpened() const
{
    const Page *parent = Keeper();

    if (parent == nullptr)
    {
        return false;
    }

    if (Is(Type::Page))
    {
        return parent->CurrentItemIsOpened();
    }
    return (MENU_POS_ACT_ITEM(parent->OwnData()->name) & 0x80) != 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Item::SetCurrent(bool active) const
{
    Page *page = (Page *)Keeper();

    if (page == nullptr)
    {
        return;
    }

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
bool Item::IsPressed() const
{
    return (this == pressedItem);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Item::Title() const
{
    return String(data->title);
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
int Item::Height() const
{
    return 23;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::Width(int pos) const
{
    int width = 320 / 5 + 1;

    if (pos != -1)
    {
        return ((pos + 1) % 5 == 0) ? width - 1 : width;
    }

    int position = PositionInKeeperList();

    if (position != -1)
    {
        return (position + 1) % 5 == 0 ? width - 1 : width;
    }

    return width;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::PositionOnScreenX() const
{
    int position = PositionInKeeperList();

    if (position != -1)
    {
        while (position > 4)
        {
            position -= 5;
        }

        return (Width(0) - 1) * position;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Item::PositionInKeeperList() const
{
    const Page *parent = Keeper();

    if (parent)
    {
        for (int i = 0; i < parent->NumItems(); i++)
        {
            if (this == parent->OwnData()->items[i])
            {
                return i;
            }
        }
    }

    return -1;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int Page::NumSubPages() const
{
    return (NumItems() - 1) / Item::NUM_ON_DISPLAY + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems() const //-V2506
{
    const Item * const * item = &OwnData()->items[0];

    int result = 0;

    while (*item)
    {
        item++;
        result++;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::ShortPress() const
{
    OwnData()->funcOnOpenClose(true);

    SetAsCurrent();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::KeyRelease() const
{
    Item::KeyRelease();

    ShortPress();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
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
PageName::E Page::GetName() const
{
    return (PageName::E)OwnData()->name;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::ProcessKey(KeyEvent event)
{
    if (OwnData()->funcOnKey(event))
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
void Page::SetAsCurrent() const
{
    if (IsActive())
    {
        SetCurrent(true);
        Open(!IsOpened());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Page::CurrentItemIsOpened() const
{
    return _GET_BIT(MENU_POS_ACT_ITEM(OwnData()->name), 7) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetPosActItem(int8 pos) const
{
    MENU_POS_ACT_ITEM(OwnData()->name) = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int8 Page::PosCurrentItem() const
{
    return MENU_POS_ACT_ITEM(OwnData()->name) & 0x7f;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Item *Page::GetItem(int numItem) const
{
    if (numItem >= NumItems())
    {
        return &Item::empty;
    }

    return (Item *)OwnData()->items[numItem];
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
int8 Page::CurrentSubPage() const
{
    return set.menu_currentSubPage[OwnData()->name];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetCurrentSubPage(int8 pos) const
{
    set.menu_currentSubPage[OwnData()->name] = pos;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::PosItemOnLeft() const
{
    return CurrentSubPage() * Item::NUM_ON_DISPLAY;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const Item *Page::ItemForFuncKey(Key::E key) const
{
    return GetItem(PosItemOnLeft() + key - Key::F1);
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Button::KeyRelease() const
{
    Item::KeyRelease();

    if (IsActive())
    {
        SetCurrent(true);

        OwnData()->funcOnPress();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Button::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    KeyRelease();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GraphButton::KeyRelease() const
{
    Item::KeyRelease();

    if (IsActive())
    {
        OwnData()->funcOnPress();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GraphButton::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    KeyRelease();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Governor::KeyRelease() const
{
    Item::KeyRelease();

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
void Governor::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::NextPosition() const
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleIncrease<int8>(&currentDigit, 0, (int8)(NumDigits() - 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::NextValue() const
{
    return (GetValue() < OwnData()->max) ? (GetValue() + 1) : OwnData()->min;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::PrevValue() const
{
    return (GetValue() > OwnData()->min) ? (GetValue() - 1) : OwnData()->max;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Governor::Step() const
{
    static const float speed = 0.05F;
    static const int numLines = 10;
    float delta = 0.0F;

    if (tsGovernor.address == this)
    {
        delta = speed * (TIME_MS - tsGovernor.timeStart);

        if (tsGovernor.dir == DECREASE)
        {
            delta *= -1.0F;
            if (delta == 0.0F)  // -V550 //-V2550 //-V550
            {
                return -0.001F;
            }
            if (delta < -numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(PrevValue());
                OwnData()->funcOnChanged();
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
        else if (tsGovernor.dir == INCREASE)
        {
            if (delta == 0.0F)  // -V550 //-V2550 //-V550
            {
                return 0.001F;
            }
            if (delta > numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(NextValue());
                OwnData()->funcOnChanged();
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
        else
        {
            // NONE
        }

    }
    return delta;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Governor::NumDigits() const
{
    int min = Integer(Math::Abs(OwnData()->min)).NumDigits();
    int max = Integer(Math::Abs(OwnData()->max)).NumDigits();
    if (min > max)
    {
        max = min;
    }
    return (uint)max;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::ChangeValue(int16 delta)
{
    if (!IsOpened())
    {
        currentDigit = 0;
    }

    int16 oldValue = GetValue();

    int16 newValue = GetValue() + (int16)(Math::Sign(delta) * Math::Pow10(currentDigit));

    LIMITATION(newValue, OwnData()->min, OwnData()->max); //-V2516

    SetValue(newValue);

    if (GetValue() != oldValue)
    {
        OwnData()->funcOnChanged();
        Beeper::GovernorChangedValue();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Governor::ProcessKey(KeyEvent event)
{
    if (event.IsLeft())
    {
        if (event.IsPress())
        {
            NextPosition();
        }
    }
    else if (event.IsRight())
    {
        if (event.IsPress())
        {
            PrevPosition();
        }
    }
    else if (event.IsUp() || event.IsDown())
    {
        if (event.IsPress() || event.IsRepeat())
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
void Governor::PrevPosition()
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleDecrease<int8>(&currentDigit, 0, (int8)(NumDigits() - 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int16 Governor::GetValue() const
{
    return *OwnData()->cell;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::SetValue(int16 v) const
{
    *OwnData()->cell = v;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Governor::GetSymbol() const
{
    static const char chars[] =
    {
        SYMBOL_GOVERNOR_SHIFT_0,
        SYMBOL_GOVERNOR_SHIFT_1,
        SYMBOL_GOVERNOR_SHIFT_2,
        SYMBOL_GOVERNOR_SHIFT_3
    };
    int value = GetValue();
    while (value < 0)
    {
        value += 4;
    }
    return chars[value % 4];
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Choice::ProcessKey(KeyEvent event)
{
    if (event.type == TypePress::Press)
    {
        Key::E key = event.key;

        int delta = (key == Key::Down || key == Key::Right) ? 1 : -1;

        ChangeIndex(Menu::IsShown() ? delta : -delta);

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::HeightOpened() const
{
    return MOI_HEIGHT_TITLE + NumChoices() * MOSI_HEIGHT - 5;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ChangeIndex(int delta) const
{
    int index = *OwnData()->cell;
    if (delta > 0)
    {
        ++index;
        if (index == NumChoices())
        {
            index = 0;
        }
    }
    else
    {
        --index;
        if (index < 0)
        {
            index = NumChoices() - 1;
        }
    }
    *OwnData()->cell = (int8)index;
    OwnData()->FuncOnChanged(IsActive());
    Beeper::GovernorChangedValue();
    Osci::Display::SetFlagRedraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::NumChoices() const
{
    pString *name = OwnData()->names;

    int result = 0;

    while (*name++)
    {
        result++;
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::KeyRelease() const
{
    Item::KeyRelease();

    if (!IsActive())
    {
        OwnData()->FuncOnChanged(false);
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::StartChange(int delta) const
{
    if (tsChoice.address == 0)
    {
        Beeper::GovernorChangedValue();

        if (HINT_MODE_ENABLED)
        {
            Menu::SetItemForHint(this);
        }
        else if (!IsActive())
        {
            OwnData()->FuncOnChanged(false);
        }
        else
        {
            tsChoice.address = (void *)this;
            tsChoice.timeStart = TIME_MS;
            tsChoice.dir = delta > 0 ? INCREASE : DECREASE;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Choice::GetSymbol()
{
    return ((Governor*)this)->GetSymbol();  // -V1027
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Choice::Step() const //-V2506
{
    static const float speed = 0.1F;
    static const int numLines = 12;

    if (tsChoice.address == this)
    {
        float delta = speed * (TIME_MS - tsChoice.timeStart);
        if (delta == 0.0F)  // -V550 //-V2550 //-V550
        {
            delta = 0.001F; // Таймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движения нет
        }
        int8 index = *OwnData()->cell;

        if (tsChoice.dir == INCREASE)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            Math::CircleIncrease<int8>(&index, 0, (int8)NumChoices() - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            Math::CircleDecrease<int8>(&index, 0, (int8)NumChoices() - 1);
        }
        else
        {
            // NONE
        }

        *OwnData()->cell = index;
        tsChoice.address = 0;
        OwnData()->FuncOnChanged(IsActive());
        Osci::Display::SetFlagRedraw();
        tsChoice.dir = NONE;
        return 0.0F;
    }

    return 0.0F;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Choice::NameCurrentSubItem() const
{
    return ((int8 *)OwnData()->cell == 0) ? String("") : String(NAME_FROM_INDEX(*OwnData()->cell));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NameNextSubItem() const
{
    if (OwnData()->cell == 0)
    {
        return "";
    }

    int index = *((int8 *)OwnData()->cell) + 1;

    if (index == NumChoices())
    {
        index = 0;
    }
    return NAME_FROM_INDEX(index);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const char *Choice::NamePrevSubItem() const
{
    if (OwnData()->cell == 0)
    {
        return "";
    }

    int index = *((int8 *)OwnData()->cell) - 1;

    if (index < 0)
    {
        index = NumChoices() - 1;
    }
    return NAME_FROM_INDEX(index);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Choice::NameSubItem(int i) const
{
    return String(NAME_FROM_INDEX(i));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Color Choice::ColorMenuField(const Choice *choice)
{
    // Измерения по частоте
    if (choice == PageFrequencyCounter::GetChoiceTimeF())
    {
        return Color(Color::WHITE);
    }
    // Измерения по периоду
    else if (choice == PageFrequencyCounter::GetChoiceFreqClc() || choice == PageFrequencyCounter::GetChoiceNumPeriods())
    {
        return Color(Color::YELLOW);
    }
    else
    {
        // здесь ничего не делаем
    }

    return Color::MenuItemField();
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void GovernorColor::KeyRelease() const
{
    Item::KeyRelease();

    if (IsActive())
    {
        if (Menu::OpenedItem() == this)
        {
            Math::CircleIncrease<int8>(&OwnData()->ct->currentField, 0, 3);
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
void GovernorColor::KeyAutoRelease() const
{
    Item::KeyAutoRelease();

    if (!IsCurrentItem())
    {
        SetCurrent(true);
    }
    Open(!IsOpened());
}

