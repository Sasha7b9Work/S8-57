#include "defines.h"
#include "MenuItems.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Display/Symbols.h"
#include "Display/Painter.h"

#include "Hardware/HAL/HAL.h"


using namespace Hardware;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

static TimeStruct tsChoice = {0, 0, NONE, 0, 0, 0};
static TimeStruct tsGovernor = {0, 0, NONE, 0, 0, 0};

int8 gCurDigit = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Choice::StartChange(int delta)
{
    if (tsChoice.address == 0)
    {
        Sound::GovernorChangedValue();

        if (HINT_MODE_ENABLED)
        {
            Menu::SetItemForHint(this);
        }
        else if (!IsAcitve())
        {
            CHOICE_RUN_FUNC_CHANGED(this, false);
        }
        else
        {
            tsChoice.address = this;
            tsChoice.timeStart = TIME_MS;
            tsChoice.dir = delta > 0 ? INCREASE : DECREASE;
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Choice::Step() //-V2506
{
    static const float speed = 0.1F;
    static const int numLines = 12;

    if (tsChoice.address == this)
    {
        float delta = speed * (TIME_MS - tsChoice.timeStart);
        if (delta == 0.0F)  // -V550
        {
            delta = 0.001F; // Таймер в несколько первых кадров может показать, что прошло 0 мс, но мы возвращаем большее число, потому что ноль будет говорить о том, что движения нет
        }
        int8 index = *cell;

        if (tsChoice.dir == INCREASE)
        {
            if (delta <= numLines)
            {
                return delta;
            }
            Math::CircleIncrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else if (tsChoice.dir == DECREASE)
        {
            delta = -delta;

            if (delta >= -numLines)
            {
                return delta;
            }
            Math::CircleDecrease<int8>(&index, 0, (int8)NumSubItems() - 1);
        }
        else
        {
            // NONE
        }

        *cell = index;
        tsChoice.address = 0;
        CHOICE_RUN_FUNC_CHANGED(this, IsAcitve());
        NEED_FINISH_DRAW = 1;
        tsChoice.dir = NONE;
        return 0.0F;
    }

    return 0.0F;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Choice::ChangeIndex(int delta)
{
    int index = *cell;
    if (delta > 0)
    {
        ++index;
        if (index == NumSubItems())
        {
            index = 0;
        }
    }
    else
    {
        --index;
        if (index < 0)
        {
            index = NumSubItems() - 1;
        }
    }
    *cell = (int8)index;
    CHOICE_RUN_FUNC_CHANGED(this, IsAcitve());
    Sound::GovernorChangedValue();
    NEED_FINISH_DRAW = 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Choice::NumSubItems()
{
    return num;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::StartChange(int delta)
{
    Sound::GovernorChangedValue();
    if (delta > 0 && tsGovernor.address == this && tsGovernor.dir == INCREASE)
    {
        SetValue(NextValue());
    }
    else if (delta < 0 && tsGovernor.address == this && tsGovernor.dir == DECREASE)
    {
        SetValue(NextValue());
    }
    else
    {
        tsGovernor.timeStart = TIME_MS;
        tsGovernor.address = this;
    }
    tsGovernor.dir = delta > 0 ? INCREASE : DECREASE;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Governor::NextValue()
{
    return (GetValue() < maxValue) ? (GetValue() + 1) : minValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Governor::PrevValue()
{
    return (GetValue() > minValue) ? (GetValue() - 1) : maxValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Governor::Step()
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
            if (delta == 0.0F)  // -V550
            {
                return -0.001F;
            }
            if (delta < -numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(PrevValue());
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
                delta = 0.0F;
                tsGovernor.address = 0;
            }
        }
        else if (tsGovernor.dir == INCREASE)
        {
            if (delta == 0.0F)  // -V550
            {
                return 0.001F;
            }
            if (delta > numLines)
            {
                tsGovernor.dir = NONE;
                SetValue(NextValue());
                if (funcOfChanged)
                {
                    funcOfChanged();
                }
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
void Governor::ChangeValue(int delta)
{
    if(!IsOpened())
    {
        gCurDigit = 0;
    }
    
    int oldValue = GetValue();

    int newValue = GetValue() + (int)(Math::Sign(delta) * Math::Pow10(gCurDigit));

    LIMITATION(newValue, minValue, maxValue); //-V2516

    SetValue(newValue);

    if (GetValue() != oldValue)
    {
        if (funcOfChanged)
        {
            funcOfChanged();
        }
        Sound::GovernorChangedValue();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::NextPosition()
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleIncrease<int8>(&gCurDigit, 0, (int8)(NumDigits() - 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Governor::PrevPosition()
{
    if (Menu::OpenedItem() == this)
    {
        Math::CircleDecrease<int8>(&gCurDigit, 0, (int8)(NumDigits() - 1));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Governor::NumDigits()
{
    int min = Integer(Math::Abs(minValue)).NumDigits();
    int max = Integer(Math::Abs(maxValue)).NumDigits();
    if (min > max)
    {
        max = min;
    }
    return (uint)max;
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Governor::GetSymbol()
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumSubPages() const
{
    return (NumItems() - 1) / MENU_ITEMS_ON_DISPLAY + 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Page::NumItems() const //-V2506
{
    if(PAGE_IS_MAIN)
    {
        return SHOW_DEBUG_MENU ? num : (num - 1);
    }
    return num;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Page::Name::E Page::GetName() const
{
    return (Page::Name::E)name;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeControl::SetOpened()
{
    PackedTime time = Clock::GetTime();
    *(seconds) = (int8)time.seconds;
    *(minutes) = (int8)time.minutes;
    *(hours) = (int8)time.hours;
    *(day) = (int8)time.day;
    *(month) = (int8)time.month;
    *(year) = (int8)time.year;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeControl::IncCurrentPosition()
{
    Sound::GovernorChangedValue();
    int8 *value[] = { 0, day, month, year, hours, minutes, seconds };
    int8 position = *curField;
    if (position != iSET && position != iEXIT)
    {
        static const int8 max[] = { 0, 31, 12, 99, 23, 59, 59 };
        static const int8 min[] = { 0, 1, 1, 15, 0, 0, 0 };

        int8 val = (*(value[position]))++;

        *(value[position]) = val;

        if (*value[position] > max[position])
        {
            *value[position] = min[position];
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeControl::SetNewTime()
{
    Clock::SetTime(*day, *month, *year, *hours, *minutes, *seconds);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeControl::SelectNextPosition()
{
    Math::CircleIncrease<int8>(curField, 0, 7);
    Color::ResetFlash();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void TimeControl::DecCurrentPosition()
{
    Sound::GovernorChangedValue();
    static const int8 max[] = {0, 31, 12, 99, 23, 59, 59};
    static const int8 min[] = {0, 1, 1, 15, 0, 0, 0};
    int8 *value[] = {0, day, month, year, hours, minutes, seconds};
    int8 position = *curField;
    if (position != iSET && position != iEXIT)
    {
        (*(value[position]))--;
        if (*value[position] < min[position])
        {
            *value[position] = max[position];
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Page::SetAsCurrent()
{
    if (funcOfActive())
    {
        if (SHOW_STRING_NAVI_TEMP)
        {
            MENU_TEMP_ENABLE_STRING_NAVI();
        }
        SetCurrent(true);
        Open(!IsOpened());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void GovernorColor::ChangeValue(int delta)
{
    if (ct->currentField == 0)
    {
        ct->BrightnessChange(delta);
        Sound::GovernorChangedValue();
    }
    else
    {
        ct->ComponentChange(delta);
        Sound::GovernorChangedValue();
    }

    Color::InitGlobalColors();
}
