#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Hardware/Beeper.h"
#include "Osci/Display/DisplayOsci.h"
#include "Hardware/Timer.h"
#include "Display/Symbols.h"
#include "Menu/Pages/Include/PageFunction.h"


enum DIRECTION
{
    NONE,
    INCREASE,
    DECREASE
};

// Структура используется для анимации элементов ГУИ Choice и Governor
struct TimeStruct
{
    void       *address;    // Адрес элемента. Если 0 - не движется
    uint        timeStart;  // Время начала анимации в миллисекундах
    DIRECTION   dir;        // Направление изменения значения
    uint8       notUsed0;
    uint8       notUsed1;
    uint8       notUsed2;
};

static TimeStruct tsChoice = { 0, 0, NONE, 0, 0, 0 };

static TimeStruct tsGovernor = { 0, 0, NONE, 0, 0, 0 };

DataItem Item::emptyData =
{
    TypeItem::None,
    "",
    "",
    nullptr,
    Item::Active
};

Item Item::empty;

int8 Governor::currentDigit = 0;

#define NAME_FROM_INDEX(index) (OwnData()->names[index])

// Здесь хранится адрес итема, соответствующего функциональной клавише [1..5], если она находится в нижнем положении, и 0,  если ни одна кнопка не нажата.
static const Item *pressedItem = nullptr;



Item::Item(const DataItem * const _data) : data(_data)
{
    if (data == nullptr)
    {
        data = &emptyData;
    }
};


void Item::HandlerFX(TypePress::E type) const
{
    if (type == TypePress::Press && IsActive())
    {
        pressedItem = this;
    }
    else if (type == TypePress::Release || type == TypePress::Long)
    {
        pressedItem = nullptr;
    }
}


int Item::HeightOpened() const
{
    return Height();
}


bool Item::IsCurrentItem() const
{
    return (this == Menu::CurrentItem());
}


void Item::Open(bool open) const
{
    Page *parent = const_cast<Page *>(Keeper());
    if (parent)
    {
        parent->SetPosActItem(open ? (parent->PosCurrentItem() | 0x80) : (parent->PosCurrentItem() & 0x7f));
        if (!open)
        {
            SetCurrent(false);
        }
    }
}


bool Item::IsOpened() const
{
    const Page *parent = Keeper();

    bool result = false;

    if (parent == nullptr)
    {
    }
    else if (Is(TypeItem::Page))
    {
        result = parent->CurrentItemIsOpened();
    }
    else
    {
        result = (Menu::Position::ActItem(static_cast<PageName::E>(parent->OwnData()->name)) & 0x80) != 0;
    }

    return result;
}


void Item::SetCurrent(bool active) const
{
    Page *page = const_cast<Page *>(Keeper());

    if (page)
    {
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
                    page->SetPosActItem(static_cast<int8>(i));
                    return;
                }
            }
        }
    }
}


bool Item::IsPressed() const
{
    return (this == pressedItem);
}


String Item::Title() const
{
    return String(data->title);
}


bool Item::ExistKeeper(const Page *_keeper) const
{
    const Page *item = Keeper();

    bool result = false;

    while (item)
    {
        if (item == _keeper)
        {
            result = true;
            break;
        }
        item = item->Keeper();
    }

    return result;
}


int Item::Height()
{
    return 23;
}


int Item::Width(int pos) const
{
    int width = 320 / 5 + 1;

    int result = width;

    if (pos != -1)
    {
        result = ((pos + 1) % 5 == 0) ? width - 1 : width;
    }
    else
    {
        int position = PositionInKeeperList();

        if (position != -1)
        {
            result = (position + 1) % 5 == 0 ? width - 1 : width;
        }
    }

    return result;
}


int Item::PositionOnScreenX() const
{
    int position = PositionInKeeperList();

    int result = 0;

    if (position != -1)
    {
        while (position > 4)
        {
            position -= 5;
        }

        result = (Width(0) - 1) * position;
    }

    return result;
}


int Item::PositionInKeeperList() const
{
    const Page *parent = Keeper();

    int result = -1;

    if (parent)
    {
        for (int i = 0; i < parent->NumItems(); i++)
        {
            if (this == parent->OwnData()->items[i])
            {
                result = i;
                break;
            }
        }
    }

    return result;
}



int Page::NumSubPages() const
{
    return (NumItems() - 1) / Item::NUM_ON_DISPLAY + 1;
}


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


void Page::HandlerFX(TypePress::E type) const
{
    Item::HandlerFX(type);

    if (type == TypePress::Release)
    {
        OwnData()->funcOnOpenClose(true);
        SetAsCurrent();
    }
    else if (type == TypePress::Long)
    {
        if (!IsCurrentItem())
        {
            SetCurrent(true);
        }
        Open(!IsOpened());
    }
}


bool Page::IsSubPage(const Page *parent)
{
    const Page *keep = Keeper();

    bool result = false;

    while (keep)
    {
        if (keep == parent)
        {
            result = true;
            break;
        }

        keep = (static_cast<Item *>(const_cast<Page *>(keep)))->Keeper();
    }

    return result;
}


