#include "defines.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/HandlersKeys.h"
#include "Keyboard/Keyboard.h"
#include "Menu/MenuItems.h"
#include "Osci/Display/DisplayOsci.h"


namespace Handlers
{
    // Обрабатываемое событие
    static KeyEvent event;

    // Общий обработчик изменения параметра канала - масштаба или смещения
    static void OnChangeParameterChannel(pFuncVChI16, Chan::E, int16);

    // Пустой обработчик
    static void Empty();

    // Общий обработчик раскрытой страницы. Возвращает true, если отработал и дальнейшая обработка события клавиатуры не требуется.
    static bool CommonHandlerPage();

    // Общий обработчик изменения временных параметров
    static void OnChangeParameterTime(pFuncVI, int);

    // Открывает страницу или закрывает меню в зависимости от того, какая страница сейчас раскрыта
    static void ShowHidePage(const Page *page);

    static void ChangeRShift(Chan::E ch, int16 delta);

    // Обработчики нажатия кнопок
    static void OnTime();      // Key::Time
    static void OnFunction();  // Key::Function
    static void OnService();   // Key::Service
    static void OnMeasures();  // Key::Measure
    static void OnDisplay();   // Key::Display
    static void OnMemory();    // Key::Memory
    static void OnChannelA();  // Key::ChannelA
    static void OnChannelB();  // Key::ChannelB
    static void OnTrig();      // Key::Trig

    static void OnArrow();     // Key::Left, Key::Up, Key::Right, Key::Down
    static void OnEnter();     // Key::Enter
    static void OnStart();     // Key::Start
    static void OnTrigLev();   // Key::TrigLevLess, Key::TrigLevMore
    static void OnRangeA();    // Key::RangeLessA, Key::RangeMoreA
    static void OnRangeB();    // Key::RangeLessB, Key::RangeMoreB
    static void OnRShiftA();   // Key::RShiftLessA, Key::RShiftMoreB
    static void OnRShiftB();   // Key::RShiftLessB, Key::RShiftMoreB
    static void OnTBase();     // Key::TBaseLess, Key::TBase::More
    static void OnTShift();    // Key::TShiftLess, Key::TShiftMore
    static void OnFX();        // Key::F1, Key::F2, Key::F3, Key::F4, Key::F5

    // Общая функция при нажатии на кнопку со страницей
    static void CommonButtonPage(const Page *page);
}


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
        RShift::Change(ch, delta);

        DisplayOsci::SetFlagRedraw();
    }
}


void Handlers::OnRShiftA()
{
    OnChangeParameterChannel(ChangeRShift, ChanA, (event.key == Key::RShiftMoreA) ? 1 : -1);
}


void Handlers::OnRShiftB()
{
    OnChangeParameterChannel(ChangeRShift, ChanB, (event.key == Key::RShiftMoreB) ? 1 : -1);
}


void Handlers::OnRangeA()
{
    OnChangeParameterChannel(Range::Change, ChanA, (event.key == Key::RangeMoreA) ? 1 : -1);
}


void Handlers::OnRangeB()
{
    OnChangeParameterChannel(Range::Change, ChanB, (event.key == Key::RangeMoreB) ? 1 : -1);
}


void Handlers::OnChangeParameterChannel(pFuncVChI16 func, Chan::E ch, int16 delta)
{
    func(ch, delta);
}


void Handlers::OnChangeParameterTime(pFuncVI func, int delta)
{
    func(delta);
}


void Handlers::OnTShift()
{
    int delta = (event.key == Key::TShiftMore) ? 1 : -1;

    if (Osci::IsRunning())
    {
        OnChangeParameterTime(TShift::Change, delta);
    }
    else
    {
        OnChangeParameterTime(DisplayOsci::ShiftInMemory::Change, delta);
    }
}


