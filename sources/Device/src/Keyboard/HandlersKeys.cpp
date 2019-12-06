#include "defines.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/HandlersKeys.h"
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/SettingsOsci.h"


KeyEvent Handlers::event;
Chan Handlers::drawingChan(Chan::A);



void Handlers::Process(KeyEvent e)
{
    event = e;

    static const pFuncVV func[Key::Count][4] =
    { // Press       Repead       Release      Long
        {Empty,      Empty,       Empty,       Empty},       // None       
        {Empty,      Empty,       OnFunction,  Empty},       // Function   
        {Empty,      Empty,       OnMeasures,  Empty},       // Measure    
        {Empty,      Empty,       OnMemory,    Empty},       // Memory     
        {Empty,      Empty,       OnService,   Empty},       // Service    
        {Empty,      Empty,       OnChannelA,  OnChannelA},  // ChannelA   
        {Empty,      Empty,       OnChannelB,  OnChannelB},  // ChannelB   
        {Empty,      Empty,       OnTime,      OnTime},      // Time       
        {OnStart,    Empty,       Empty,       Empty},       // Start      
        {Empty,      Empty,       OnTrig,      OnTrig},      // Trig       
        {Empty,      Empty,       OnDisplay,   Empty},       // Display    
        {OnRangeA,   Empty,       Empty,       Empty},       // RangeMoreA 
        {OnRangeA,   Empty,       Empty,       Empty},       // RangeLessA 
        {OnRShiftA,  OnRShiftA,   Empty,       Empty},       // RShiftMoreA
        {OnRShiftA,  OnRShiftA,   Empty,       Empty},       // RShiftLessA
        {OnRangeB,   Empty,       Empty,       Empty},       // RangeMoreB 
        {OnRangeB,   Empty,       Empty,       Empty},       // RangeLessB 
        {OnRShiftB,  OnRShiftB,   Empty,       Empty},       // RShiftMoreB
        {OnRShiftB,  OnRShiftB,   Empty,       Empty},       // RShiftLessB
        {OnTBase,    Empty,       Empty,       Empty},       // TBaseMore
        {OnTBase,    Empty,       Empty,       Empty},       // TBaseLess
        {OnTShift,   OnTShift,    Empty,       Empty},       // TShiftMore 
        {OnTShift,   OnTShift,    Empty,       Empty},       // TShiftLess 
        {OnTrigLev,  OnTrigLev,   Empty,       Empty},       // TrigLevMore
        {OnTrigLev,  OnTrigLev,   Empty,       Empty},       // TrigLevLess
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Left       
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Right      
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Up         
        {OnArrow,    OnArrow,     OnArrow,     OnArrow},     // Down       
        {Empty,      Empty,       OnEnter,     OnEnter},     // Enter      
        {OnFX,       Empty,       OnFX,        OnFX},        // F1
        {OnFX,       Empty,       OnFX,        OnFX},        // F2
        {OnFX,       Empty,       OnFX,        OnFX},        // F3
        {OnFX,       Empty,       OnFX,        OnFX},        // F4
        {OnFX,       Empty,       OnFX,        OnFX}         // F5
    };

    Key::E       key = e.key;
    TypePress::E type = e.type;

    if (key < Key::Count && type < TypePress::None)
    {
        if (!CommonHandlerPage())
        {
            func[key][type]();
        }
    }
}

void Handlers::Empty()
{
}

void Handlers::ChangeRShift(Chan::E ch, int16 delta)
{
    if (!Device::InModeRecorder())
    {
        static bool stop[Chan::Count] = { false, false };      // Признак того, что смещение изменять не нужно - оно равно нулю и прошло мало времени
        static uint timeStop[Chan::Count] = { 0, 0 };          // Время устновки признака stop

        if (stop[ch])
        {
            if (TIME_MS - timeStop[ch] > 500)
            {
                stop[ch] = false;
            }
        }
        else
        {
            RShift(ch).Change(delta);

            if (RShift(ch) == 0)
            {
                stop[ch] = true;
                timeStop[ch] = TIME_MS;
            }

            DisplayOsci::SetFlagRedraw();
        }
    }
}


void Handlers::OnRShiftA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, (event.key == Key::RShiftMoreA) ? 1 : -1);
}


void Handlers::OnRShiftB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, (event.key == Key::RShiftMoreB) ? 1 : -1);
}


void Handlers::ChangeRange(Chan::E ch, int16 delta)
{
    Range(ch).Change(delta);
}


void Handlers::OnRangeA()
{
    OnChangeParameterChannel(ChangeRange, Chan::A, (event.key == Key::RangeMoreA) ? 1 : -1);
}


void Handlers::OnRangeB()
{
    OnChangeParameterChannel(ChangeRange, Chan::B, (event.key == Key::RangeMoreB) ? 1 : -1);
}


void Handlers::OnChangeParameterChannel(pFuncVChI16 func, Chan::E ch, int16 delta)
{
    if (Device::InModeOsci())
    {
        drawingChan = Chan(ch);
    }

    func(ch, delta);
}


