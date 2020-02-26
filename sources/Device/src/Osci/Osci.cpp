#include "defines.h"
#include "log.h"
#include "device.h"
#include "Osci/DeviceSettings.h"
#include "Hardware/Memory/Reader.h"
#include "FPGA/FPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Memory/RAM.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Utils/Values.h"
#include <cstring>


int    Osci::addShift = 0;
void (*Osci::funcStart)(bool) = EmptyFuncVB;
void (*Osci::funcStop)() = EmptyFuncVV;


void Osci::Init()
{
    Stop();

    RAM::Init();
    FPGA::LoadRegUPR();
    Range::LoadBoth();
    RShift(Chan::A).Load();
    RShift(Chan::B).Load();
    TrigInput::Load();
    TrigLevel().Load();
    TrigPolarity().Load();
    TBase::Set();
    TShift().Load();
    FPGA::LoadCalibratorMode();
    LoadHoldfOff();
    HAL_PIO::Init(PIN_P2P, HMode::Input, HPull::Up);
    ChangedTrigStartMode();
    Osci::Start(true);
}


void Osci::DeInit()
{
    Stop();
}


void Osci::Start(bool button)
{
    if(OSCI_IN_MODE_RANDOMIZER)
    {
        std::memset(IntRAM::DataRand(Chan::A), VALUE::NONE, FPGA::MAX_NUM_POINTS);
        std::memset(IntRAM::DataRand(Chan::B), VALUE::NONE, FPGA::MAX_NUM_POINTS);
    }

    funcStart(button);
}


void Osci::Restart()
{
    if(FPGA::IsRunning())
    {
        Stop();
        Start(true);
    }
}


void Osci::Update()
{
    if(!Device::InModeOsci())
    {
        return;
    }

    if(FPGA::IsRunning())
    {
        UpdateFPGA();
    };

    Reader::ReadDataFromRAM();

    AutoMeasurements::SetData();
}


void Osci::Stop()
{
    funcStop();
}


bool Osci::IsRunning()
{
    return FPGA::IsRunning();
}


void Osci::UpdateFPGA()
{
    uint number = OSCI_IN_MODE_RANDOMIZER ? TBase().RandK() : 1;

    RAM::NewFrameForRandomize();

    for (uint i = 0; i < number; i++)
    {
        FPGA::ReadFlag();
    
        ProcessFlagPred();

        if(ProcessFlagReady())
        {
            Osci::Stop();
            break;
        }
    }
}


void Osci::ProcessFlagPred()
{
    if(FPGA::flag.Pred() && !FPGA::forcedStart)
    {
        if(!OSCI_IN_MODE_RANDOMIZER && TrigStartMode::IsAuto() && FPGA::flag.HoldOff())
        {
            FPGA::ForcedStart();
        }
    }
}


bool Osci::ProcessFlagReady()
{
    bool needStop = false;

    if(FPGA::flag.DataReady())
    {
        if(CanReadData())
        {
            Timer::PauseOnTicks(5 * 90 * 20);

            ReadData();

            if(TrigStartMode::IsSingle())
            {
                needStop = true;
                Trig::pulse = false;
            }
            else
            {
                Timer::PauseOnTicks(5 * 90 * 20);
                Osci::Start(false);
            }
        }
    }

    return needStop;
}


