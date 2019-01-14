#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include "Utils/Values.h"
#include "FPGA/FPGAMath.h"
#endif


using FPGA::SET::TBase;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
int FPGA::SET::TShift::Min()
{
#define k 0
#define mul 2

    static const struct StructENumPoints
    {
        int m[3];
        StructENumPoints(int m0, int m1, int m2) { m[0] = m0; m[1] = m1; m[2] = m2; }
    }
    m[ENumPointsFPGA::Size] =
    {
        StructENumPoints( -256 * mul + k,  -128 * mul + k, 0 * mul + k),  // 512
        StructENumPoints( -512 * mul + k,  -256 * mul + k, 0 * mul + k),  // 1024
        StructENumPoints(-1024 * mul + k,  -512 * mul + k, 0 * mul + k),  // 2048
        StructENumPoints(-2048 * mul + k, -1024 * mul + k, 0 * mul + k),  // 4096
        StructENumPoints(-4096 * mul + k, -2048 * mul + k, 0 * mul + k)   // 8192
    };
  

    if (SET_PEAKDET_DIS)
    {
        return m[(int)FPGA_ENUM_POINTS].m[TPOS];
    }

    return m[(int)FPGA_ENUM_POINTS].m[TPOS] * 2; // Для пикового детектора это значение должно быть в два раза больше

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::SET::TShift::Zero()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::SET::TShift::Max()
{
    return 60000;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::SET::TShift::InPoints()
{
    return SET_PEAKDET_EN ? value : (value * 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
FPGA::SET::TShift::TShift(int tshift)
{
    value = tshift;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::TShift::Set(int tShift)
{
    LIMITATION(tShift, Min(), Max());

    value = tShift;
    SET_TSHIFT.value = tShift;
    
    TShift::Load();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String FPGA::SET::TShift::ToString(TBase::E tBase) const
{
    float time = FPGAMath::TShift2Abs(value, tBase);
    return Time(time).ToString(true);
}
