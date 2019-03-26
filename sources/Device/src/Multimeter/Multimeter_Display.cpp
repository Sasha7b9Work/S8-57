#include "defines.h"
#include "Multimeter/Multimeter.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Display/Display_Primitives.h"
#include "Hardware/Beeper.h"
#include "Utils/Debug.h"


using namespace Multimeter::Settings;

using Display::Primitives::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// ƒанные дл€ вывода.
static char outBuffer[10];

static void PrepareBell(const char *);
static void PrepareConstantVoltage(const char *);
static void PrepareVariableVoltage(const char *);
static void PrepareConstantCurrent(const char *);
static void PrepareVariableCurrent(const char *);
static void PrepareResistance(const char *);
static void PrepareTestDiode(const char *);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static bool MeasureNotReceive()
{
    return outBuffer[0] == '-';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawChar(uint i)
{
    static const int y = 50;
    
    static int x = 0;
    
    static const int dX = 30;

    /// true, если точка уже выведена
    static bool point = false;
    
    char symbols[2] = {outBuffer[i], 0};
    
    if(i == 0)
    {
        Text(symbols).Draw(10, y);
    }
    else
    {
        Text(symbols).Draw(point ? (x - 18) : x, y);
    }
    
    x += dX;

    if (i == 0)
    {
        x = 35;
        point = false;
    }

    if (outBuffer[i] == '.')
    {
        point = true;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawMeasure()
{
    Color color = MeasureNotReceive() ? Color::GRAY_50 : Color::FILL;

    Font::SetCurrent(Font::Type::_Big51);

    Color::SetCurrent(color);
    
    for(uint i = 0; i < 7; i++)
    {
        DrawChar(i);
    }

    Font::SetSpacing(5);

    Text(&outBuffer[7]).Draw(205, 50);

    Font::SetSpacing(1);

    Font::Pop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::Update()
{
    Painter::BeginScene(Color::BACK);

    DrawMeasure();

    Color::SetCurrent(Color::FILL);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int GetRange()
{
    if (MULTI_MEASURE == Multimeter::Measure::VoltageDC)
    {
        return MULTI_RANGE_VOLTAGE_DC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::VoltageAC)
    {
        return MULTI_RANGE_VOLTAGE_AC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentDC)
    {
        return MULTI_RANGE_CURRENT_DC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::CurrentAC)
    {
        return MULTI_RANGE_CURRENT_AC;
    }
    else if (MULTI_MEASURE == Multimeter::Measure::Resistance)
    {
        return MULTI_RANGE_RESISTANCE;
    }

    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::ChangedMode()
{
    std::memset(outBuffer, '-', 7); //-V512

    static const int position[Measure::Size][4] =
    {
        {2, 3, 4},      // VoltageDC
        {2, 3, 4},      // VoltageAC
        {3, 2},         // CurrentDC
        {3, 2},         // CurrentAC
        {2, 3, 4, 3},   // Resistance
        {2},            // TestDiode
        (2),            // Bell
    };
    
    static const pString suffix[Measure::Size][4] =
    {
        {"V=", "V=", "V="},
        {"V~", "V~", "V~"},
        {"mA=", "A="},
        {"mA=", "A="},
        {"kQ", "kQ", "kQ", "MQ"},
        {"V="},
        {"kQ="}
    };

    outBuffer[position[MULTI_MEASURE][GetRange()]] = '.';
    
    std::strcpy(&outBuffer[7], suffix[MULTI_MEASURE][GetRange()]);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::SetMeasure(const uint8 buf[13])
{
    typedef void(*pFuncVCC)(const char *);

    static const struct Func
    {
        pFuncVCC func;
        Func(pFuncVCC f) : func(f) {};
    }
    funcs[Multimeter::Measure::Size] =
    {
        PrepareConstantVoltage,
        PrepareVariableVoltage,
        PrepareConstantCurrent,
        PrepareVariableCurrent,
        PrepareResistance,
        PrepareTestDiode,
        PrepareBell
    };

    Measure::E meas = Measure::GetCode((const char *)buf);

    if (meas >= Measure::Size)
    {
        return;
    }

    std::memcpy(outBuffer, buf + 1, 7);

    funcs[meas].func((const char *)buf);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareTestDiode(const char *)
{
    std::strcpy(outBuffer + 7, "V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantVoltage(const char *) //-V524
{
    std::strcpy(outBuffer + 7, "V=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableVoltage(const char *)
{
    std::strcpy(outBuffer + 7, "V~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantCurrent(const char *buf)
{
    std::strcpy(outBuffer + 7, (buf[10] == '1') ? "A=" : "mA=");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableCurrent(const char *buf)
{
    std::strcpy(outBuffer + 7, (buf[10] == '1') ? "A~" : "mA~");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PrepareResistance(const char *buf)
{
    outBuffer[7] = buf[8];
    outBuffer[8] = buf[9];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ResistanceLess100()
{
    return ((outBuffer[1] == '0') && (outBuffer[3] == '0'));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareBell(const char *)
{
    std::strcpy(outBuffer + 7, "kQ");

    if (ResistanceLess100())
    {
        Beeper::Bell::On();
    }
    else
    {
        Beeper::Bell::Off();
    }
}
