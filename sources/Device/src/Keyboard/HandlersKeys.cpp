#include "defines.h"
#include "HandlersKeys.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/ContextOsci.h"
#include "Hardware/Timer.h"
#include "Keyboard/Keyboard.h"
#include "Menu/Menu.h"
#include "Menu/MenuItems.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Settings/SettingsOsci.h"
#include "device.h"


typedef void (*pFuncVCh)(Chan::E);
typedef void (*pFuncVChI16)(Chan::E, int16);
typedef void (*pFuncVI)(int);


/// Обрабатываемое событие
static KeyEvent event;
/// Канал, параметры которого нужно временно выводить
static Chan drawingChan(Chan::A);


/// Обработчик нажатия функциональной кнопки
static void FX_Press();
/// Обработчик отпускания функциональной кнопки
static void FX_Release();
/// Обработчик длительного нажатия функциональной кнопки
static void FX_Long();
/// Общий обработчик изменения параметра канала - масштаба или смещения
static void OnChangeParameterChannel(pFuncVChI16, Chan::E, int16);
/// Общий обработчик изменения временных параметров
static void OnChangeParameterTime(pFuncVI, int);
/// Открывает страницу или закрывает меню в зависимости от того, какая страница сейчас раскрыта
static void ShowHidePage(const Page *page);


/// Пустой обработчик
static void Empty();
/// Обработчик нажатия стрелки

static void HandlerArrow();

static void EnterRelease();
static void EnterLong();

static void Time_Long();
static void Time_Release();

static void Start();

static void Function_Release();

static void Service_Release();

static void Measures_Release();

static void Display_Release();

static void Memory_Release();

static void ChannelA_Release();
static void ChannelB_Release();
static void ChannelA_Long();
static void ChannelB_Long();


static void Trig_Release();
static void Trig_Long();
static void TrigLevLess_Press();
static void TrigLevMore_Press();

static void RangeLessA();
static void RangeMoreA();
static void RangeLessB();
static void RangeMoreB();

static void RShiftLessA();
static void RShiftMoreA();
static void RShiftLessB();
static void RShiftMoreB();

static void TBaseLess();
static void TBaseMore();
static void TShiftLess();
static void TShiftMore();
/// Общий обработчик раскрытой страницы. Возвращает true, если отработал и дальнейшая обработка события клавиатуры не требуется.
static bool CommonHandlerPage();


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
        {Empty,             Empty,             Time_Release,     Time_Long},        // Time       
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
        {TBaseMore,         Empty,             Empty,            Empty},            // TBaseMore  
        {TBaseLess,         Empty,             Empty,            Empty},            // TBaseLess  
        {TShiftMore,        TShiftMore,        Empty,            Empty},            // TShiftMore 
        {TShiftLess,        TShiftLess,        Empty,            Empty},            // TShiftLess 
        {TrigLevMore_Press, TrigLevMore_Press, Empty,            Empty},            // TrigLevMore
        {TrigLevLess_Press, TrigLevLess_Press, Empty,            Empty},            // TrigLevLess
        {HandlerArrow,      HandlerArrow,      HandlerArrow,     HandlerArrow},     // Left       
        {HandlerArrow,      HandlerArrow,      HandlerArrow,     HandlerArrow},     // Right      
        {HandlerArrow,      HandlerArrow,      HandlerArrow,     HandlerArrow},     // Up         
        {HandlerArrow,      HandlerArrow,      HandlerArrow,     HandlerArrow},     // Down       
        {Empty,             Empty,             EnterRelease,     EnterLong},        // Enter      
        {FX_Press,          Empty,             FX_Release,       FX_Long},          // F1
        {FX_Press,          Empty,             FX_Release,       FX_Long},          // F2
        {FX_Press,          Empty,             FX_Release,       FX_Long},          // F3
        {FX_Press,          Empty,             FX_Release,       FX_Long},          // F4
        {FX_Press,          Empty,             FX_Release,       FX_Long}           // F5
    };

    Key::E code = event.key;
    TypePress::E type = event.type;

    if (code < Key::Count && type < TypePress::None)
    {
        if (!CommonHandlerPage())
        {
            func[static_cast<int>(code)][static_cast<int>(type)]();
        }
    }
}

