#include "defines.h"
#include "device.h"
#include "common/Decoder_d.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "Hardware/HAL/HAL.h"
#include "common/Display/Font/Font_d.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"
#include "Osci/Display/DisplayOsci.h"
#include "Recorder/DisplayRecorder.h"
#include "Utils/Values.h"
#include <cstring>


static void EmptyFunc() { }


bool Display::Message::waitKey = false;
volatile bool Display::Message::running = false;


static pFuncVV funcOnHand = nullptr;
static uint timeStart = 0;
static const char *textWait = 0;
static bool clearBackground = false;
// Дополнительная функция рисования. Выполняется после стандартной отрисовки, но перед вызовом EndScene;
volatile static pFuncVV funcAdditionDraw = EmptyFunc;
// true означает, что происходит процесс отрисовки
static bool inStateDraw = false;

static pFuncVV funcAfterUpdateOnce = EmptyFunc;

// Выполняет функцию, определённую для выполнения после отрисовки
static void ExecuteFuncAfterUpdateOnce();


void Display::Init()
{
    for(uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        Color(i).LoadValueRGB();
    }

    Painter::Init();

    LoadBrightness();
}


void Display::Update()
{
    static uint prevTime = 0;

    if(Device::InModeOsci() && (TIME_MS - prevTime < ENumSignalsInSec::TimeBetweenFramesMS()))
    {
        return;
    }

    prevTime = TIME_MS;

    DFont::SetSpacing();

    Color::ChangeFlash(false);

    DFont::Set(DTypeFont::_8);

    inStateDraw = true;

    static const pFuncVV funcs[Device::Mode::Count] =
    {
        DisplayOsci::Update,
        DisplayTester::Update,
        DisplayMultimeter::Update,
        DisplayRecorder::Update
    };

    funcs[static_cast<int>(Device::CurrentMode())]();

    Console::Draw();

    funcAdditionDraw();

    Painter::EndScene();

    inStateDraw = false;

    ExecuteFuncAfterUpdateOnce();
}


bool Display::InProcess()
{
    return inStateDraw;
}


static void ExecuteFuncAfterUpdateOnce()
{
    funcAfterUpdateOnce();
    funcAfterUpdateOnce = EmptyFunc;
}


void Display::SetFuncAfterUpadteOnce(pFuncVV func)
{
    funcAfterUpdateOnce = func;
}


void Display::ShowWarning(const char *warn)
{
    Warnings::AddWarning(warn);
}


void Display::SetDrawMode(DrawMode::E mode, pFuncVV func)
{
    funcOnHand = func;

    if (mode == DrawMode::Hand)
    {
        Timer::SetAndEnable(TypeTimer::Display, funcOnHand, 100);
    }
    else
    {
        Timer::Disable(TypeTimer::Display);
    }
}


void Display::Message::Hide()
{
    Display::SetDrawMode(DrawMode::Auto, nullptr);
    running = false;
    BufferButtons::Clear();
}


void Display::Message::Func()
{
    if (clearBackground)
    {
        Painter::BeginScene(Color::BACK);
    }

    uint time = ((TIME_MS - timeStart) / 50) % 100;

    if (time > 50)
    {
        time = (100 - time);
    }

    int width = 200;
    int height = 80;
    int x = 160 - width / 2;
    int y = 120 - height / 2;

    Region(width, height).Fill(x, y, Color::BACK);
    Rectangle(width, height).Draw(x, y, Color::FILL);

    int length = DFont::GetLengthText(textWait);

    if (waitKey)
    {
        y += 10;
    }

    if (length < width)
    {
        Text(textWait).DrawInCenterRect(x, y, width, height - 20);
    }
    else
    {
        Text(textWait).DrawInRectWithTransfers(x + 11, y + 20, width, height - 20);
    }

    char buf[100] = { 0 };

    if (!waitKey)
    {
        for (uint i = 0; i < time; i++)
        {
            std::strcat(buf, "."); //-V2513
        }
    }

    Text(buf).DrawInCenterRect(x, y + 20, width, height - 20);

    Painter::EndScene();

    if (waitKey)
    {
        while (HAL_BUS::Panel::Receive()) {};

        DDecoder::Update();

        while (!BufferButtons::IsEmpty())
        {
            if (BufferButtons::Extract().IsRelease())
            {
                Hide();
            }
        }
    }
}



void Display::Message::Show(const char *text, bool eraseBackground)
{
    running = true;
    BufferButtons::Clear();
    timeStart = TIME_MS;
    textWait = text;
    clearBackground = eraseBackground;
    waitKey = false;
    Display::SetDrawMode(DrawMode::Hand, Func);
}


void Display::Message::ShowAndWaitKey(const char *text, bool eraseBackground)
{
    Show(text, eraseBackground);
    waitKey = true;
    while (running) {};
}


void Display::RemoveAddDrawFunction()
{
    funcAdditionDraw = EmptyFunc;
}


void Display::SetAddDrawFunction(pFuncVV func, uint time)
{
    if (func == 0)
    {
        func = EmptyFunc;
    }

    funcAdditionDraw = func;

    if (time)
    {
        Timer::SetAndStartOnce(TypeTimer::RemoveAddFunction, RemoveAddDrawFunction, time);
    }
}


int Display::TimeMenuAutoHide()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[set.disp.menuAutoHide] * 1000;
}



bool Display::IsSeparate()
{
    return ((set.math.modeDraw == FuncModeDraw::Separate) || set.fft.enabled);
}


uint ENumSignalsInSec::TimeBetweenFramesMS()
{
    static const uint time[] = { 1, 100, 200, 500, 1000 };

    return time[set.disp.enumSignalsInSec];
}


void Display::LoadBrightness()
{
    HAL_BUS::Panel::Send(Command::Display_Brightness, static_cast<uint8>(set.disp.brightness + 10));
}