PageName::E Page::GetName() const
{
    return static_cast<PageName::E>(OwnData()->name);
}


bool Page::HandlerKey(const KeyEvent &event)
{
    bool result = false;

    if (OwnData()->handlerKey(event))
    {
        result = true;
    }
    else if (event.IsPress())
    {
        if (event.IsArrowLeft())
        {
            ChangeSubPage(-1);
            result = true;
        }
        else if (event.IsArrowRight())
        {
            ChangeSubPage(1);
            result = true;
        }
    }
    else
    {
        // остальные типы событий не обрабатываются
    }

    return result;
}


void Page::SetAsCurrent() const
{
    if (IsActive())
    {
        SetCurrent(true);
        Open(!IsOpened());
    }
}


bool Page::CurrentItemIsOpened() const
{
    return _GET_BIT(Menu::Position::ActItem(static_cast<PageName::E>(OwnData()->name)), 7) == 1;
}


void Page::SetPosActItem(int8 pos) const
{
    Menu::Position::ActItem(static_cast<PageName::E>(OwnData()->name)) = pos;
}


int8 Page::PosCurrentItem() const
{
    return Menu::Position::ActItem(static_cast<PageName::E>(OwnData()->name)) & 0x7f;
}


Item *Page::GetItem(int numItem) const
{
    Item *result = &Item::empty;

    if (numItem < NumItems())
    {
        result = const_cast<Item *>(OwnData()->items[numItem]);
    }

    return result;
}


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
}


int8 Page::CurrentSubPage() const
{
    return set.menu.currentSubPage[OwnData()->name];
}


void Page::SetCurrentSubPage(int8 pos) const
{
    set.menu.currentSubPage[OwnData()->name] = pos;
}


int Page::PosItemOnLeft() const
{
    return CurrentSubPage() * Item::NUM_ON_DISPLAY;
}


const Item *Page::ItemForFuncKey(Key::E key) const
{
    return GetItem(PosItemOnLeft() + key - Key::F1);
}



void Button::HandlerFX(TypePress::E type) const
{
    Item::HandlerFX(type);

    if (type == TypePress::Release || type == TypePress::Long)
    {
         SetCurrent(true);
         OwnData()->handlerPress();
    }
}



void GraphButton::HandlerFX(TypePress::E type) const
{
    Item::HandlerFX(type);

    if (type == TypePress::Release || type == TypePress::Long)
    {
        OwnData()->handlerPress();
    }
}



void Governor::HandlerFX(TypePress::E type) const
{
    Item::HandlerFX(type);
    if (type == TypePress::Release)
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
    else if (type == TypePress::Long)
    {
        if (!IsCurrentItem())
        {
            SetCurrent(true);
        }
        Open(!IsOpened());
    }
}


void Governor::NextPosition() const
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleIncrease<int8>(&currentDigit, 0, static_cast<int8>(NumDigits() - 1));
    }
}


int16 Governor::NextValue() const
{
    return (GetValue() < OwnData()->max) ? (GetValue() + 1) : OwnData()->min;
}


int16 Governor::PrevValue() const
{
    return (GetValue() > OwnData()->min) ? (GetValue() - 1) : OwnData()->max;
}


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
                delta = -0.001F;
            }
            else if (delta < -numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(PrevValue());
                OwnData()->handlerChange();
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
        else if (tsGovernor.dir == INCREASE)
        {
            if (delta == 0.0F)  // -V550 //-V2550 //-V550
            {
                delta = 0.001F;
            }
            else if (delta > numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(NextValue());
                OwnData()->handlerChange();
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


uint Governor::NumDigits() const
{
    int min = Integer(Math::Abs(OwnData()->min)).NumDigits();
    int max = Integer(Math::Abs(OwnData()->max)).NumDigits();
    if (min > max)
    {
        max = min;
    }
    return static_cast<uint>(max);
}


void Governor::ChangeValue(int16 delta)
{
    if (!IsOpened())
    {
        currentDigit = 0;
    }

    int16 oldValue = GetValue();

    int16 newValue = GetValue() + static_cast<int16>(Math::Sign(delta) * Math::Pow10(currentDigit));

    LIMITATION(newValue, OwnData()->min, OwnData()->max);

    SetValue(newValue);

    if (GetValue() != oldValue)
    {
        OwnData()->handlerChange();
        Beeper::GovernorChangedValue();
    }
}


bool Governor::HandlerKey(const KeyEvent &event)
{
    if (event.IsArrowLeft())
    {
        if (event.IsPress())
        {
            NextPosition();

            return true;
        }
    }
    else if (event.IsArrowRight())
    {
        if (event.IsPress())
        {
            PrevPosition();

            return true;
        }
    }
    else if (event.IsArrowUp() || event.IsArrowDown())
    {
        if (event.IsPress() || event.IsRepeat())
        {
            ChangeValue(event.IsArrowUp() ? 1 : -1);

            return true;
        }
    }
    else
    {
        // остальные случаи не обрабатываем
    }

    return false;
}


void Governor::PrevPosition()
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleDecrease<int8>(&currentDigit, 0, static_cast<int8>(NumDigits() - 1));
    }
}


int16 Governor::GetValue() const
{
    return *OwnData()->cell;
}


void Governor::SetValue(int16 v) const
{
    *OwnData()->cell = v;
}


char Governor::GetSymbol() const
{
    static const char chars[] =
    {
        Symbol8::GOVERNOR_SHIFT_0,
        Symbol8::GOVERNOR_SHIFT_1,
        Symbol8::GOVERNOR_SHIFT_2,
        Symbol8::GOVERNOR_SHIFT_3
    };
    int value = GetValue();
    while (value < 0)
    {
        value += 4;
    }
    return chars[value % 4];
}



bool Choice::HandlerKey(const KeyEvent &event)
{
    bool result = false;

    if (event.IsPress())
    {
        int delta = (event.IsArrowDown() || event.IsArrowRight()) ? 1 : -1;

        ChangeIndex(Menu::IsShown() ? delta : -delta);

        result = true;
    }

    return result;
}


void Choice::HandlerFX(TypePress::E type) const
{
    Item::HandlerFX(type);

    if (type == TypePress::Release)
    {
        if (!IsActive())
        {
            OwnData()->handlerChange(false);
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
    else if (type == TypePress::Long)
    {
        if(IsActive())
        {
            if(!IsCurrentItem())
            {
                SetCurrent(true);
            }
            Open(!IsOpened());
        }
    }
}


int Choice::HeightOpened() const
{
    return MOI_HEIGHT_TITLE + NumChoices() * MOSI_HEIGHT - 5;
}


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
    *OwnData()->cell = static_cast<int8>(index);
    OwnData()->handlerChange(IsActive());
    Beeper::GovernorChangedValue();
    DisplayOsci::SetFlagRedraw();
}


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
            OwnData()->handlerChange(false);
        }
        else
        {
            tsChoice.address = const_cast<void *>(static_cast<const void *>(this));
            tsChoice.timeStart = TIME_MS;
            tsChoice.dir = delta > 0 ? INCREASE : DECREASE;
        }
    }
}