bool Osci::CanReadData()
{
    if (ENumSignalsInSec::Is25())
    {
        return true;
    }
    
    static uint timePrevRead = 0;

    if (TIME_MS > timePrevRead + ENumSignalsInSec::TimeBetweenFramesMS())
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
    TShift::Set(TShift());
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


void Osci::OnPressStart()
{
    return IsRunning() ? Stop() : Start(true);
}


void Osci::ChangedTBase()
{
    SetFunctionsStartStop();
}


void Osci::ChangedTrigStartMode()
{
    Stop();

    SetFunctionsStartStop();

    if(!TrigStartMode::IsSingle())
    {
        Start(true);
    }

    // Елси находимся в режиме рандомизатора
    if(OSCI_IN_MODE_RANDOMIZER)
    {
        // и переключаемся на одиночный режим запуска, то надо сохранить имеющийся тип выборки, чтобы восстановить при возвращении в режим 
        // рандомизатора автоматический или ждущий
        if(TrigStartMode::IsSingle())
        {
            set.time.sampleTypeOld = SampleType();
            SampleType().Set(SampleType::Real);
        }
        else if(TrigStartMode::IsAuto())    // Иначе восстановим ранее сохранённый
        {
            SampleType().Set(set.time.sampleTypeOld);
        }
    }
}


void Osci::SetFunctionsStartStop()
{
    static const pFuncVB start[2][TrigStartMode::Count] =
    {
        //  Auto         Wait          Single
        { StartNormal, StartNormal,  StartNormal    },     // Normal mode
        { StartP2P,    StartWaitP2P, StartSingleP2P }      // P2P mode
    };

    static const pFuncVV stop[2][TrigStartMode::Count] =
    {
        //  Auto        Wait         Single
        { StopNormal, StopNormal,  StopNormal    },        // Normal mode
        { StopNormal, StopWaitP2P, StopSingleP2P }         // P2P mode
    };

    int index = OSCI_IN_MODE_P2P ? 1 : 0;

    funcStart = start[index][TrigStartMode()];

    funcStop = stop[index][TrigStartMode()];
}


void Osci::StartNormal(bool)
{
    FPGA::forcedStart = false;
    addrRead = 0xffff;

    FPGA::GiveStart(FPGA::pred, FPGA::post);

    FPGA::isRunning = true;
}


void Osci::StartP2P(bool)
{
    FPGA::forcedStart = false;
    addrRead = 0xffff;

    //if(button || TrigStartMode::IsWait())
    //{
    Roller::Prepare();
    FPGA::GiveStart(FPGA::pred, FPGA::post);
    //}

    FPGA::isRunning = true;
}


void Osci::StopNormal()
{
    FPGA::isRunning = false;
}


void Osci::StartWaitP2P(bool)
{

}


void Osci::StopWaitP2P()
{

}


void Osci::StartSingleP2P(bool)
{
}


void Osci::StopSingleP2P()
{

}


void Osci::InputController::Init()
{
    HAL_PIO::Init(PIN_SPI3_SCK, HMode::Output_PP, HPull::Down);     // SPI3_SCK
    HAL_PIO::Init(PIN_SPI3_DAT, HMode::Output_PP, HPull::Down);     // SPI3_DAT
    HAL_PIO::Init(PIN_SPI3_CS1, HMode::Output_PP, HPull::Down);     // SPI3_CS1
    HAL_PIO::Init(PIN_SPI3_CS2, HMode::Output_PP, HPull::Down);     // SPI3_CS2
    HAL_PIO::Init(PIN_A1, HMode::Output_PP, HPull::Down);           // A1
    HAL_PIO::Init(PIN_A2, HMode::Output_PP, HPull::Down);           // A2
    HAL_PIO::Init(PIN_A3, HMode::Output_PP, HPull::Down);           // A3
    HAL_PIO::Init(PIN_A4, HMode::Output_PP, HPull::Down);           // A4
    HAL_PIO::Init(PIN_LF1, HMode::Output_PP, HPull::Down);          // LF1 Ограничение полосы
    HAL_PIO::Init(PIN_LF2, HMode::Output_PP, HPull::Down);          // LF2 Ограничение полосы
    HAL_PIO::Init(PIN_LF3, HMode::Output_PP, HPull::Down);          // A1S
    HAL_PIO::Init(PIN_A0S, HMode::Output_PP, HPull::Down);          // A0S
    HAL_PIO::Init(PIN_LFS, HMode::Output_PP, HPull::Down);          // LFS
}

void Osci::InputController::Write(HPort::E portCS, uint16 pinCS, uint16 value)
{
    HAL_PIO::Reset(portCS, pinCS);

    if(portCS == PORT_SPI3_CS1)
    {
        for(int i = 15; i >= 0; --i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }
    else if(portCS == PORT_SPI3_CS2)
    {
        for(int i = 0; i < 16; ++i)
        {
            HAL_PIO::Write(PIN_SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            HAL_PIO::Set(PIN_SPI3_SCK);
            HAL_PIO::Reset(PIN_SPI3_SCK);
        }
    }
    else
    {
        // нет действий
    }

    HAL_PIO::Set(portCS, pinCS);
}


void Osci::ClearDataRand()
{
    DataSettings *ds = RAM::Get();
    ds->Fill();

    std::memset(ds->Data(Chan::A), VALUE::NONE, ds->PointsInChannel());
    std::memset(ds->Data(Chan::B), VALUE::NONE, ds->PointsInChannel());
}


void Osci::ReadData()
{
    Osci::Stop();

    DataSettings *ds = RAM::PrepareForNewData();

    if(OSCI_IN_MODE_RANDOMIZER)
    {
        Randomizer::MoveReadedData(ds);
    }

    if(ReadDataChannel(Chan::A, ds->dataA))
    {
        if(ReadDataChannel(Chan::B, ds->dataB))
        {
        }
        else
        {
            return;
        }
    }

    if(ENumAverage() != ENumAverage::_1)               // Если включено усреднение
    {
        DataSettings *last = RAM::Get(0);
        DataSettings *prev = RAM::Get(1);

        if(prev && last)
        {
            if(last->IsEquals(*prev))
            {
                if(ENABLED_A(last))
                {
                    AveragerOsci::Process(Chan::A, last->dataA, last->BytesInChannel());
                }
                if(ENABLED_B(last))
                {
                    AveragerOsci::Process(Chan::B, last->dataB, last->BytesInChannel());
                }
            }
        }
    }
}


void Randomizer::MoveReadedDataChannel(DataSettings *ds, Chan::E ch)
{
    if(ENABLED(ds, ch))
    {
        uint numPoints = ds->BytesInChannel();

        uint8 *in = IntRAM::DataRand(ch);
        uint8 *out = ds->Data(ch);

        for(uint i = 0; i < numPoints; i++)
        {
            if(in[i] != VALUE::NONE)
            {
                out[i] = in[i];
                in[i] = VALUE::NONE;
            }
        }
    }
}


void Randomizer::MoveReadedData(DataSettings *ds)
{
    MoveReadedDataChannel(ds, Chan::A);
    MoveReadedDataChannel(ds, Chan::B);
}


void Randomizer::Read()
{
    FPGA::ReadFlag();

    Osci::ProcessFlagPred();

    if(FPGA::flag.DataReady())
    {
        Timer::PauseOnTicks(5 * 90 * 20);

        Osci::ReadDataChannel(Chan::A, IntRAM::DataRand(Chan::A));

        Osci::ReadDataChannel(Chan::B, IntRAM::DataRand(Chan::B));

        Osci::funcStart(false);
    }
}
