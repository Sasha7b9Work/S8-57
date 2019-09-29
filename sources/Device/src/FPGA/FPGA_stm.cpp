#include "defines.h"
#include "log.h"
#include "device.h"
#include "FPGA.h"
#include "FPGA_HAL.h"
#include "AD9286.h"
#include "Display/Display.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Values.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci_Storage.h"
#include "Data/Reader.h"
#include <stdlib.h>
#include <cstring>


using namespace FPGA::HAL::GPIO;
using namespace ::HAL;
using namespace ::HAL::ADDRESSES::FPGA;

using Hardware::AD9286;


extern bool givingStart;
extern uint8 dataRand[Chan::Size][FPGA::MAX_NUM_POINTS];

/// Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
uint16 addrRead = 0xffff;

volatile static int numberMeasuresForGates = 10000;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace FPGA
{
    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    class ADC3_
    {
    public:
        static void Init()
        {
            ::HAL::ADC3_::Init();
        }
    };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::Init()
{
    givingStart = false;

    HAL::GPIO::Init();

    SetPin(Pin::SPI3_CS1);
    SetPin(Pin::SPI3_CS2);

    ResetPin(Pin::SPI3_SCK);
    ResetPin(Pin::SPI3_DAT);

    AD9286::Init();

    ADC3_::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    static float minGate = 0.0F;
    static float maxGate = 0.0F;

    if (rand < 500 || rand > 4000)
    {
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

    if (minGate == 0.0F)    // -V550 //-V2550 //-V550
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

        LOG_WRITE("Новые ворота %d %d", (uint16)minGate, (uint16)(maxGate - 50));
    }

    *eMin = (uint16)(minGate);      // -V519 // -V2004
    *eMax = (uint16)(maxGate - 50); // -V519 // -V2004

    if (rand < *eMin || rand > *eMax)
    {
        return false;
    }

    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int CalculateShift()
{
    uint16 min = 0;
    uint16 max = 0;

    if (!CalculateGate(FPGA::valueADC, &min, &max))
    {
        return NULL_TSHIFT;
    }

    int deltaMAX = set.dbg.enum_gate_max * 10;
    int deltaMIN = set.dbg.enum_gate_min * 10;

    if (FPGA::valueADC > max - deltaMAX || FPGA::valueADC < min + deltaMIN)
    {
        return NULL_TSHIFT;
    }

    if (Osci::InModeRandomizer())
    {

        float tin = (float)(FPGA::valueADC - min + deltaMIN) / (max - deltaMAX - (min + deltaMIN));
        int retValue = (int)(tin * Osci::Kr[SET_TBASE]);

        return retValue;
    }

    return NULL_TSHIFT;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data) // -V2506
{
    int Tsm = CalculateShift();

    if (Tsm == NULL_TSHIFT)
    {
        std::memcpy(data, &dataRand[ch][0], FPGA_NUM_POINTS);
        return false;
    }

    Osci::StructReadRand infoRead = Osci::GetInfoForReadRand(Tsm, address);

    int step = infoRead.step;

    uint8 *dataRead = &dataRand[ch][infoRead.posFirst];

    uint8 *last = &dataRand[ch][FPGA_NUM_POINTS];

    if (set.disp.ENumAverage > 1)
    {
        uint8 *dataPointer = &data[infoRead.posFirst];              // Указатель в переданном массиве

        while (dataRead < last)
        {
            *dataRead = *address;
            *dataPointer = *dataRead;

            dataRead += step;
            dataPointer += step;
        }
    }
    else
    {
        while (dataRead < last)
        {
            *dataRead = *address;
            dataRead += step;
        }

        std::memcpy(data, &dataRand[ch][0], FPGA_NUM_POINTS);
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::ReadDataChanenl(Chan::E ch, uint8 data[FPGA::MAX_NUM_POINTS])
{
    uint numPoints = FPGA_NUM_POINTS;

    if (addrRead == 0xffff)
    {
        int k = 1;

        if (Osci::InModeRandomizer())
        {
            k = Osci::Kr[SET_TBASE];
        }

        addrRead = (uint16)(ReadLastRecord(ch) - (int)numPoints / k);
    }

    FSMC::WriteToFPGA16(WR::PRED_LO, (uint16)(addrRead));
    FSMC::WriteToFPGA8(WR::START_ADDR, 0xff);


    uint8 *addr0 = Chan(ch).IsA() ? RD::DATA_A : RD::DATA_B;  // -V566
    uint8 *addr1 = addr0 + 1;

    if (Osci::InModeRandomizer())
    {
        return ReadDataChanenlRand(ch, addr1, data);
    }
    else
    {
        uint8 *p = data;

        *p = *addr0;    // Первая точка почему-то неправильная читается. Просто откидываем её.
        *p = *addr1;    // -V519

        if (SET_PEAKDET_EN)
        {
            for (uint i = 0; i < numPoints; i++)
            {
                *p++ = *addr0;
                *p++ = *addr1;
            }
        }
        else
        {
            for (uint i = 0; i < numPoints / 4U; ++i)   // -V112
            {
                *p++ = *addr1;
                *p++ = *addr1;
                *p++ = *addr1;
                *p++ = *addr1;
            }
        }
    }

    return true;
}
