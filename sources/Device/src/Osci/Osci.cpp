#include "defines.h"
#include "device.h"
#include "common/Transceiver.h"
#include "Data/Reader.h"
#include "FPGA/ContextOsci.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/MemoryRAM.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <cstring>
#include <cmath>


extern bool givingStart;

extern uint16 addrRead;


/// Возвращает true, если уже можно читать данные
static bool CanReadData();

static void UpdateFPGA();

int Osci::addShift = 0;
//  2нс 5нс 10нс 20нс 50нс
const uint Osci::Kr[TBase::Count] = { 50, 20, 10,  5,   2 };



void Osci::Init()
{
    Stop();

    RAM::Init();

    ContextOsci::LoadRegUPR();
    Range::LoadBoth();
    RShift::Load(Chan::A);
    RShift::Load(Chan::B);
    TrigInput::Load();
    TrigLevel::Load();
    TrigPolarity::Load();
    TBase::Load();
    TShift::Load();
    ContextOsci::LoadCalibratorMode();
    LoadHoldfOff();
    HAL_PIO::Init(HPort::_G, HPin::_1, HMode::Input, HPull::Up);
    StorageOsci::Clear();
    ContextOsci::OnPressStart();
}


void Osci::DeInit()
{
    Stop();
}


void Osci::Start()
{
    givingStart = false;
    addrRead = 0xffff;

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, ContextOsci::pred);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, ContextOsci::post);
    HAL_FSMC::WriteToFPGA8(WR::START, 0xff);

    ContextOsci::timeStart = TIME_MS;

    if (InModeP2P())
    {
        StorageOsci::PrepareNewFrameP2P();
    }

    ContextOsci::isRunning = true;
}


void Osci::Stop(bool)
{
    ContextOsci::isRunning = false;
}


void Osci::Restart()
{
    if (ContextOsci::IsRunning())
    {
        Stop();
        Start();
    }
}


bool Osci::IsRunning()
{
    return ContextOsci::IsRunning();
}


void Osci::Update()
{
    if (!Device::State::InModeOsci())
    {
        return;
    }

    if (ContextOsci::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromStorage();

    AutoMeasurements::SetData();
}


static void UpdateFPGA()
{
    bool needStop = false;
    
    uint number = (Osci::InModeRandomizer()) ? Osci::Kr[set.time.base] : 1;

    for (uint i = 0; i < number; i++)
    {
        ContextOsci::ReadFlag();
    
        if (ContextOsci::GetFlag::PRED() && !givingStart)
        {
            if (!Osci::InModeRandomizer() && (set.trig.startMode == TrigStartMode::Auto) && ContextOsci::GetFlag::HOLD_OFF())
            {
                ContextOsci::GiveStart();
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
    
                ContextOsci::ReadData();
    
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
    if (!InModeP2P() || !ContextOsci::IsRunning())
    {
        return;
    }

    if (HAL_PIO::Read(HPort::_G, HPin::_1))
    {
        BitSet16 dataA(HAL_FSMC::ReadFromFPGA(RD::DATA_A), HAL_FSMC::ReadFromFPGA(RD::DATA_A + 1));
        BitSet16 dataB(HAL_FSMC::ReadFromFPGA(RD::DATA_B), HAL_FSMC::ReadFromFPGA(RD::DATA_B + 1));
        StorageOsci::GetFrameP2P()->AddPoints(dataA, dataB);
    }
}


static void BalanceChannel(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Range::Set(ch, range);

    set.dbg.addRShift[ch][range] = 0;

    RShift::Set(ch, RShift::ZERO);

    Osci::Start();

    float sum = 0;

    int numPoints = 0;

    uint8 *address = (ch == Chan::A) ? RD::DATA_A : RD::DATA_B;
    address++;

    while (numPoints < 100)
    {
        if(!Transceiver::inInteraction)
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
    ModeCouple::Set(ch, ModeCouple::GND);

    set.time.base = TBase::_100ms;

    TBase::Load();

    for (int range = 0; range < Range::Count; range++)
    {
        BalanceChannel(ch, static_cast<Range::E>(range));
    }
}


bool Osci::InModeP2P()
{
    return (set.time.base >= TBase::MIN_P2P);
}


bool Osci::InModeRandomizer()
{
    return (set.time.base <= TBase::_50ns);
}


static bool CanReadData()
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
    ContextOsci::LoadRegUPR();
    ContextOsci::Reset();
    DisplayOsci::PainterData::ChangeTPos();
    ContextOsci::Reset();
    TShift::Set(set.time.shift);
    ContextOsci::Reset();
    StorageOsci::Clear();
}


Osci::StructReadRand Osci::GetInfoForReadRand(int Tsm, const uint8 *address)
{
    static StructReadRand result = { 0, 0 };

    if (Tsm != NULL_TSHIFT)
    {
        result.step = Kr[set.time.base];

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


void Osci::SetModeWork(ModeWork::E mode)
{
    set.mem.modeWork = mode;
}
