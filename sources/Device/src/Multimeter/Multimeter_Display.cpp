#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Multimeter/Multimeter.h"
#include "Display/Painter.h"
#include "Settings/Settings.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// В этом буфере готовая к выводу информация
#define SIZE_OUT 15
char out[SIZE_OUT];


namespace Multimeter
{
    namespace Display
    {
        static void PrepareRing();
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

    Measure meas = Measure::ForSymbol(buffer[7]);
    if (meas == Measure::Number)
    {
        meas = MULTI_MEASURE;
    }

    funcs[meas].func();

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
