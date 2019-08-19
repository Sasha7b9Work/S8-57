#include "defines.h"
#include "log.h"
#include "device.h"
#include "Display/Console.h"
#include "Display/Painter.h"
#include "Display/Display_Primitives.h"
#include "Hardware/Timer.h"
#include "Hardware/Beeper.h"
#include "Recorder/Recorder_Display.h"
#include "Osci/Display/Osci_Display.h"
#include "Osci/Display/PainterData.h"
#include "Multimeter/Multimeter.h"
#include "Utils/Debug.h"
#include "FlashDrive/FlashDrive.h"
#include "Keyboard/DecoderDevice.h"
#include "Transceiver.h"

#include <cstring>


using namespace Display::Primitives;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NEED_SET_ORIENTATION    (bf.needSetOrientation)

#define NU {0, 0}

static struct StructWarning
{
    Warning             warning;
    bool                good;
    uint8               notUsed[2];
    const char * const  message[3];
}
warns[Warning::Number] =
{
    {Warning::LimitChan1_Volts, false, NU,           {"ПРЕДЕЛ КАНАЛ 1 - ВОЛЬТ/ДЕЛ"}},
    {Warning::LimitChan2_Volts, false, NU,           {"ПРЕДЕЛ КАНАЛ 2 - ВОЛЬТ/ДЕЛ"}},
    {Warning::LimitSweep_Time, false, NU,            {"ПРЕДЕЛ РАЗВЕРТКА - ВРЕМЯ/ДЕЛ"}},
    {Warning::EnabledPeakDet, false, NU,             {"ВКЛЮЧЕН ПИКОВЫЙ ДЕТЕКТОР"}},
    {Warning::LimitChan1_RShift, false, NU,          {"ПРЕДЕЛ КАНАЛ 1 - \x0d"}},
    {Warning::LimitChan2_RShift, false, NU,          {"ПРЕДЕЛ КАНАЛ 2 - \x0d"}},
    {Warning::LimitSweep_Level, false, NU,           {"ПРЕДЕЛ РАЗВЕРТКА - УРОВЕНЬ"}},
    {Warning::LimitSweep_TShift, false, NU,          {"ПРЕДЕЛ РАЗВЕРТКА - \x97"}},
    {Warning::TooSmallSweepForPeakDet, false, NU,    {"ПИК. ДЕТ. НЕ РАБОТАЕТ НА РАЗВЕРТКАХ МЕНЕЕ 0.5мкс/дел"}},
    {Warning::TooFastScanForRecorder, false, NU,     {"САМОПИСЕЦ НЕ МОЖЕТ РАБОТАТЬ НА БЫСТРЫХ РАЗВЁРТКАХ"}},
    {Warning::FileIsSaved, true, NU,                 {"ФАЙЛ СОХРАНЕН"}},
    {Warning::SignalIsSaved, true, NU,               {"СИГНАЛ СОХРАНЕН"}},
    {Warning::SignalIsDeleted, true, NU,             {"СИГНАЛ УДАЛЕН"}},
    {Warning::MenuDebugEnabled, true, NU,            {"МЕНЮ ОТЛАДКА ВКЛЮЧЕНО"}},
    {Warning::TimeNotSet, true, NU,                  {"ВРЕМЯ НЕ УСТАНОВЛЕНО. МОЖЕТЕ УСТАНОВИТЬ ЕГО СЕЙЧАС"}},
    {Warning::SignalNotFound, true, NU,              {"СИГНАЛ НЕ НАЙДЕН"}},
    {Warning::SetTPosToLeft, true, NU,               {  "НА РАЗВЕРТКАХ МЕДЛЕННЕЕ 10мс/дел ЖЕЛАТЕЛЬНО УСТАНАВ-",
                                                        "ЛИВАТЬ \"РАЗВЕРТКА - \x7b\" В ПОЛОЖЕНИЕ \"Лево\" ДЛЯ УСКОРЕ-", 
                                                        "НИЯ ВЫВОДА СИГНАЛА"}},
    {Warning::NeedRebootDevice, true, NU,               {"Для вступления изменений в силу", 
                                                         "выключите прибор"}},
    {Warning::ImpossibleEnableMathFunction, true, NU,   {"Отключите вычисление БПФ"}},
    {Warning::ImpossibleEnableFFT, true, NU,            {"Отключите математическую функцию"}},
    {Warning::WrongFileSystem, false, NU,               {"Не могу прочитать флешку. Убедитесь, что на ней FAT32"}},
    {Warning::WrongModePeackDet, true, NU,              {"Чтобы изменить длину памяти, отключите пиковый детектор"}},
    {Warning::DisableChannelB, true, NU,                {"Сначала выключите канал 2"}},
    {Warning::TooLongMemory, true, NU,                  {"Второй канал рабоатает при длине памяти 16к и менее"}},
    {Warning::NoPeakDet32k, true, NU,                   {"Пиковый детектор не работает при длине памяти 32к"}},
    {Warning::NoPeakDet16k, true, NU,                   {"Для работы пикового детектора при длине памяти 16к", 
                                                         "отключите канал 2"}},
    {Warning::Warn50Ohms, false, NU,                    {"При величине входного сопротивления 50 Ом", 
                                                         "запрещается подавать на вход более 5 В"}},
    {Warning::WarnNeedForFlashDrive, true, NU,          {"Сначала подключите флеш-диск"}},
    {Warning::FirmwareSaved, true, NU,                  {"Прошивка сохранена"}},
    {Warning::FullyCompletedOTP, false, NU,             {"Память OTP полностью заполнена"}}
};

