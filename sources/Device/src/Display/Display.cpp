#include "defines.h"
#include "device.h"
#include "common/Transceiver.h"
#include "Display/Console.h"
#include "Display/Display.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Display/Warnings.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/DecoderDevice.h"
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
/// true, если нужно сохранять копию экрана на флешку
static bool needSaveScreen = false;
/// Дополнительная функция рисования. Выполняется после стандартной отрисовки, но перед вызовом EndScene;
volatile static pFuncVV funcAdditionDraw = EmptyFunc;
/// true означает, что происходит процесс отрисовки
static bool inStateDraw = false;

static pFuncVV funcAfterUpdateOnce = EmptyFunc;

static int numRow = -1;

static void SaveScreenToFlash();

static void ReadRow(uint8 row);
/// Выполняет функцию, определённую для выполнения после отрисовки
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
    Color::ChangeFlash(false);

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

    if (needSaveScreen)
    {
        SaveScreenToFlash();

        needSaveScreen = 0;
    }
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

    int length = Font::GetLengthText(textWait);

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
        while (Transceiver::Receive()) {};

        Decoder::Update();

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
    return times[MenuAutoHide()] * 1000;
}



bool Display::IsSeparate()
{
    return ((set.math.modeDraw == FuncModeDraw::Separate) || set.fft.enabled);
}


uint ENumSignalsInSec::TimeBetweenFramesMS()
{
    static const uint time[] = { 40, 100, 200, 500, 1000 };

    return time[ENumSignalsInSec()];
}


ENumSignalsInSec::E &ENumSignalsInSec::Ref()
{
    return set.disp.enumSignalsInSec;
}


ENumSmoothing::E &ENumSmoothing::Ref()
{
    return set.disp.enumSmoothing;
}


void Display::SaveScreenToDrive()
{
    needSaveScreen = true;
}


static void CreateFileName(char name[256])
{
    std::strcpy(name, "screen.bmp");
}


static void SaveScreenToFlash()
{
    if (!FDrive::IsConnected())
    {
        return;
    }

#pragma pack(1)
    struct BITMAPFILEHEADER
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    }
    bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240,
        0,
        0,
        14 + 40 + 1024
    };

    // 14

    struct BITMAPINFOHEADER
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    }
    bmIH =
    {
        40, // size;
        320,// width;
        240,// height;
        1,  // planes;
        8,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    // 54
#pragma pack(4)

    StructForWrite structForWrite;

    char fileName[255];

    CreateFileName(fileName);

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile(reinterpret_cast<uint8 *>(&bmFH), 14, &structForWrite);

    FDrive::WriteToFile(reinterpret_cast<uint8 *>(&bmIH), 40, &structForWrite);

    uint8 buffer[320 * 3] = { 0 };

    typedef struct tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    } RGBQUAD;

    RGBQUAD colorStruct;

    for (int i = 0; i < 32; i++)
    {
        uint color = COLOR(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color));
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color));
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color));
        colorStruct.rgbReserved = 0;
        (reinterpret_cast<RGBQUAD*>(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    uint8 pixels[320];

    Decoder::SetBufferForScreenRow(pixels);

    for (int row = 239; row >= 0; row--)
    {
        ReadRow(static_cast<uint8>(row));

        FDrive::WriteToFile(pixels, 320, &structForWrite);
    }

    FDrive::CloseFile(&structForWrite);

    Display::Message::Show("Файл сохранён", false);

    Timer::PauseOnTime(1500);

    Display::Message::Hide();
}


static void ReadRow(uint8 row)
{
    numRow = -1;

    Transceiver::Send(Command::Screen, row);

    while (numRow == -1)
    {
        uint8 data = 0;
        Transceiver::Send(data);
        Decoder::Update();
    }
}


void Display::SaveRow(int row)
{
    numRow = row;
}


void Display::LoadBrightness()
{
    Transceiver::Send(Command::Display_Brightness, static_cast<uint8>(set.disp.brightness + 10));
}
