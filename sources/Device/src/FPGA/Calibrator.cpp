#include "defines.h"
#include "FPGA/Calibrator.h"
#include "FPGA/FPGA.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"
#include <cmath>
#include <cstring>


// Откалибровать канал Ch
static bool CalibrateChannel(Chan::E ch);

// Балансировать канал на одном диапазоне
static void BalanceRange(Chan::E ch, Range::E range);

// "Растянуть" канал
static bool StretchChannel(Chan::E ch);

// Найти коэффициент растяжки канала
static float FindStretchChannel(Chan::E ch);

static void NormalExit();

static void BadExit();


void Calibrator::Calibrate()
{
    ExtraStretch::SetTypeReal();
    ExtraShift::SetTypeReal();

    if (!CalibrateChannel(Chan::A))
    {
        Display::Message::ShowAndWaitKey("Калибровка канала 1 не прошла", true);

        return BadExit();
    }
    else if (!CalibrateChannel(Chan::B))
    {
        Display::Message::ShowAndWaitKey("Калибровка канала 2 не прошла", true);

        return BadExit();
    }

    Display::Message::ShowAndWaitKey("Калибровка успешно завершена", true);

    NormalExit();
}


static bool CalibrateChannel(Chan::E ch)
{
    Display::Message::ShowAndWaitKey(ch == Chan::A ?  "Подключите встроенный калибратор ко входу 1 и нажмите любую кнопку" :
                                                      "Подключите встроенный калибратор ко входу 2 и нажмите любую кнопку", true);

    Display::Message::Show(ch == Chan::A ? "Калибрую канал 1" : "Калибрую канал 2", true);

    bool result = Calibrator::BalanceChannel(ch, false) && StretchChannel(ch);

    Display::Message::Hide();

    return result;
}


bool Calibrator::BalanceChannel(Chan::E ch, bool showHint)
{
    SettingsNRST old = setNRST;

    ExtraShift::SetTypeDisabled();

    static const pString messages[Chan::Count] =
    {
        "Балансирую канал 1",
        "Балансирую канал 2"
    };

    if (showHint)
    {
        Display::Message::Show(messages[ch], true);
    }

    ModeCouple(ch).SetGND();

    TBase::Set100ms();

    for (int range = 0; range < Range::Count; range++)
    {
        BalanceRange(ch, static_cast<Range::E>(range));
    }

    std::memcpy(&old.exShift.value[ch][0], &setNRST.exShift.value[ch][0], sizeof(setNRST.exShift.value[ch][0]) * Range::Count);

    setNRST = old;

    Osci::Init();

    if (showHint)
    {
        Display::Message::Hide();
    }

    return true;
}


static void BalanceRange(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Range(ch).Set(range);

    RShift(ch).Set(0);

    Osci::Start(false);

    float sum = 0;

    int numPoints = 0;

    uint8 *addr = ((ch == Chan::A) ? RD::DATA_A : RD::DATA_B) + 1;

    while (numPoints < 100)
    {
        if (!HAL_BUS::Panel::InInteraction())
        {
            if (HAL_PIO::Read(PIN_P2P))
            {
                HAL_BUS::FPGA::SetAddrData(addr);
                sum += HAL_BUS::FPGA::ReadA0();
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0.0F)
    {
        ExtraShift::SetValue(ch, range, static_cast<int8>(delta * 200.0F / 125.0F + 0.5F));
    }
    else
    {
        ExtraShift::SetValue(ch, range, static_cast<int8>(delta * 200.0F / 125.0F - 0.5F));
    }
}


static bool StretchChannel(Chan::E ch)
{
    SettingsNRST old = setNRST;

    ExtraShift::SetTypeReal();
    ExtraStretch::SetTypeDisabled();

    ModeCouple(ch).SetAC();
    RShift(ch).Set(0);
    Range(ch).Set500mV();
    TBase::Set200us();
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigLevel(ch).Set(0);


    float k = FindStretchChannel(ch);

    if (k > 0.0F)
    {
        old.exStretch.value[ch] = k;
    }

    setNRST = old;

    Osci::Init();

    return (k > 0.0F);
}


static float FindStretchChannel(Chan::E ch)
{
#define NUM_POINTS 300
    
    Timer::PauseOnTime(500);

    FPGA::GiveStart(static_cast<uint16>(~(1)), static_cast<uint16>(~(NUM_POINTS + 100)));
    
    do 
    {
        FPGA::ReadFlag();

    } while (!FPGA::flag.DataReady());

    uint16 addrRead = static_cast<uint16>(Osci::ReadLastRecord(ch) - NUM_POINTS - 50);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, addrRead);
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);

    uint8 *a0 = Chan(ch).IsA() ? RD::DATA_A : RD::DATA_B;
    uint8 *a1 = a0 + 1;

    HAL_BUS::FPGA::SetAddrData(a0, a1);

    uint8 data = *a0;
    data = *a1;

    int sumMIN = 0;
    int sumMAX = 0;
    int numMIN = 0;
    int numMAX = 0;

    for(uint i = 0; i < NUM_POINTS; i++)
    {
        data = *a1;

        if(data < VALUE::MIN + 64)
        {
            sumMIN += data;
            numMIN++;
        }
        else if(data > VALUE::MAX - 64)
        {
            sumMAX += data;
            numMAX++;
        }
        else
        {
            return -1.0F;
        }
    }

    float patternDelta = (VALUE::MAX - VALUE::MIN) / 10.0F * 8.0F;    // Образцоввая разница между минимальным и максимальным значениями - ровно на 8 клеток из десяти

    float min = static_cast<float>(sumMIN) / numMIN;
    float max = static_cast<float>(sumMAX) / numMAX;

    return patternDelta / (max - min);
}


static void NormalExit()
{
    ExtraStretch::SetTypeReal();
    ExtraShift::SetTypeReal();
}


static void BadExit()
{
    ExtraStretch::SetTypeDisabled();
    ExtraShift::SetTypeDisabled();
}
