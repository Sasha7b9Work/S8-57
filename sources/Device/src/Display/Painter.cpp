#include "defines.h"
#include "log.h"
#include "Command.h"
#include "Message.h"
#include "Painter.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Tester/Tester.h"
#include "Utils/Buffer.h"
#include <cstdlib>
#include <cstring>

#include "FlashDrive/FlashDrive.h"
#include "Keyboard/DecoderDevice.h"
#include "Display/Display.h"
#include "Utils/Dictionary.h"


using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool needSaveScreen = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Painter::Init()
{
    Timer::SetAndEnable(Timer::Type::FlashDisplay, Color::OnTimerFlashDisplay, 500);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::BeginScene(Color color)
{
    Message message(2, Command::Paint_BeginScene, color.value);

    FSMC::WriteToPanel(message.Data(), message.Size());
}

static int numRow = -1;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SaveRow(int row)
{
    numRow = row;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadRow(uint8 row)
{
    numRow = -1;

    uint8 buffer[] = { Command::Screen, row };

    FSMC::WriteToPanel(buffer, 2);

    while (numRow == -1)
    {
        uint8 data = 0;
        FSMC::WriteToPanel(&data, 1);
        Decoder::Update();
    }
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

    //Display::FuncOnWaitStart(DICT(DBalanceChB), false);
    //
    //Timer::PauseOnTime(5000);
    //
    //Display::FuncOnWaitStop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::EndScene()
{
    if (needSaveScreen)
    {
        SaveScreenToFlash();

        needSaveScreen = 0;
    }

    uint8 buffer[1] = { Command::Paint_EndScene };
    FSMC::WriteToPanel(buffer, 1);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::DrawTesterData(uint8 mode, Color color, const uint8 *x, const uint8 *y)
{
    Buffer buffer(483);
    buffer.data[0] = Command::Paint_TesterLines;
    buffer.data[1] = mode;
    buffer.data[2] = color.value;
    uint8 *pointer = buffer.data + 3;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer++ = x[i];
    }
    for (int i = 0; i < TESTER_NUM_POINTS; i++)
    {
        *pointer++ = y[i];
    }
    FSMC::WriteToPanel(buffer.data, 483);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Painter::SaveScreenToDrive()
{
    needSaveScreen = true;
}