#define NUM_WARNINGS    10
static const char       *warnings[NUM_WARNINGS] = {0};      ///< Здесь предупреждающие сообщения.
static uint             timeWarnings[NUM_WARNINGS] = {0};   ///< Здесь время, когда предупреждающее сообщение поступило на экран.


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static pFuncVV funcOnHand = 0;
static uint timeStart = 0;
static const char *textWait = 0;
static bool clearBackground = false;
/// true, если нужно сохранять копию экрана на флешку
static bool needSaveScreen = false;
/// Дополнительная функция рисования. Выполняется после стандартной отрисовки, но перед вызовом EndScene;
volatile static pFuncVV funcAdditionDraw = EmptyFuncVV;
/// true означает, что происходит процесс отрисовки
static bool inStateDraw = false;

static pFuncVV funcAfterUpdateOnce = EmptyFuncVV;

static int numRow = -1;

static void SaveScreenToFlash();

static void ReadRow(uint8 row);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Выполняет функцию, определённую для выполнения после отрисовки
static void ExecuteFuncAfterUpdateOnce();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Display::Init()
{
    for(uint8 i = 0; i < Color::NUMBER.value; i++)
    {
        Color(i).SetValue(COLOR(i));
    }

    Painter::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::Update()
{
    inStateDraw = true;

    static const pFuncVV funcs[Device::Mode::Size] =
    {
        Osci::Display::Update,
        Tester::Display::Update,
        Multimeter::Display::Update,
        Recorder::Display::Update
    };

    funcs[Device::State::CurrentMode()]();

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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::InProcess()
{
    return inStateDraw;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ExecuteFuncAfterUpdateOnce()
{
    funcAfterUpdateOnce();
    funcAfterUpdateOnce = EmptyFuncVV;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetFuncAfterUpadteOnce(pFuncVV func)
{
    funcAfterUpdateOnce = func;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerShowWarning()
{
    uint time = TIME_MS;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (time - timeWarnings[i] > (uint)TIME_MESSAGES * 1000)
        {
            timeWarnings[i] = 0;
            warnings[i] = 0;
        }
    }

    int pointer = 0;
    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] != 0)
        {
            warnings[pointer] = warnings[i];
            timeWarnings[pointer] = timeWarnings[i];
            if (pointer != i)
            {
                timeWarnings[i] = 0;
                warnings[i] = 0;
            }
            pointer++;
        }
    }

    if (pointer == 0)
    {
        Timer::Disable(Timer::Type::ShowMessages);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ShowWarn(const char *message)
{
    if (warnings[0] == 0)
    {
        Timer::SetAndEnable(Timer::Type::ShowMessages, OnTimerShowWarning, 100);
    }
    bool alreadyStored = false;

    for (int i = 0; i < NUM_WARNINGS; i++)
    {
        if (warnings[i] == 0 && !alreadyStored)
        {
            warnings[i] = message;
            timeWarnings[i] = TIME_MS;
            alreadyStored = true;
        }
        else if (warnings[i] == message)
        {
            timeWarnings[i] = TIME_MS;
            return;
        }
        else
        {
            // ничего делать не надо
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::ShowWarning(Warning::E warning)
{
    Color::ResetFlash();
    for (int i = 2; i >= 0; i--)
    {
        const char *message = warns[warning].message[i];
        if (message)
        {
            ShowWarn(message);
        }
    }
    if (warns[warning].good)
    {
        Beeper::WarnBeepGood();
    }
    else
    {
        Beeper::WarnBeepBad();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetDrawMode(DrawMode::E mode, pFuncVV func)
{
    funcOnHand = func;
    if (mode == DrawMode::Hand)
    {
        Timer::SetAndEnable(Timer::Type::Display, funcOnHand, 40);
    }
    else
    {
        Timer::Disable(Timer::Type::Display);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::FuncOnWaitStop()
{
    Display::SetDrawMode(DrawMode::Auto, 0);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void FuncOnWait()
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

    Text(textWait).DrawInCenterRect(x, y, width, height - 20);

    char buf[100];
    buf[0] = 0;

    for (uint i = 0; i < time; i++)
    {
        std::strcat(buf, "."); //-V2513
    }

    Text(buf).DrawInCenterRect(x, y + 20, width, height - 20);

    Painter::EndScene();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::FuncOnWaitStart(const char *text, bool eraseBackground)
{
    timeStart = TIME_MS;
    textWait = text;
    clearBackground = eraseBackground;
    Display::SetDrawMode(DrawMode::Hand, FuncOnWait);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::RemoveAddDrawFunction()
{
    funcAdditionDraw = EmptyFuncVV;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetOrientation(Orientation::E orientation)
{
    DISPLAY_ORIENTATION = orientation;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SetAddDrawFunction(pFuncVV func, uint time)
{
    if (func == 0)
    {
        func = EmptyFuncVV;
    }

    funcAdditionDraw = func;

    if (time)
    {
        Timer::SetAndStartOnce(Timer::Type::RemoveAddFunction, RemoveAddDrawFunction, time);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Display::TimeMenuAutoHide()
{
    static const int times[] =
    {
        0, 5, 10, 15, 30, 60
    };
    return times[MENU_AUTO_HIDE] * 1000;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Display::IsSeparate()
{
    return (FUNC_MODE_DRAW_IS_SEPARATE || FFT_ENABLED);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Display::ENumSignalsInSec::TimeBetweenFramesMS() const
{
    static const uint time[] = { 40, 100, 200, 500, 1000 };

    return time[value];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Display::ENumSmoothing::ToNumber() const
{
    return (uint)value + 1U;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SaveScreenToDrive()
{
    needSaveScreen = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void CreateFileName(char name[256])
{
    std::strcpy(name, "screen.bmp");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    FDrive::WriteToFile((uint8 *)(&bmFH), 14, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmIH), 40, &structForWrite);

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
        ((RGBQUAD*)(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    uint8 pixels[320];

    Decoder::SetBufferForScreenRow(pixels);

    for (int row = 239; row >= 0; row--)
    {
        ReadRow((uint8)row);

        FDrive::WriteToFile(pixels, 320, &structForWrite);
    }

    FDrive::CloseFile(&structForWrite);

    Display::FuncOnWaitStart("Файл сохранён", false);

    Timer::PauseOnTime(1500);

    Display::FuncOnWaitStop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadRow(uint8 row)
{
    numRow = -1;

    Transceiver::Transmitter::Send(Command::Screen, row);

    while (numRow == -1)
    {
        uint8 data = 0;
        Transceiver::Transmitter::Send(data);
        Decoder::Update();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Display::SaveRow(int row)
{
    numRow = row;
}
