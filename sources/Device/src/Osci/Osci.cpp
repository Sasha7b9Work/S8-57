#include "defines.h"
#include "device.h"
#include "Transceiver.h"
#include "Data/Reader.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"
#include <cstring>
#include <cmath>

#include "Osci/Display/PainterData.h"
#include "Utils/Debug.h"


using namespace FPGA;
using namespace ::HAL;
using namespace ::HAL::ADDRESSES::FPGA;
using namespace Osci::Settings;


extern bool givingStart;

extern uint16 addrRead;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает true, если уже можно читать данные
static bool CanReadData();

namespace Osci
{
    static void UpdateFPGA();

    int addShift = 0;
                              //  2нс 5нс 10нс 20нс 50нс
    const int Kr[TBase::Size] = { 50, 20, 10,  5,   2 };
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Init()
{
    Stop();

    FPGA::HAL::LoadRegUPR();
    Settings::Range::LoadBoth();
    Settings::RShift::Load(Chan::A);
    Settings::RShift::Load(Chan::B);
    Settings::Trig::Input::Load();
    Settings::Trig::Level::Load();
    Settings::Trig::Polarity::Load();
    Settings::TBase::Load();
    Settings::TShift::Load();
    FPGA::Settings::LoadCalibratorMode();
    Settings::LoadHoldfOff();
    ::HAL::PIO::Init(::HAL::PIO::Port::_G, ::HAL::PIO::Pin::_1, ::HAL::PIO::Mode::Input, ::HAL::PIO::Pull::Up);
    Osci::Storage::Clear();
    FPGA::OnPressStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::DeInit()
{
    Stop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Start()
{
    givingStart = false;
    addrRead = 0xffff;

    FSMC::WriteToFPGA16(WR::PRED_LO, FPGA::pred);
    FSMC::WriteToFPGA16(WR::POST_LO, FPGA::post);
    FSMC::WriteToFPGA8(WR::START, 0xff);

    FPGA::timeStart = TIME_MS;

    if (InModeP2P())
    {
        Storage::PrepareNewFrameP2P();
    }

    FPGA::isRunning = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Stop(bool)
{
    FPGA::isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Restart()
{
    if (FPGA::IsRunning())
    {
        Stop();
        Start();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

    Measurements::SetData();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Osci::UpdateFPGA()
{
    int number = (Osci::InModeRandomizer()) ? Kr[SET_TBASE] : 1;

    number = 1;

    for (int i = 0; i < number; i++)
    {
        FPGA::HAL::ReadFlag();

        if (FPGA::HAL::GetFlag::PRED() && !givingStart)
        {
            if (!Osci::InModeRandomizer() && START_MODE_IS_AUTO && FPGA::HAL::GetFlag::HOLD_OFF())
            {
                GiveStart();
                givingStart = true;
            }
            if (!FPGA::HAL::GetFlag::TRIG_READY())
            {
                Trig::pulse = false;
            }
        }

        if (FPGA::HAL::GetFlag::DATA_READY())
        {
            if (CanReadData())
            {
                Timer::PauseOnTicks(5 * 90 * 20);

                ReadData();

                if (START_MODE_IS_SINGLE)
                {
                    OnPressStart();
                    Trig::pulse = false;
                }
                else
                {
                    Timer::PauseOnTicks(5 * 90 * 20);
                    Start();
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::ReadPointP2P()
{
    if (!InModeP2P() || !FPGA::IsRunning())
    {
        return;
    }

    if (::HAL::PIO::Read(::HAL::PIO::Port::_G, ::HAL::PIO::Pin::_1))
    {
        BitSet16 dataA(FSMC::ReadFromFPGA(RD::DATA_A), FSMC::ReadFromFPGA(RD::DATA_A + 1));
        BitSet16 dataB(FSMC::ReadFromFPGA(RD::DATA_B), FSMC::ReadFromFPGA(RD::DATA_B + 1));
        Osci::Storage::GetFrameP2P()->AddPoints(dataA, dataB);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void BalanceChannel(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Osci::Settings::Range::Set(ch, range);

    set.addRShift[ch][range] = 0;

    Osci::Settings::RShift::Set(ch, Osci::Settings::RShift::ZERO);

    Osci::Start();

    float sum = 0;

    int numPoints = 0;

    uint8 *address = (ch == Chan::A) ? RD::DATA_A : RD::DATA_B;
    address++;

    while (numPoints < 100)
    {
        if(!Transceiver::InInteraction())
        {
            if (::HAL::PIO::Read(::HAL::PIO::Port::_G, ::HAL::PIO::Pin::_1))
            {
                sum += FSMC::ReadFromFPGA(address);
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0)
    {
        set.addRShift[ch][range] = (int8)(delta * 200.0F / 125.0F + 0.5F);
    }
    else
    {
        set.addRShift[ch][range] = (int8)(delta * 200.0F / 125.0F - 0.5F);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Balance(Chan::E ch)
{
    FPGA::Settings::ModeCouple::Set(ch, FPGA::Settings::ModeCouple::GND);

    SET_TBASE = Osci::Settings::TBase::_100ms;

    Osci::Settings::TBase::Load();

    for (int range = 0; range < Range::Size; range++)
    {
        BalanceChannel(ch, (Range::E)range);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Osci::InModeP2P()
{
    return (SET_TBASE >= TBase::MIN_P2P);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Osci::InModeRandomizer()
{
    return (SET_TBASE <= TBase::_50ns);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool CanReadData()
{
    if (ENUM_SIGNALS_IN_SEC.value == Display::ENumSignalsInSec::_25)
    {
        return true;
    }
    
    static uint timePrevRead = 0;

    if (TIME_MS > timePrevRead + ENUM_SIGNALS_IN_SEC.TimeBetweenFramesMS())
    {
        timePrevRead = TIME_MS;

        return true;
    }

    return false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::OnChangedPoints()
{
    FPGA::HAL::LoadRegUPR();
    FPGA::Reset();
    Osci::Display::PainterData::ChangeTPos();
    FPGA::Reset();
    TShift::Set(SET_TSHIFT);
    FPGA::Reset();
    Osci::Storage::Clear();
}
