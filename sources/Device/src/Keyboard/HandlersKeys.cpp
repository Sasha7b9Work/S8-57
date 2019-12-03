#include "defines.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/ContextOsci.h"
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
    { // Press              Repead             Release           Long
        {Empty,             Empty,             Empty,            Empty},            // None       
        {Empty,             Empty,             Function_Release, Empty},            // Function   
        {Empty,             Empty,             Measures_Release, Empty},            // Measure    
        {Empty,             Empty,             Memory_Release,   Empty},            // Memory     
        {Empty,             Empty,             Service_Release,  Empty},            // Service    
        {Empty,             Empty,             ChannelA_Release, ChannelA_Long},    // ChannelA   
        {Empty,             Empty,             ChannelB_Release, ChannelB_Long},    // ChannelB   
        {Empty,             Empty,             OnTime_Release,   OnTime_Long},      // Time       
        {Start,             Empty,             Empty,            Empty},            // Start      
        {Empty,             Empty,             Trig_Release,     Trig_Long},        // Trig       
        {Empty,             Empty,             Display_Release,  Empty},            // Display    
        {RangeMoreA,        Empty,             Empty,            Empty},            // RangeMoreA 
        {RangeLessA,        Empty,             Empty,            Empty},            // RangeLessA 
        {RShiftMoreA,       RShiftMoreA,       Empty,            Empty},            // RShiftMoreA
        {RShiftLessA,       RShiftLessA,       Empty,            Empty},            // RShiftLessA
        {RangeMoreB,        Empty,             Empty,            Empty},            // RangeMoreB 
        {RangeLessB,        Empty,             Empty,            Empty},            // RangeLessB 
        {RShiftMoreB,       RShiftMoreB,       Empty,            Empty},            // RShiftMoreB
        {RShiftLessB,       RShiftLessB,       Empty,            Empty},            // RShiftLessB
        {OnTBase,           Empty,             Empty,            Empty},            // TBaseMore
        {OnTBase,           Empty,             Empty,            Empty},            // TBaseLess
        {OnTShift,          OnTShift,          Empty,            Empty},            // TShiftMore 
        {OnTShift,          OnTShift,          Empty,            Empty},            // TShiftLess 
        {TrigLevMore_Press, TrigLevMore_Press, Empty,            Empty},            // TrigLevMore
        {TrigLevLess_Press, TrigLevLess_Press, Empty,            Empty},            // TrigLevLess
        {OnArrow,           OnArrow,           OnArrow,          OnArrow},          // Left       
        {OnArrow,           OnArrow,           OnArrow,          OnArrow},          // Right      
        {OnArrow,           OnArrow,           OnArrow,          OnArrow},          // Up         
        {OnArrow,           OnArrow,           OnArrow,          OnArrow},          // Down       
        {Empty,             Empty,             OnEnter_Release,  OnEnter_Long},     // Enter      
        {OnFX,              Empty,             OnFX,             OnFX},             // F1
        {OnFX,              Empty,             OnFX,             OnFX},             // F2
        {OnFX,              Empty,             OnFX,             OnFX},             // F3
        {OnFX,              Empty,             OnFX,             OnFX},             // F4
        {OnFX,              Empty,             OnFX,             OnFX}              // F5
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
    if (!Device::State::InModeRecorder())
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

void Handlers::RShiftLessA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, -1);
}

void Handlers::RShiftMoreA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, 1);
}

void Handlers::RShiftLessB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, -1);
}

void Handlers::RShiftMoreB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, 1);
}

void Handlers::ChangeRange(Chan::E ch, int16 delta)
{
    Range(ch).Change(delta);
}

void Handlers::RangeLessA()
{
    OnChangeParameterChannel(ChangeRange, Chan::A, -1);
}


void Handlers::RangeMoreA()
{
    OnChangeParameterChannel(ChangeRange, Chan::A, +1);
}


void Handlers::RangeLessB()
{
    OnChangeParameterChannel(ChangeRange, Chan::B, -1);
}


void Handlers::RangeMoreB()
{
    OnChangeParameterChannel(ChangeRange, Chan::B, +1);
}


void Handlers::OnChangeParameterChannel(pFuncVChI16 func, Chan::E ch, int16 delta)
{
    if (Device::State::InModeOsci())
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
    static int prevDelta = 0;                       // Предыдущее направление перемещения
    static uint timeStartBrake = 0;                 // Время начала торможения

    if ((event.type == TypePress::Repeat) &&        // Если смещаемся не однокртаным нажатием кнопки
        (prevDelta == delta) &&                     // В том же направлении, что и в прошлый раз
        (timeStartBrake != 0) &&                    // И "тормоз" включён
        (TIME_MS - timeStartBrake < 500))           // и прошло ещё мало времени
    {
        return;                                     // то ничего не делаем
    }

    prevDelta = delta;
    timeStartBrake = 0;

    TShift().Change(delta);

    if ((TShift() == 0) && (event.type == TypePress::Repeat))   // Если новое пожение смещения - ноль, то включаем торможение
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

    if (Device::State::InModeRecorder())
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
    if (Menu::IsShown())
    {
        Item *openedItem = Menu::OpenedItem();

        if (!openedItem->Is(TypeItem::Page))
        {
            openedItem->HandlerKey(event);
        }
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

void Handlers::OnEnter_Release()
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

void Handlers::OnEnter_Long()
{
    return Menu::IsShown() ? Menu::Hide() : Menu::Show();
}

void Handlers::TrigLevMore_Press()
{
    TrigLevel().Change(1);
}


void Handlers::TrigLevLess_Press()
{
    TrigLevel().Change(-1);
}


void Handlers::ChannelA_Release()
{
    ShowHidePage(PageChannelA::self);
}


void Handlers::ChannelB_Release()
{
    ShowHidePage(PageChannelB::self);
}


void Handlers::ChannelA_Long()
{
    RShift(Chan::A, 0);
}


void Handlers::ChannelB_Long()
{
    RShift(Chan::B, 0);
}


void Handlers::Function_Release()
{
    ShowHidePage(PageFunction::self);
}


void Handlers::Measures_Release()
{
    ShowHidePage(PageMeasures::self);
}


void Handlers::Memory_Release()
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


void Handlers::Service_Release()
{
    ShowHidePage(PageService::self);
}


void Handlers::OnTime_Release()
{
    ShowHidePage(PageTime::self);
}


void Handlers::OnTime_Long()
{
    TShift().Reset();
}


void Handlers::Start()
{
    if (Device::State::InModeTester())
    {
        Tester::StartStop();
    }
    else
    {
        ContextOsci::OnPressStart();
    }
}


void Handlers::Trig_Release()
{
    ShowHidePage(PageTrig::self);
}


void Handlers::Trig_Long()
{
    TrigLevel().Set(0);
}


void Handlers::Display_Release()
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
