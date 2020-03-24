#include "defines.h"
#include "log.h"
#include "FPGA/AD9286.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/BufferButtons.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"


struct Gates
{
    Gates() : minGate(0.0F), maxGate(0.0F) { }
    bool Calculate(uint16 value, uint16 *min, uint16 *max);

private:
    static const int numberMeasuresForGates = 10000;
    static const uint TIME_WAIT = 3000;
    float minGate;
    float maxGate;
    // Здесь хранятся два наименьших и два наибольших значения из всех подаваемых в функцию Calculate
    MinMax2 m;
    // Пересчитать значения ворот
    void RecalculateGates();

    void CalculateWithoutGates(uint16 *min, uint16 *max);
};


static Gates gates; // "Ворота" рандомизатора


uint16 Osci::addrRead = 0xffff;


bool Gates::Calculate(uint16 value, uint16 *min, uint16 *max)
{
    if(value < 250 || value > 4000)
    {
        return false;
    }

    m.Add(value);

    if((TIME_MS > TIME_WAIT) && (BufferButtons::TimeAfterControlMS() < TIME_WAIT))
    {
        CalculateWithoutGates(min, max);

        return true;
    }

    if(minGate == 0.0F)
    {
        *min = m.Min();
        *max = m.Max();
        if(m.Count() < numberMeasuresForGates)
        {
            return true;
        }
        minGate = m.Min();
        maxGate = m.Max();
        m.Reset();
    }

    if(m.Count() >= numberMeasuresForGates)
    {
        RecalculateGates();

        m.Reset();
    }

    *min = static_cast<uint16>(minGate);
    *max = static_cast<uint16>(maxGate);

    return (value >= *min) && (value <= *max);
}


void Gates::RecalculateGates()
{
    minGate = 0.8F * minGate + m.Min() * 0.2F;
    maxGate = 0.8F * maxGate + m.Max() * 0.2F;

    static uint timePrev = 0;

    LOG_WRITE("Новые ворота %d %d  время %d", static_cast<uint16>(minGate), static_cast<uint16>(maxGate), (TIME_MS - timePrev) / 1000);

    timePrev = TIME_MS;
}


void Gates::CalculateWithoutGates(uint16 *min, uint16 *max)
{
    if(minGate == 0.0F)
    {
        *min = m.Min();
        *max = m.Max();
    }
    else
    {
        *min = static_cast<uint16>(minGate);
        *max = static_cast<uint16>(maxGate);
    }
}


void FPGA::Init()
{
    forcedStart = false;

    Osci::InputController::Init();

    HAL_PIO::Set(PIN_SPI3_CS1);
    HAL_PIO::Set(PIN_SPI3_CS2);

    HAL_PIO::Reset(PIN_SPI3_SCK);
    HAL_PIO::Reset(PIN_SPI3_DAT);

    AD9286::Init();

    HAL_ADC3::Init();
}


int Osci::CalculateShift()
{
    uint16 min = 0;
    uint16 max = 0;

    if(!gates.Calculate(valueADC, &min, &max))
    {
        return NULL_TSHIFT;
    }

    if ((valueADC > max - setNRST.enumGameMax * 10) || (valueADC < min + setNRST.enumGameMin * 10))
    {
        return NULL_TSHIFT;
    }

    if (OSCI_IN_MODE_RANDOMIZER)
    {
        float tin = static_cast<float>(valueADC - min) / (max - min);
        return static_cast<int>(tin * TBase().RandK());
    }

    return NULL_TSHIFT;
}


bool Osci::ReadDataChannelRand(uint8 *addr, uint8 *data)
{
    int Tsm = CalculateShift();

    if (Tsm == NULL_TSHIFT)
    {
        return false;
    }

    StructReadRand infoRead = GetInfoForReadRand(Tsm, addr);

    uint step = infoRead.step;

    uint8 *dataRead = data + infoRead.posFirst;

    uint8 *last = data + ENumPointsFPGA::PointsInChannel();

    HAL_BUS::FPGA::SetAddrData(addr);

    if (ENumAverage() > 1)
    {
        uint8 *dataPointer = &data[infoRead.posFirst];              // Указатель в переданном массиве

        while (dataRead < last)
        {
            *dataRead = HAL_BUS::FPGA::ReadA0();
            *dataPointer = *dataRead;

            dataRead += step;
            dataPointer += step;
        }   
    }
    else
    {
        while (dataRead < last)
        {
            *dataRead = HAL_BUS::FPGA::ReadA0();
            dataRead += step;
        }
    }

    return true;
}


bool Osci::ReadDataChannel(Chan::E ch, uint8 *data)
{
    uint numPoints = ENumPointsFPGA::PointsInChannel();

    if (addrRead == 0xffff)
    {
        uint k = 1;

        if (OSCI_IN_MODE_RANDOMIZER)
        {
            k = TBase().RandK();
        }

        addrRead = static_cast<uint16>(ReadLastRecord(ch) - static_cast<int>(numPoints) / k);
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, static_cast<uint16>(addrRead));
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);


    uint8 *a0 = Chan(ch).IsA() ? RD::DATA_A : RD::DATA_B;  // -V566
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    if (OSCI_IN_MODE_RANDOMIZER)
    {
        return ReadDataChannelRand(a1, data);
    }
    else
    {
        uint8 *p = data;

        *p = HAL_BUS::FPGA::ReadA0();    // Первая точка почему-то неправильная читается. Просто откидываем её.
        *p = HAL_BUS::FPGA::ReadA1();    // -V519

        if (PeakDetMode().IsEnabled())
        {
            for (uint i = 0; i < numPoints; i++)
            {
                *p++ = HAL_BUS::FPGA::ReadA0();
                *p++ = HAL_BUS::FPGA::ReadA1();
            }
        }
        else
        {
            float stretch = HAL_BUS::FPGA::GetStretch(a1);

            for(uint i = 0; i < numPoints; i++)
            {
                int delta = VALUE::AVE - static_cast<int>(*a1);

                int result = static_cast<int>(VALUE::AVE - static_cast<int>(delta * stretch));

                if(result < VALUE::MIN)      { p[i] = VALUE::MIN; }
                else if(result > VALUE::MAX) { p[i] = VALUE::MAX; }
                else                         { p[i] = static_cast<uint8>(result); }
            }
        }
    }

    return true;
}