void Handlers::OnChangeParameterTime(pFuncVI func, int delta)
{
    func(delta);
}


void Handlers::ChangeTShift(int delta)
{
    static int prevDelta = 0;               // Предыдущее направление перемещения
    static uint timeStartBrake = 0;         // Время начала торможения

    if (event.IsRepeat() &&                 // Если смещаемся не однокртаным нажатием кнопки
        (prevDelta == delta) &&             // В том же направлении, что и в прошлый раз
        (timeStartBrake != 0) &&            // И "тормоз" включён
        (TIME_MS - timeStartBrake < 500))   // и прошло ещё мало времени
    {
        return;                             // то ничего не делаем
    }

    prevDelta = delta;
    timeStartBrake = 0;

    TShift().Change(delta);

    if ((TShift() == 0) && event.IsRepeat())   // Если новое пожение смещения - ноль, то включаем торможение
    {
        timeStartBrake = TIME_MS;
    }
}


void Handlers::OnTShift()
{
    OnChangeParameterTime(ChangeTShift, (event.key == Key::TShiftMore) ? 1 : -1);
}


void Handlers::ChangeTBase(int delta)
{
    TBase().Change(delta);
}


void Handlers::OnTBase()
{
    int delta = (event.key == Key::TBaseMore) ? 1 : -1;

    if (Device::InModeRecorder())
    {
        OnChangeParameterTime(RecorderScaleX::Change, delta);
    }
    else
    {
        OnChangeParameterTime(ChangeTBase, delta);
    }
}


void Handlers::OnFX()
{
    if (Menu::IsShown())
    {
        Menu::ItemUnderFunctionalKey(event.key)->HandlerFX(event.type);
    }
}


void Handlers::OnArrow()
{
    Item *openedItem = Menu::OpenedItem();

    if (Menu::IsShown())
    {
        if (!openedItem->Is(TypeItem::Page))
        {
            openedItem->HandlerKey(event);
        }
    }
    else if (openedItem == PageFFT::PageCursors::self)
    {
        openedItem->HandlerKey(event);
    }
    else
    {
        // здесь ничего
    }
}


bool Handlers::CommonHandlerPage()
{
    bool result = false;

    if (Menu::IsShown())
    {
        Item *openedPage = Menu::OpenedItem();

        if (!openedPage->Is(TypeItem::Page))
        {
        }
        else if (Menu::CurrentItem()->HandlerKey(event))
        {
            result = true;
        }
        else
        {
            result = openedPage->HandlerKey(event);
        }
    }

    return result;
}


void Handlers::OnEnter()
{
    if (event.IsRelease())
    {
        if (!Menu::IsShown())
        {
            Menu::Show();
        }
        else
        {
            Menu::CloseOpenedItem();
        }
    }
    else if (event.IsLong())
    {
        return Menu::IsShown() ? Menu::Hide() : Menu::Show();
    }
    else
    {
        // здесь ничего
    }
}


void Handlers::OnTrigLev()
{
    TrigLevel().Change(event.key == Key::TrigLevMore ? 1 : -1);
}


void Handlers::OnChannelA()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageChannelA::self);
    }
    else if (event.IsLong())
    {
        RShift(Chan::A).Set(0);
    }
    else
    {
        // здесь ничего
    }
}


void Handlers::OnChannelB()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageChannelB::self);
    }
    else if (event.IsLong())
    {
        RShift(Chan::B).Set(0);
    }
    else
    {
        // здесь ничего
    }
}


void Handlers::OnFunction()
{
    ShowHidePage(PageFunction::self);
}


void Handlers::OnMeasures()
{
    ShowHidePage(PageMeasures::self);
}


void Handlers::OnMemory()
{
    if (FDrive::IsConnected())
    {
        Display::SaveScreenToDrive();
    }
    else
    {
        ShowHidePage(PageMemory::self);
    }
}


void Handlers::OnService()
{
    ShowHidePage(PageService::self);
}


void Handlers::OnTime()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageTime::self);
    }
    else if (event.IsLong())
    {
        TShift().Reset();
    }
    else
    {
        // здесь ничего
    }
}


void Handlers::OnStart()
{
    if (Device::InModeTester())
    {
        Tester::StartStop();
    }
    else
    {
        Osci::OnPressStart();
    }
}


void Handlers::OnTrig()
{
    if (event.IsRelease())
    {
        ShowHidePage(PageTrig::self);
    }
    else if (event.IsLong())
    {
        TrigLevel().Set(0);
    }
    else
    {
        // здесь ничего
    }
}


void Handlers::OnDisplay()
{
    ShowHidePage(PageDisplay::self);
}


void Handlers::ShowHidePage(const Page *page)
{
    if (Menu::OpenedItem() == page)
    {
        Menu::IsShown() ? Menu::Hide() : Menu::Show();
    }
    else
    {
        Menu::mainPage = page;

        if (!Menu::IsShown())
        {
            Menu::Show();
        }
    }
}
