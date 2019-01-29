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


using namespace Display::Primitives;
using namespace FPGA::Settings;
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
static void RangeMoreB();
/// Обработчик отпускания функциональной кнопки
static void FuncRelease();
/// Обработчик длительного нажатия функциональной кнопки
static void FuncLong();
/// Общий обработчик изменения параметра канала - масштаба или смещения
static void OnChangeParameterChannel(pFuncVChI, Chan::E, int);
/// Функция отрисовки параметров канала
static void DrawParametersChannel();
/// Общий обработчик изменения временных параметров
static void OnChangeParameterTime(pFuncVI, int);
/// Функция отрисовки временных параметров
static void DrawParametersTime();

static void OpenPage(const PageBase *page);


/// Пустой обработчик
static void Empty();
/// Обработчик нажатия стрелки
static void HandlerArrow();
static void RShiftLessA();
static void RShiftMoreA();
static void RShiftLessB();
static void RShiftMoreB();
static void RangeLessA();
static void RangeMoreA();
static void RangeLessB();
static void TShiftLess();
static void TShiftMore();
static void TBaseLess();
static void TBaseMore();
static void TrigLevLess();
static void TrigLevMore();
static void EnterRelease();
static void EnterLong();
static void ChannelA();
static void ChannelB();
static void Function();
static void Measure();
static void Memory();
static void Service();
static void Time();
static void Start();
static void HandlerTrig();
static void HandlerDisplay();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Handlers::Process(KeyEvent e)
{
    event = e;

    static const pFuncVV func[Key::Number][4] =
    { // Press           Repead          Release         Long
        {Empty,          Empty,          Empty,          Empty},            // None       
        {Empty,          Empty,          Function,       Empty},            // Function   
        {Measure,        Measure,        Measure,        Measure},          // Measure    
        {Memory,         Memory,         Memory,         Memory},           // Memory     
        {Service,        Service,        Service,        Service},          // Service    
        {ChannelA,       Empty,          Empty,          Empty},            // ChannelA   
        {ChannelB,       Empty,          Empty,          Empty},            // ChannelB   
        {Time,           Time,           Time,           Time},             // Time       
        {Start,          Empty,          Empty,          Empty},            // Start      
        {HandlerTrig,    HandlerTrig,    HandlerTrig,    HandlerTrig},      // Trig       
        {HandlerDisplay, HandlerDisplay, HandlerDisplay, HandlerDisplay},   // Display    
        {RangeMoreA,     Empty,          Empty,          Empty},            // RangeMoreA 
        {RangeLessA,     Empty,          Empty,          Empty},            // RangeLessA 
        {RShiftMoreA,    RShiftMoreA,    Empty,          Empty},            // RShiftMoreA
        {RShiftLessA,    RShiftLessA,    Empty,          Empty},            // RShiftLessA
        {RangeMoreB,     Empty,          Empty,          Empty},            // RangeMoreB 
        {RangeLessB,     Empty,          Empty,          Empty},            // RangeLessB 
        {RShiftMoreB,    RShiftMoreB,    Empty,          Empty},            // RShiftMoreB
        {RShiftLessB,    RShiftLessB,    Empty,          Empty},            // RShiftLessB
        {TBaseMore,      Empty,          Empty,          Empty},            // TBaseMore  
        {TBaseLess,      Empty,          Empty,          Empty},            // TBaseLess  
        {TShiftMore,     TShiftMore,     Empty,          Empty},            // TShiftMore 
        {TShiftLess,     TShiftLess,     Empty,          Empty},            // TShiftLess 
        {TrigLevMore,    TrigLevMore,    Empty,          Empty},            // TrigLevMore
        {TrigLevLess,    TrigLevLess,    Empty,          Empty},            // TrigLevLess
        {HandlerArrow,   HandlerArrow,   HandlerArrow,   HandlerArrow},            // Left       
        {HandlerArrow,   HandlerArrow,   HandlerArrow,   HandlerArrow},            // Right      
        {HandlerArrow,   HandlerArrow,   HandlerArrow,   HandlerArrow},            // Up         
        {HandlerArrow,   HandlerArrow,   HandlerArrow,   HandlerArrow},            // Down       
        {Empty,          Empty,          EnterRelease,   EnterLong},        // Enter      
        {Empty,          Empty,          FuncRelease,    FuncLong},         // F1         
        {Empty,          Empty,          FuncRelease,    FuncLong},         // F2         
        {Empty,          Empty,          FuncRelease,    FuncLong},         // F3         
        {Empty,          Empty,          FuncRelease,    FuncLong},         // F4         
        {Empty,          Empty,          FuncRelease,    FuncLong}          // F5         
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
static void DrawParametersTime()
{
    int x = Grid::Left() + 5;
    int y = Grid::Top() + 5;
    int width = 126;
    int height = 15;

    //Painter::DrawBoundedRegion(x, y, width, height, Color::BACK, Color::FILL);
    Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

    String("Разв : %s", TBase(SET_TBASE).Name()).Draw(x + 3, y + 3, Color::FILL);

    TShift shift(SET_TSHIFT);

    shift.ToString(SET_TBASE).Draw(x + 64, y + 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawParametersChannel()
{
    int x = Grid::Left() + 5;
    int y = Grid::Top() + 5;
    int width = 126;
    int height = 15;
    Chan::E ch = drawingChan.value;

    //Painter::DrawBoundedRegion(x, y, width, height, Color::BACK, Color::Channel(ch));
    Region(width, height).DrawBounded(x, y, Color::BACK, Color::Channel(ch));

    char buffer[50];
    std::sprintf(buffer, "%s : %s %s",
        drawingChan.Name(),
        ModeCouple(SET_COUPLE(ch)).UGO(),
        Range(SET_RANGE(ch)).Name()
    );

    //Painter::DrawBigText(x + 3, y + 3, 1, buffer, Color::Channel(ch));
    Text(buffer).Draw(x + 3, y + 3, Color::Channel(ch));

    //Painter::DrawBigText(x + 80, y + 3, 1, RShift::ToString(SET_RSHIFT(ch), SET_RANGE(ch), SET_DIVIDER(ch)).CString());
    Text(RShift::ToString(SET_RSHIFT(ch), SET_RANGE(ch), SET_DIVIDER(ch)).CString()).Draw(x + 80, y + 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChangeParameterChannel(pFuncVChI func, Chan::E ch, int delta)
{
    if (Device::State::InModeOsci())
    {
        drawingChan = Chan(ch);

        Display::SetAddDrawFunction(DrawParametersChannel, 5000);
    }

    func(ch, delta);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChangeParameterTime(pFuncVI func, int delta)
{
    if (Device::State::InModeOsci())
    {
        Display::SetAddDrawFunction(DrawParametersTime, 5000);
    }

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
static void FuncRelease()
{
    Menu::ReleaseFunctionalButton(event.key);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncLong()
{
    Menu::LongFunctionalButton(event.key);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void HandlerArrow()
{ 
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
static void TrigLevMore()
{
    Trig::Level::Change(1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void TrigLevLess()
{
    Trig::Level::Change(-1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChannelA()
{
    OpenPage(PageChannelA::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ChannelB()
{
    OpenPage(PageChannelB::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Function()
{
    OpenPage(PageFunction::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Measure()
{
    OpenPage(PageMeasures::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Memory()
{
    if (MODE_BTN_MEMORY_IS_SAVE)
    {
        if (event.type == TypePress::Press)
        {
            Painter::SaveScreenToDrive();
        }
    }
    else
    {
        OpenPage(PageMemory::pointer);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Service()
{
    OpenPage(PageService::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Time()
{
    OpenPage(PageTime::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Start()
{
    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void HandlerTrig()
{
    OpenPage(PageTrig::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void HandlerDisplay()
{
    OpenPage(PageDisplay::pointer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OpenPage(const PageBase *page)
{
    ((Page *)page)->SetAsCurrent();
    ((Page *)page)->Open(true);
    Menu::Show(true);
}
