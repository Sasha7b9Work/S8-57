#include "defines.h"
#include "device.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "Settings/Settings.h"
#include <cstring>

#include "Hardware/HAL/HAL.h"
#include "Hardware/Timer.h"
#include "Display/Display.h"
#include "Utils/Debug.h"
#include "Osci/Osci_Storage.h"


using namespace FPGA;
using namespace ::HAL;
using namespace ::HAL::ADDRESSES::FPGA;
using namespace Osci::Settings;


extern bool givingStart;

extern uint16 addrRead;

extern uint8 dataRand[Chan::Size][FPGA::MAX_NUM_POINTS];


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
                      //  2нс 5нс 10нс 20нс 50нс
const int Kr[] = { 50, 20, 10,  5,   2 };

volatile static int numberMeasuresForGates = 1000;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Читать канал в рандомизаторе с адреса address
void ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data);

static int CalculateShift();

static bool CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax);
/// Чтение точки в поточечном режиме
static void ReadPointP2P();


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
    Settings::TBase::Load();
    Settings::TShift::Load();
    FPGA::Settings::LoadCalibratorMode();
    Settings::LoadHoldfOff();
    FPGA::HAL::Interrupt::P2P::Init(ReadPointP2P);
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

    FPGA::isRunning = true;

    if (InModeP2P())
    {
        FPGA::HAL::Interrupt::P2P::Enable();
        Storage::PrepareNewFrameP2P();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Stop(bool)
{
    FPGA::HAL::Interrupt::P2P::Disable();

    isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Osci::Update()
{
    if (!Device::State::InModeOsci())
    {
        return;
    }

    if (!FPGA::IsRunning())
    {
        return;
    };

    int number = (IN_RANDOMIZE_MODE) ? Kr[SET_TBASE] : 1;

    for (int i = 0; i < number; i++)
    {
        FPGA::HAL::ReadFlag();

        if (FPGA::HAL::GetFlag::PRED() && !givingStart)
        {
            if (START_MODE_IS_AUTO && FPGA::HAL::GetFlag::HOLD_OFF())
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

    uint8 *last = &dataRand[ch][FPGA_NUM_POINTS];

    while (dataRead < last)
    {
        *dataRead = *address;

        dataRead += step;
    }

    std::memcpy(data, &dataRand[ch][0], FPGA_NUM_POINTS);
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

    LOG_WRITE("Считано %d", rand);

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

    LOG_WRITE("Ворота %d %d", *eMin, *eMax);

    if (rand < *eMin || rand > *eMax)
    {
        return false;
    }

    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadPointP2P()
{
    /// Время чтения точки. Если == 0, то нужно брать актуальное при сохранении точки, иначе - из time
    static bool time = 0;

    if(::HAL::FSMC::InterchangeWithPanel())
    {
        time = TIME_MS;
        ::HAL::FSMC::RunFunctionAfterInteractionWitchPanel(ReadPointP2P);
    }
    else
    {
        if (time == 0)
        {
            time = TIME_MS;
        }

        BitSet16 dataA(FSMC::ReadFromFPGA(RD::DATA_A), FSMC::ReadFromFPGA(RD::DATA_A + 1));
        BitSet16 dataB(FSMC::ReadFromFPGA(RD::DATA_B), FSMC::ReadFromFPGA(RD::DATA_B + 1));

        Osci::Storage::GetFrameP2P()->AddPoints(time, dataA, dataB);

        time = 0;
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
