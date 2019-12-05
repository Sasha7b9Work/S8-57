#include "defines.h"
#include "device.h"
#include "common/Transceiver.h"
#include "Data/Reader.h"
#include "FPGA/ContextOsci.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <cstring>
#include <cmath>


extern bool givingStart;

int Osci::addShift = 0;


void Osci::Init()
{
    Stop();

    RAM::Init();
    FPGA::LoadRegUPR();
    Range::LoadBoth();
    RShift(Chan::A).Load();
    RShift(Chan::B).Load();
    TrigInput().Load();
    TrigLevel().Load();
    TrigPolarity().Load();
    TBase().Load();
    TShift().Load();
    FPGA::LoadCalibratorMode();
    LoadHoldfOff();
    HAL_PIO::Init(HPort::_G, HPin::_1, HMode::Input, HPull::Up);
    ContextOsci::OnPressStart();
}


void Osci::DeInit()
{
    Stop();
}


void Osci::Start()
{
    givingStart = false;
    FPGA::addrRead = 0xffff;

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, ContextOsci::pred);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, ContextOsci::post);
    HAL_FSMC::WriteToFPGA8(WR::START, 0xff);

    ContextOsci::timeStart = TIME_MS;

    if (InModeP2P())
    {
        RAM::PrepareNewFrameP2P();
    }

    ContextOsci::isRunning = true;
}


void Osci::Stop(bool)
{
    ContextOsci::isRunning = false;
}


void Osci::Restart()
{
    if (FPGA::IsRunning())
    {
        Stop();
        Start();
    }
}


bool Osci::IsRunning()
{
    return FPGA::IsRunning();
}


void Osci::Update()
{
    if (!Device::InModeOsci())
    {
        return;
    }

    if (FPGA::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromRAM();

    AutoMeasurements::SetData();
}


void Osci::UpdateFPGA()
{
    bool needStop = false;
    
    uint number = (Osci::InModeRandomizer()) ? TBase().RandK() : 1;

    for (uint i = 0; i < number; i++)
    {
        FPGA::ReadFlag();
    
        if (ContextOsci::GetFlag::PRED() && !givingStart)
        {
            if (!Osci::InModeRandomizer() && (set.trig.startMode == TrigStartMode::Auto) && ContextOsci::GetFlag::HOLD_OFF())
            {
                FPGA::GiveStart();
                givingStart = true;
            }
            if (!ContextOsci::GetFlag::TRIG_READY())
            {
                Trig::pulse = false;
            }
        }
    
        if (ContextOsci::GetFlag::DATA_READY())
        {
            if (CanReadData())
            {
                Timer::PauseOnTicks(5 * 90 * 20);
    
                FPGA::ReadData();
    
                if (set.trig.startMode == TrigStartMode::Single)
                {
                    needStop = true;
                    Trig::pulse = false;
                }
                else
                {
                    Timer::PauseOnTicks(5 * 90 * 20);
                    Osci::Start();
                }
            }
        }
    }
    
    if(needStop)
    {
        ContextOsci::OnPressStart();
    }
}


void Osci::ReadPointP2P()
{
    if (InModeP2P() && FPGA::IsRunning() && HAL_PIO::Read(HPort::_G, HPin::_1))
    {
        BitSet16 dataA(HAL_FSMC::ReadFromFPGA(RD::DATA_A), HAL_FSMC::ReadFromFPGA(RD::DATA_A + 1));
        BitSet16 dataB(HAL_FSMC::ReadFromFPGA(RD::DATA_B), HAL_FSMC::ReadFromFPGA(RD::DATA_B + 1));

        LOG_WRITE("%d %d", dataA.halfWord, dataB.halfWord);

        RAM::GetFrameP2P()->AddPoints(dataA, dataB);
    }
}


void Osci::BalanceChannel(Chan::E ch, Range::E range)
{
    Stop();

    Range(ch).Load(range);

    set.dbg.addRShift[ch][range] = 0;

    RShift(ch, 0);

    Start();

    float sum = 0;

    int numPoints = 0;

    uint8 *address = (ch == Chan::A) ? RD::DATA_A : RD::DATA_B;
    address++;

    while (numPoints < 100)
    {
        if(!Transceiver::InInteraction())
        {
            if (HAL_PIO::Read(HPort::_G, HPin::_1))
            {
                sum += HAL_FSMC::ReadFromFPGA(address);
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0)
    {
        set.dbg.addRShift[ch][range] = static_cast<int8>(delta * 200.0F / 125.0F + 0.5F);
    }
    else
    {
        set.dbg.addRShift[ch][range] = static_cast<int8>(delta * 200.0F / 125.0F - 0.5F);
    }
}


void Osci::Balance(Chan::E ch)
{
    ModeCouple(ch, ModeCouple::GND);

    TBase::Load(TBase::_100ms);

    for (int range = 0; range < Range::Count; range++)
    {
        BalanceChannel(ch, static_cast<Range::E>(range));
    }
}


bool Osci::InModeP2P()
{
    return (TBase() >= TBase::MIN_P2P);
}


bool Osci::InModeRandomizer()
{
    return (TBase() <= TBase::_50ns);
}


bool Osci::CanReadData()
{
    if (set.disp.ENumSignalsInSec.value == ENumSignalsInSec::_25)
    {
        return true;
    }
    
    static uint timePrevRead = 0;

    if (TIME_MS > timePrevRead + set.disp.ENumSignalsInSec.TimeBetweenFramesMS())
    {
        timePrevRead = TIME_MS;

        return true;
    }

    return false;
}


void Osci::OnChangedPoints()
{
    FPGA::LoadRegUPR();
    FPGA::Reset();
    DisplayOsci::PainterData::ChangeTPos();
    FPGA::Reset();
    TShift(set.time.shift);
    FPGA::Reset();
    RAM::Init();
}


Osci::StructReadRand Osci::GetInfoForReadRand(int Tsm, const uint8 *address)
{
    static StructReadRand result = { 0, 0 };

    if (Tsm != NULL_TSHIFT)
    {
        result.step = TBase().RandK();

        int index = Tsm - addShift;

        while (index < 0)
        {
            index += result.step;
            volatile uint8 d = *address;
            d = d;
        }

        result.posFirst = static_cast<uint>(index);
    }

    return result;
}
