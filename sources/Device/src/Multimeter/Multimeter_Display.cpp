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

/// Сообщение от мультиметра
/// Если нулевой элемент == 0, то выводить ничего не нужно
static char message[20] = { 0 };

static void PrepareRing();
static void PrepareConstantVoltage();
static void PrepareVariableVoltage();
static void PrepareConstantCurrent();
static void PrepareVariableCurrent();
static void PrepareResistance();
static void PrepareTestDiode();
static void PrepareZero();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Multimeter::Display::Update()
{
    static struct Func
    {
        pFuncVV func;
        Func(pFuncVV f) : func(f) {};
    }
    const funcs[Multimeter::Measure::Size] = //-V808
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

    //std::memset(out, 0, SIZE_OUT);
    //
    //Measure::E meas = Measure::ForSymbol(message[7]);
    //if (meas == Measure::Size)
    //{
    //    meas = MULTI_MEASURE;
    //}
    //
    if (Multimeter::zero == 1)
    {
        PrepareZero();
    }
    //else
    //{
    //    funcs[meas].func();
    //}

    Text(message, 5).Draw(30, 30, (message[0] == '8') ? Color::GRAY_50 : Color::FILL);

    Color::SetCurrent(Color::FILL);

    Menu::Draw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::ChangedMode()
{
    //std::memset(message, '8', 10); //-V512
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Multimeter::Display::SetMeasure(const char *buf)
{
    std::strcpy(message, buf);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareRing()
{
    out[0] = message[2];
    out[1] = '.';
    out[2] = message[3];
    out[3] = message[4];
    out[4] = message[5];
    out[5] = message[6];
    out[6] = ' ';
    out[7] = 'к';
    out[8] = 'О';
    out[9] = 'м';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareTestDiode()
{
    out[0] = (char)message[1];
    out[1] = message[2];
    out[2] = '.';
    out[3] = message[3];
    out[4] = message[4];
    out[5] = message[5];
    out[6] = message[6];
    out[7] = ' ';
    out[8] = 'V';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantVoltage()
{
    out[0] = message[1];
    out[1] = message[2];
    out[5] = message[5];
    out[6] = message[6];
    out[7] = ' ';
    out[8] = 'V';
    out[9] = '=';

    switch (MULTI_RANGE_DC)
    {
    case RangeDC::_2V:
        out[2] = '.';
        out[3] = message[3];
        out[4] = message[4];
        break;
    case RangeDC::_20V:
        out[2] = message[3];
        out[3] = '.';
        out[4] = message[4];
        break;
    case RangeDC::_500V:
        out[2] = message[3];
        out[3] = message[4];
        out[4] = '.';
        break;
    };
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableVoltage()
{
    out[0] = message[2];
    out[4] = message[5];
    out[5] = message[6];
    out[6] = ' ';
    out[7] = 'V';
    out[8] = '~';

    switch (MULTI_RANGE_AC)
    {
    case RangeAC::_2V:
        out[1] = '.';
        out[2] = message[3];
        out[3] = message[4];
        break;
    case RangeAC::_20V:
        out[1] = message[3];
        out[2] = '.';
        out[3] = message[4];
        break;
    case RangeAC::_400V:
        out[1] = message[3];
        out[2] = message[4];
        out[3] = '.';
        break;
    };
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareConstantCurrent()
{
    out[0] = (char)message[1];
    out[1] = message[2];
    out[2] = '.';
    out[3] = message[3];
    out[4] = message[4];
    out[5] = message[5];
    out[6] = message[6];
    out[7] = ' ';
    out[8] = 'A';
    out[9] = '=';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareVariableCurrent()
{
    out[0] = message[2];
    out[1] = '.';
    out[2] = message[3];
    out[3] = message[4];
    out[4] = message[5];
    out[5] = message[6];
    out[6] = ' ';
    out[7] = 'A';
    out[8] = '~';
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void PrepareZero()
{
    std::strcpy(message, "   Нуль");
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PrepareResistance()
{
    out[0] = message[2];
    out[4] = message[5];
    out[5] = message[6];
    out[6] = ' ';
    out[7] = 'к';
    out[8] = 'О';
    out[9] = 'м';

    switch (MULTI_RANGE_RESISTANCE)
    {
    case RangeResistance::_2k:
        out[1] = '.';
        out[2] = message[3];
        out[3] = message[4];
        break;
    case RangeResistance::_20k:
        out[1] = message[3];
        out[2] = '.';
        out[3] = message[4];
        break;
    case RangeResistance::_200k:
        out[1] = message[3];
        out[2] = message[4];
        out[3] = '.';
        break;
    case RangeResistance::_10M:
        out[1] = message[3];
        out[2] = '.';
        out[3] = message[4];
        out[7] = 'M';
        break;
    }
}
