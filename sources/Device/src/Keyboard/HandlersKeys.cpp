#include "defines.h"
#include "device.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Keyboard/HandlersKeys.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Settings/Settings.h"
#include <cstdio>

#include "Osci/Display/Osci_Display.h"
#include "FlashDrive/FlashDrive.h"


using namespace Display::Primitives;
using namespace FPGA::Settings;
using namespace Osci::Display;
using namespace Osci::Settings;
using namespace Recorder::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
typedef void(*pFuncVCh)(Chan::E);
typedef void(*pFuncVChI)(Chan::E, int);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Обрабатываемое событие
static KeyEvent event;
/// Канал, параметры которого нужно временно выводить
static Chan drawingChan = Chan(Chan::A);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Обработчик отпускания функциональной кнопки
static void FX_Release();
/// Обработчик длительного нажатия функциональной кнопки
static void FX_Long();
/// Общий обработчик изменения параметра канала - масштаба или смещения
static void OnChangeParameterChannel(pFuncVChI, Chan::E, int);
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



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Handlers::Process(KeyEvent e)
{
    event = e;

    static const pFuncVV func[Key::Number][4] =
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
        {Empty,             Empty,             FX_Release,       FX_Long},          // F1
        {Empty,             Empty,             FX_Release,       FX_Long},          // F2
        {Empty,             Empty,             FX_Release,       FX_Long},          // F3
        {Empty,             Empty,             FX_Release,       FX_Long},          // F4
        {Empty,             Empty,             FX_Release,       FX_Long}           // F5
    };

    Key::E code = event.key;
    TypePress::E type = event.type;

    if (code < Key::Number && type < TypePress::None)
    {
        func[code][type]();
    }
    else
    {
        /// \todo Временная затычка. Доделать, чтобы такого не было
        //LOG_WRITE("Ошибка приёма команды кнопки");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Empty()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChangeRShift(Chan::E ch, int delta)
{
    if (Device::State::InModeRecorder())
    {
        return;
    }

    static bool stop[Chan::Size] = { false, false };      // Признак того, что смещение изменять не нужно - оно равно нулю и прошло мало времени
    static uint timeStop[Chan::Size] = { 0, 0 };          // Время устновки признака stop

    if (stop[ch])
    {
        if (TIME_MS - timeStop[ch] > 500)
        {
            stop[ch] = false;
        }
        return;
    }
    else
    {
        RShift::Change(ch, delta);
    }

    if (SET_RSHIFT(ch) == RShift::ZERO)
    {
        stop[ch] = true;
        timeStop[ch] = TIME_MS;
    }

    Osci::Display::SetFlagRedraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RShiftLessA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RShiftMoreA()
{
    OnChangeParameterChannel(ChangeRShift, Chan::A, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RShiftLessB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RShiftMoreB()
{
    OnChangeParameterChannel(ChangeRShift, Chan::B, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RangeLessA()
{
    OnChangeParameterChannel(Osci::Settings::Range::Change, Chan::A, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RangeMoreA()
{
    OnChangeParameterChannel(Osci::Settings::Range::Change, Chan::A, +1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RangeLessB()
{
    OnChangeParameterChannel(Osci::Settings::Range::Change, Chan::B, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void RangeMoreB()
{
    OnChangeParameterChannel(Osci::Settings::Range::Change, Chan::B, +1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChangeParameterChannel(pFuncVChI func, Chan::E ch, int delta)
{
    if (Device::State::InModeOsci())
    {
        drawingChan = Chan(ch);
    }

    func(ch, delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChangeParameterTime(pFuncVI func, int delta)
{
    func(delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TShiftLess()
{
    OnChangeParameterTime(TShift::Change, -1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TShiftMore()
{
    OnChangeParameterTime(TShift::Change, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TBaseLess()
{
    if (Device::State::InModeRecorder())
    {
        OnChangeParameterTime(ScaleX::Change, -1);
    }
    else
    {
        OnChangeParameterTime(TBase::Change, -1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TBaseMore()
{
    if (Device::State::InModeRecorder())
    {
        OnChangeParameterTime(ScaleX::Change, 1);
    }
    else
    {
        OnChangeParameterTime(TBase::Change, 1);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FX_Release()
{
    if (Menu::IsShown())
    {
        const Control *control = Menu::ItemUnderButton(event.key);
        if (control)
        {
            control->ShortPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FX_Long()
{
    if (Menu::IsShown())
    {
        const Control *control = Menu::ItemUnderButton(event.key);
        if (control)
        {
            control->LongPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void HandlerArrow()
{ 
    if(!Menu::IsShown())
    {
        return;
    }
    
    if(Menu::OpenedItem()->IsPage())
    {
        if(!Menu::CurrentItem()->ProcessKey(event))
        {
            Menu::OpenedItem()->ProcessKey(event);
        }
    }
    else
    {
        Menu::OpenedItem()->ProcessKey(event);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void EnterRelease()
{
    if (!Menu::IsShown())
    {
        Menu::Show(true);
    }
    else
    {
        Menu::CloseOpenedItem();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void EnterLong()
{
    Menu::Show(!Menu::IsShown());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TrigLevMore_Press()
{
    Trig::Level::Change(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TrigLevLess_Press()
{
    Trig::Level::Change(-1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChannelA_Release()
{
    ShowHidePage(PageChannelA::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChannelB_Release()
{
    ShowHidePage(PageChannelB::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChannelA_Long()
{
    RShift::Set(Chan::A, RShift::ZERO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChannelB_Long()
{
    RShift::Set(Chan::B, RShift::ZERO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Function_Release()
{
    ShowHidePage(PageFunction::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Measures_Release()
{
    ShowHidePage(PageMeasures::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Memory_Release()
{
    if (FDrive::IsConnected())
    {
        Menu::SaveScreenToDrive();
    }
    else
    {
        ShowHidePage(PageMemory::self);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Service_Release()
{
    ShowHidePage(PageService::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Time_Release()
{
    ShowHidePage(PageTime::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Time_Long()
{
    TShift::Set(TShift::Zero());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Start()
{
    if (Device::State::InModeTester())
    {
        Tester::StartStop();
    }
    else
    {
        FPGA::OnPressStart();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Trig_Release()
{
    ShowHidePage(PageTrig::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Trig_Long()
{
    Trig::Level::Set(Trig::Level::ZERO);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Display_Release()
{
    ShowHidePage(PageDisplay::self);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ShowHidePage(const Page *page)
{
    if (Menu::OpenedItem() == page)
    {
        Menu::Show(!Menu::IsShown());
    }
    else
    {
        Menu::mainPage = page;

        if (!Menu::IsShown())
        {
            Menu::Show(true);
        }
    }
}
