#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Cursors.h"
#include "Settings/Settings.h"
#include "Display/Grid.h"
#include "Menu/Menu.h"
#include "FPGA/FPGAMath.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include <cstring>
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
float Cursors::PosU(Chan::E ch, int numCur)
{
    return CURsU_POS(ch, numCur) / (Grid::Bottom() == Grid::FullBottom() ? 1.0f : 2.0f);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Cursors::NecessaryDraw()
{
    return (CURsU_ENABLED || CURsT_ENABLED) && (CURS_SHOW || Menu::GetNameOpenedPage() == Page::Name::Measures_Cursors_Set);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Cursors::Voltage(Chan::E source, int numCur, char buffer[20])
{
    float voltage = FPGAMath::VoltageCursor(Cursors::PosU(source, numCur), SET_RANGE(source), SET_RSHIFT(source));
    if (SET_DIVIDER_10(source))
    {
        voltage *= 10.0f;
    }
    return ::Voltage(voltage).ToString(true, buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Cursors::PosT(Chan::E ch, int num)
{
    float retValue = 0.0f;
    std::memcpy(&retValue, &set.curs_posCurT[ch][num], sizeof(float));
    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Cursors::SetCursPosT_temp(Chan::E ch, int num, float value)
{
    std::memcpy(&set.curs_posCurT[ch][num], &value, sizeof(float));
}