char Choice::GetSymbol()
{
    return (reinterpret_cast<Governor*>(this))->GetSymbol();  
}


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
            Math::CircleIncrease<int8>(&index, 0, static_cast<int8>(NumChoices()) - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            Math::CircleDecrease<int8>(&index, 0, static_cast<int8>(NumChoices()) - 1);
        }
        else
        {
            // NONE
        }

        *OwnData()->cell = index;
        tsChoice.address = 0;
        OwnData()->handlerChange(IsActive());
        DisplayOsci::SetFlagRedraw();
        tsChoice.dir = NONE;
        return 0.0F;
    }

    return 0.0F;
}


String Choice::NameCurrentSubItem() const
{
    return (OwnData()->cell == 0) ? String("") : String(NAME_FROM_INDEX(*OwnData()->cell));
}


const char *Choice::NameNextSubItem() const
{
    const char *result = "";

    if (OwnData()->cell != 0)
    {
        int index = *(static_cast<int8 *>(OwnData()->cell)) + 1;

        if (index == NumChoices())
        {
            index = 0;
        }

        result = NAME_FROM_INDEX(index);
    }

    return result;
}


const char *Choice::NamePrevSubItem() const
{
    const char *result = "";

    if (OwnData()->cell != 0)
    {
        int index = *(static_cast<int8 *>(OwnData()->cell)) - 1;

        if (index < 0)
        {
            index = NumChoices() - 1;
        }

        result = NAME_FROM_INDEX(index);
    }

    return result;
}


String Choice::NameSubItem(int i) const
{
    return String(NAME_FROM_INDEX(i));
}


Color Item::ColorBackground() const
{
    return Color::GRAY_50;
}


void GovernorColor::HandlerFX(TypePress::E type) const
{
    Item::HandlerFX(type);

    if (type == TypePress::Release)
    {
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
    else if (type == TypePress::Long)
    {
        if (!IsCurrentItem())
        {
            SetCurrent(true);
        }
        Open(!IsOpened());
    }
}


bool GovernorColor::HandlerKey(const KeyEvent &event)
{
    if (!event.IsRelease() && !event.IsRepeat())
    {
        return false;
    }

    ColorType *ct = OwnData()->ct;

    if (event.IsArrowLeft())
    {
        Math::CircleIncrease<int8>(&ct->currentField, 0, 3);

        return true;
    }
    else if (event.IsArrowRight())
    {
        Math::CircleDecrease<int8>(&ct->currentField, 0, 3);

        return true;
    }
    else if (ct->currentField == 0)       // яркость
    {
        if (event.IsArrowUp())
        {
            ct->BrightnessChange(1);

            return true;
        }
        else if (event.IsArrowDown())
        {
            ct->BrightnessChange(-1);

            return true;
        }
    }
    else
    {
        if (event.IsArrowUp())
        {
            ct->ComponentChange(1);

            return true;
        }
        else if (event.IsArrowDown())
        {
            ct->ComponentChange(-1);

            return true;
        }
    }

    return false;
}


const DataGovernorColor *GovernorColor::OwnData() const
{
    return static_cast<const DataGovernorColor *>(data->ad);
}
