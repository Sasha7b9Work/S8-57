#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "FPGA/FPGATypes.h"
#include "Hardware/FSMC.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "FPGA/FPGA.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Settings::LoadHoldfOff()
{
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_ENABLE, TRIG_HOLDOFF_ENABLED ? 1U : 0U);

    uint value = (uint)(0 - TRIG_HOLDOFF + 1);

    BitSet32 bs(value);

    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_LOW, bs.byte0);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_MID, bs.byte1);
    FSMC::WriteToFPGA8(WR_TRIG_HOLD_VALUE_HI, bs.byte2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Settings::TBase::Change(int delta)
{
    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&SET_TBASE), (uint8)(TBase::Number - 1));
    }
    else
    {
        if (SET_PEAKDET_EN &&                               // Если вклюён режим пикового детектора
            SET_TBASE == TBase::MIN_PEAK_DET)               // и установлен масштаб по времени, соответствующий минмальному в режиме пикового детектора :
        {
            ::Display::ShowWarning(Warning::EnabledPeakDet);  // выводим сообщение об этом
            return;                                         // и выходим
        }

        ::Math::LimitationDecrease<uint8>((uint8 *)(&SET_TBASE), 0);
    }

    Load();
    FPGA::Start();
}