void Handlers::OnTBase()
{
    int delta = (event.key == Key::TBaseMore) ? 1 : -1;

    if (Device::InModeRecorder())
    {
        OnChangeParameterTime(Recorder::ScaleX::Change, delta);
    }
    else
    {
        OnChangeParameterTime(TBase::Change, delta);
    }
}


void Handlers::OnFX()
{
    if (Menu::IsShown())
    {
        const Item *underKey = Menu::ItemUnderFunctionalKey(event.key);     // Находим элемент меню, соответствующий нажатой кнопке

        if((underKey == &Item::empty && !Menu::OpenedItem()->IsPage()) && (event.IsRelease() || event.IsLong()) )
        {
            Menu::CloseOpenedItem();
        }
        else
        {
            underKey->HandlerFX(event.type);
        }
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
    else if (openedItem == PageFFT::Cursors::self) //-V2516
    {
        openedItem->HandlerKey(event);
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
    else if (event.IsLong()) //-V2516
    {
        return Menu::IsShown() ? Menu::Hide() : Menu::Show();
    }
}


void Handlers::OnTrigLev()
{
    TrigLevel::Change((event.key == Key::TrigLevMore) ? 1 : -1);
}


void Handlers::OnChannelA()
{
    if (event.IsRelease())
    {
        CommonButtonPage(PageChannelA::self);

        ShowHidePage(PageChannelA::self);
    }
    else if (event.IsLong()) //-V2516
    {
        RShift::Set(ChanA, 0);
    }
}


void Handlers::OnChannelB()
{
    if (event.IsRelease())
    {
        CommonButtonPage(PageChannelB::self);

        ShowHidePage(PageChannelB::self);
    }
    else if (event.IsLong()) //-V2516
    {
        RShift::Set(ChanB, 0);
    }
}


void Handlers::OnFunction()
{
    CommonButtonPage(PageFunction::self);

    ShowHidePage(PageFunction::self);
}


void Handlers::OnMeasures()
{
    CommonButtonPage(PageMeasures::self);

    ShowHidePage(PageMeasures::self);
}


void Handlers::OnMemory()
{
    if (FDrive::IsConnected() && S_MEM_MODE_BTN_MEMORY_IS_SAVE)
    {
        FDrive::SaveScreen();
    }
    else if(Device::InModeOsci()) //-V2516
    {
        CommonButtonPage(PageMemory::self);

        ShowHidePage(PageMemory::self);
    }
}


void Handlers::OnService()
{
    CommonButtonPage(PageService::self);

    ShowHidePage(PageService::self);
}


void Handlers::OnTime()
{
    CommonButtonPage(PageTime::self);

    if (event.IsRelease())
    {
        ShowHidePage(PageTime::self);
    }
    else if (event.IsLong()) //-V2516
    {
        if (Osci::IsRunning())
        {
            TShift::Reset();
        }
        else
        {
            DisplayOsci::ShiftInMemory::OnChangeTPos();
        }
    }
}


void Handlers::OnStart()
{
    if (Device::InModeTester())
    {
        Tester::StartStop();
    }
    else if(Device::InModeRecorder())
    {
        Recorder::OnPressStart();
    }
    else
    {
        Osci::OnPressButtonStart();
    }
}


void Handlers::OnTrig()
{
    if (event.IsRelease())
    {
        CommonButtonPage(PageTrig::self);

        ShowHidePage(PageTrig::self);
    }
    else if (event.IsLong()) //-V2516
    {
        TrigLevel::Set(S_TRIG_SOURCE, 0);
    }
}


void Handlers::OnDisplay()
{
    CommonButtonPage(PageDisplay::self);

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
        Menu::SetMainPage(page);

        if (!Menu::IsShown())
        {
            Menu::Show();
        }
    }
}


void Handlers::CommonButtonPage(const Page *page)
{
    if(Menu::GetMainPage() != page)
    {
        if(!Menu::OpenedItem()->IsPage())
        {
            Menu::CloseOpenedItem();
        }
    }
}
