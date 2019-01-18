#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "device.h"
#include "Osci/Osci.h"

#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "FPGA/FPGA_HAL.h"
#include <cstring>
#endif


using namespace FPGA;
using namespace Osci::Settings;


extern bool givingStart;

extern uint8 dataRand[Chan::Number][FPGA::MAX_NUM_POINTS];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                      //  2нс 5нс 10нс 20нс 50нс
const int Kr[] = { 50, 20, 10,  5,   2 };

volatile static int numberMeasuresForGates = 1000;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// „итать канал в рандомизаторе с адреса address
void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);
static int CalculateShift();
static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Init()
{
    Settings::Range::LoadBoth();
    Settings::RShift::Load(Chan::A);
    Settings::RShift::Load(Chan::B);
    Settings::Trig::Input::Load();
    Settings::Trig::Level::Load();
    Settings::TBase::Load();
    Settings::TShift::Load();
    FPGA::Settings::LoadCalibratorMode();
    Settings::LoadHoldfOff();

    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::DeInit()
{
    Stop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Update()
{
    if (!Device::State::InModeOsci())
    {
        return;
    }

    if (!FPGA::isRunning)
    {
        return;
    };

    int number = (IN_RANDOMIZE_MODE) ? Kr[SET_TBASE] : 1;

    for (int i = 0; i < number; i++)
    {
        HAL::ReadFlag();

        if (HAL::GetFlag::PRED() && !givingStart)
        {
            if (START_MODE_IS_AUTO && HAL::GetFlag::HOLD_OFF())
            {
                GiveStart();
                givingStart = true;
            }
            if (!HAL::GetFlag::TRIG_READY())
            {
                Trig::pulse = false;
            }
        }

        if (HAL::GetFlag::DATA_READY())
        {
            ReadData();
            if (START_MODE_IS_SINGLE)
            {
                OnPressStart();
                Trig::pulse = false;
            }
            else
            {
                Start();
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Stop(bool)
{
    isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data) // -V2506
{
    int Tsm = CalculateShift();

    if (Tsm == NULL_TSHIFT)
    {
        return;
    }

    int step = Kr[SET_TBASE];

    int index = Tsm - step;

    uint8 *dataRead = &dataRand[ch][0];
    dataRead = &dataRand[ch][index];    // -V519

    while (index < 0)
    {
        volatile uint8 d = *address;
        d = d;
        index += step;
        dataRead += step;
    }

    uint8 *last = &dataRand[ch][FPGA::NumPoints()];

    while (dataRead < last)
    {
        *dataRead = *address;
        dataRead += step;
    }

    std::memcpy(data, &dataRand[ch][0], (uint)FPGA::NumPoints());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateShift()
{
    uint16 min = 0;
    uint16 max = 0;

    if (!CalculateGate(valueADC, &min, &max))
    {
        return NULL_TSHIFT;
    }

    int deltaMAX = set.dbg_enum_gate_max * 10;
    int deltaMIN = set.dbg_enum_gate_min * 10;

    if (valueADC > max - deltaMAX || valueADC < min + deltaMIN)
    {
        return NULL_TSHIFT;
    }

    if (IN_RANDOMIZE_MODE)
    {

        float tin = (float)(valueADC - min + deltaMIN) / (max - deltaMAX - (min + deltaMIN));
        int retValue = (int)(tin * Kr[SET_TBASE]);

        return retValue;
    }

    return NULL_TSHIFT;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    static float minGate = 0.0F;
    static float maxGate = 0.0F;

    if (rand < 500 || rand > 4000)
    {
        ///LOG_WRITE("ќЎ»Ѕ ј!!! считано %d", rand);
        return false;
    }

    static int numElements = 0;
    static uint16 min = 0xffff;
    static uint16 max = 0;

    numElements++;

    bool retValue = true;

    if (rand < min)
    {
        min = rand;
    }
    if (rand > max)
    {
        max = rand;
    }

    if (minGate == 0.0F)    // -V550
    {
        *eMin = min;
        *eMax = max;
        if (numElements < numberMeasuresForGates)
        {
            return true;
        }
        minGate = min;
        maxGate = max;
        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    if (numElements >= numberMeasuresForGates)
    {
        minGate = 0.8F * minGate + min * 0.2F;
        maxGate = 0.8F * maxGate + max * 0.2F;

        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    *eMin = (uint16)(minGate);      // -V519 // -V2004
    *eMax = (uint16)(maxGate - 50); // -V519 // -V2004

    if (rand < *eMin || rand > *eMax)
    {
        return false;
    }

    return retValue;
}
