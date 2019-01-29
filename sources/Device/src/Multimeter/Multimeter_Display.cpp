#include "defines.h"
#include "Multimeter/Multimeter.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Display/Display_Primitives.h"


using namespace Multimeter::Settings;

using Display::Primitives::Text;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этом буфере готовая к выводу информация
#define SIZE_OUT 15
char out[SIZE_OUT];

namespace Multimeter
{
    namespace Display
    {
        static void PrepareRing();
        static void PrepareConstantVoltage();
        static void PrepareVariableVoltage();
        static void PrepareConstantCurrent();
        static void PrepareVariableCurrent();
        static void PrepareResistance();
        static void PrepareTestDiode();
    }
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::Display::Update()
{
    struct Func
    {
        pFuncVV func;
        Func(pFuncVV f) : func(f) {};
    };

    static const Func funcs[Multimeter::Measure::Number] =
    {
        PrepareConstantVoltage,
        PrepareVariableVoltage,
        PrepareConstantCurrent,
        PrepareVariableCurrent,
        PrepareResistance,
        PrepareTestDiode,
        PrepareRing
    };

    Painter::BeginScene(Color::BACK);

    std::memset(out, 0, SIZE_OUT);

    Measure::E meas = Measure::ForSymbol(buffer[7]);
    if (meas == Measure::Number)
    {
        meas = MULTI_MEASURE;
    }

    funcs[meas].func();

    Text(out, 5).Draw(30, 30, (buffer[0] == '8') ? Color::GRAY_50 : Color::FILL);

    Color::SetCurrent(Color::FILL);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Multimeter::Display::PrepareRing()
{
    out[0] = buffer[2];
    out[1] = '.';
    out[2] = buffer[3];
    out[3] = buffer[4];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'к';
    out[8] = 'О';
    out[9] = 'м';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Multimeter::Display::PrepareTestDiode()
{
    out[0] = (char)buffer[1];
    out[1] = buffer[2];
    out[2] = '.';
    out[3] = buffer[3];
    out[4] = buffer[4];
    out[5] = buffer[5];
    out[6] = buffer[6];
    out[7] = ' ';
    out[8] = 'V';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Multimeter::Display::PrepareConstantVoltage()
{
    out[0] = buffer[1];
    out[1] = buffer[2];
    out[5] = buffer[5];
    out[6] = buffer[6];
    out[7] = ' ';
    out[8] = 'V';
    out[9] = '=';

    switch (MULTI_RANGE_DC)
    {
    case RangeDC::_2V:
        out[2] = '.';
        out[3] = buffer[3];
        out[4] = buffer[4];
        break;
    case RangeDC::_20V:
        out[2] = buffer[3];
        out[3] = '.';
        out[4] = buffer[4];
        break;
    case RangeDC::_500V:
        out[2] = buffer[3];
        out[3] = buffer[4];
        out[4] = '.';
        break;
    };
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Multimeter::Display::PrepareVariableVoltage()
{
    out[0] = buffer[2];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'V';
    out[8] = '~';

    switch (MULTI_RANGE_AC)
    {
    case RangeAC::_2V:
        out[1] = '.';
        out[2] = buffer[3];
        out[3] = buffer[4];
        break;
    case RangeAC::_20V:
        out[1] = buffer[3];
        out[2] = '.';
        out[3] = buffer[4];
        break;
    case RangeAC::_400V:
        out[1] = buffer[3];
        out[2] = buffer[4];
        out[3] = '.';
        break;
    };
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Multimeter::Display::PrepareConstantCurrent()
{
    out[0] = (char)buffer[1];
    out[1] = buffer[2];
    out[2] = '.';
    out[3] = buffer[3];
    out[4] = buffer[4];
    out[5] = buffer[5];
    out[6] = buffer[6];
    out[7] = ' ';
    out[8] = 'A';
    out[9] = '=';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Multimeter::Display::PrepareVariableCurrent()
{
    out[0] = buffer[2];
    out[1] = '.';
    out[2] = buffer[3];
    out[3] = buffer[4];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'A';
    out[8] = '~';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::PrepareResistance()
{
    out[0] = buffer[2];
    out[4] = buffer[5];
    out[5] = buffer[6];
    out[6] = ' ';
    out[7] = 'к';
    out[8] = 'О';
    out[9] = 'м';

    switch (MULTI_RANGE_RESISTANCE)
    {
    case RangeResistance::_2k:
        out[1] = '.';
        out[2] = buffer[3];
        out[3] = buffer[4];
        break;
    case RangeResistance::_20k:
        out[1] = buffer[3];
        out[2] = '.';
        out[3] = buffer[4];
        break;
    case RangeResistance::_200k:
        out[1] = buffer[3];
        out[2] = buffer[4];
        out[3] = '.';
        break;
    case RangeResistance::_10M:
        out[1] = buffer[3];
        out[2] = '.';
        out[3] = buffer[4];
        out[7] = 'M';
        break;
    }
}