static void Empty()
{
}

static void ChangeRShift(Chan::E ch, int16 delta)
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
            RShift::Change(ch, delta);

            if (RShift(ch) == 0)
            {
                stop[ch] = true;
                timeStop[ch] = TIME_MS;
            }

            DisplayOsci::SetFlagRedraw();
        }
    }
}

static void RShiftLessA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, -1);
}

static void RShiftMoreA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, 1);
}

static void RShiftLessB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, -1);
}

static void RShiftMoreB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, 1);
}

static void RangeLessA()
{
    OnChangeParameterChannel(Range::Change, Chan::A, -1);
}

static void RangeMoreA()
{
    OnChangeParameterChannel(Range::Change, Chan::A, +1);
}

static void RangeLessB()
{
    OnChangeParameterChannel(Range::Change, Chan::B, -1);
}

static void RangeMoreB()
{
    OnChangeParameterChannel(Range::Change, Chan::B, +1);
}

static void OnChangeParameterChannel(pFuncVChI16 func, Chan::E ch, int16 delta)
{
    if (Device::State::InModeOsci())
    {
        drawingChan = Chan(ch);
    }

    func(ch, delta);
}

static void OnChangeParameterTime(pFuncVI func, int delta)
{
    func(delta);
}

static void TShiftLess()
{
    OnChangeParameterTime(TShift::Change, -1);
}

static void TShiftMore()
{
    OnChangeParameterTime(TShift::Change, 1);
}

static void TBaseLess()
{
    if (Device::State::InModeRecorder())
    {
        OnChangeParameterTime(RecorderScaleX::Change, -1);
    }
    else
    {
        OnChangeParameterTime(TBase::Change, -1);
    }
}

static void TBaseMore()
{
    if (Device::State::InModeRecorder())
    {
        OnChangeParameterTime(RecorderScaleX::Change, 1);
    }
    else
    {
        OnChangeParameterTime(TBase::Change, 1);
    }
}

static void FX_Press()
{
    if (Menu::IsShown())
    {
        Menu::ItemUnderFunctionalKey(event.key)->HandlerFX(TypePress::Press);
    }
}

static void FX_Release()
{
    if (Menu::IsShown())
    {
        Menu::ItemUnderFunctionalKey(event.key)->HandlerFX(TypePress::Release);
    }
}

static void FX_Long()
{
    if (Menu::IsShown())
    {
        Menu::ItemUnderFunctionalKey(event.key)->HandlerFX(TypePress::Long);
    }
}

static void HandlerArrow()
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

static bool CommonHandlerPage()
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

static void EnterRelease()
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

static void EnterLong()
{
    return Menu::IsShown() ? Menu::Hide() : Menu::Show();
}

static void TrigLevMore_Press()
{
    TrigLevel::Change(1);
}


static void TrigLevLess_Press()
{
    TrigLevel::Change(-1);
}


static void ChannelA_Release()
{
    ShowHidePage(PageChannelA::self);
}


static void ChannelB_Release()
{
    ShowHidePage(PageChannelB::self);
}


static void ChannelA_Long()
{
    RShift(Chan::A, 0);
}


static void ChannelB_Long()
{
    RShift(Chan::B, 0);
}


static void Function_Release()
{
    ShowHidePage(PageFunction::self);
}


static void Measures_Release()
{
    ShowHidePage(PageMeasures::self);
}


static void Memory_Release()
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


static void Service_Release()
{
    ShowHidePage(PageService::self);
}


static void Time_Release()
{
    ShowHidePage(PageTime::self);
}


static void Time_Long()
{
    TShift::Reset();
}


static void Start()
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


static void Trig_Release()
{
    ShowHidePage(PageTrig::self);
}


static void Trig_Long()
{
    TrigLevel(set.trig.source, 0);
}


static void Display_Release()
{
    ShowHidePage(PageDisplay::self);
}


static void ShowHidePage(const Page *page)
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
