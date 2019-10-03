#include "defines.h"
#include "device.h"
#include "Transceiver.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"
#include "Utils/Debug.h"
#include <cstring>
#include <cmath>


using namespace FPGA;
using namespace Address;


extern bool givingStart;

extern uint16 addrRead;


/// Возвращает true, если уже можно читать данные
static bool CanReadData();

static void UpdateFPGA();

int Osci::addShift = 0;
//  2нс 5нс 10нс 20нс 50нс
const int Osci::Kr[TBase::Size] = { 50, 20, 10,  5,   2 };



void Osci::Init()
{
    Stop();

    FPGA::LoadRegUPR();
    Range::LoadBoth();
    RShift::Load(Chan::A);
    RShift::Load(Chan::B);
    TrigInput::Load();
    Trig::Level::Load();
    TrigPolarity::Load();
    TBase::Load();
    TShift::Load();
    FPGA::LoadCalibratorMode();
    LoadHoldfOff();
    HAL_PIO::Init(HPort::_G, HPin::_1, HMode::Input, HPull::Up);
    StorageOsci::Clear();
    FPGA::OnPressStart();
}


void Osci::DeInit()
{
    Stop();
}


void Osci::Start()
{
    givingStart = false;
    addrRead = 0xffff;

    HAL_FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    HAL_FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);
    HAL_FSMC::WriteToFPGA8(WR::START, 0xff);

    FPGA::timeStart = TIME_MS;

    if (InModeP2P())
    {
        StorageOsci::PrepareNewFrameP2P();
    }

    FPGA::isRunning = true;
}


void Osci::Stop(bool)
{
    FPGA::isRunning = false;
}


void Osci::Restart()
{
    if (FPGA::IsRunning())
    {
        Stop();
        Start();
    }
}


void Osci::Update()
{
    if (!Device::State::InModeOsci())
    {
        return;
    }

    if (FPGA::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromStorage();

    MeasurementsOsci::SetData();
}


static void UpdateFPGA()
{
    bool needStop = false;
    
    int number = (Osci::InModeRandomizer()) ? Osci::Kr[set.time.base] : 1;

    for (int i = 0; i < number; i++)
    {
        FPGA::ReadFlag();
    
        if (GetFlag::PRED() && !givingStart)
        {
            if (!Osci::InModeRandomizer() && (set.trig.startMode == TrigStartMode::Auto) && GetFlag::HOLD_OFF())
            {
                GiveStart();
                givingStart = true;
            }
            if (!FPGA::GetFlag::TRIG_READY())
            {
                Trig::pulse = false;
            }
        }
    
        if (FPGA::GetFlag::DATA_READY())
        {
            if (CanReadData())
            {
                Timer::PauseOnTicks(5 * 90 * 20);
    
                ReadData();
    
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
        OnPressStart();
    }
}


void Osci::ReadPointP2P()
{
    if (!InModeP2P() || !FPGA::IsRunning())
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
        set.dbg.addRShift[ch][range] = (int8)(delta * 200.0F / 125.0F + 0.5F);
    }
    else
    {
        set.dbg.addRShift[ch][range] = (int8)(delta * 200.0F / 125.0F - 0.5F);
    }
}


void Osci::Balance(Chan::E ch)
{
    ModeCouple::Set(ch, ModeCouple::GND);

    set.time.base = TBase::_100ms;

    TBase::Load();

    for (int range = 0; range < Range::Size; range++)
    {
        BalanceChannel(ch, (Range::E)range);
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
    FPGA::LoadRegUPR();
    FPGA::Reset();
    DisplayOsci::PainterData::ChangeTPos();
    FPGA::Reset();
    TShift::Set(set.time.shift);
    FPGA::Reset();
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

        result.posFirst = index;
    }

    return result;
}
