#include "stdafx.h"
#ifndef WIN32
#include <stm32f4xx.h>
#include "defines.h"
#include "device.h"
#include "log.h"
#include "FPGA.h"
#include "FPGA_HAL.h"
#include "FPGA_Settings.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Timer.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Data/DataBuffer.h"
#include "Data/DataStorage.h"
#include <cstring>
#include <stdlib.h>
#endif


using FPGA::SET::TBase;
using FPGA::SET::Trig;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NULL_TSHIFT 1000000


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static ADC_HandleTypeDef handleADC;
uint16 FPGA::valueADC = 0;

uint16 FPGA::post = (uint16)~(512);
uint16 FPGA::pred = (uint16)~(512);
uint16 FPGA::flag = 0;

struct PinStruct
{
    GPIO_TypeDef   *gpioTD;
    uint            pin;
};

static PinStruct pins[Pin::Number] =
{
    {GPIOC, GPIO_PIN_10},   // SPI3_SCK
    {GPIOC, GPIO_PIN_12},   // SPI3_DAT
    {GPIOD, GPIO_PIN_3},    // SPI3_CS1
    {GPIOG, GPIO_PIN_13},   // SPI3_CS2
    {GPIOD, GPIO_PIN_10},   // A1
    {GPIOD, GPIO_PIN_11},   // A2
    {GPIOD, GPIO_PIN_12},   // A3
    {GPIOD, GPIO_PIN_13},   // A4
    {GPIOG, GPIO_PIN_2},    // LF1
    {GPIOG, GPIO_PIN_3},    // LF2
    {GPIOG, GPIO_PIN_4},    // A1S
    {GPIOG, GPIO_PIN_5},    // A0S
    {GPIOG, GPIO_PIN_6}     // LFS
};


volatile static int numberMeasuresForGates = 1000;

static uint8 dataRand[Chan::Number][FPGA_MAX_NUM_POINTS];    ///< Здесь будут данные рандомизатора
              //  2нс 5нс 10нс 20нс 50нс
const int Kr[] = {50, 20, 10,  5,   2};
/// Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
static uint16 addrRead = 0xffff;

bool          FPGA::isRunning = false;
bool          FPGA::givingStart = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;
FPGA::State   FPGA::state = {false, StateWorkFPGA_Stop, StateCalibration_None};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::GiveStart()
{
    uint8 value = (uint8)TRIG_POLARITY;
    value = (uint8)((value + 1) % 2);
    FSMC::WriteToFPGA8(WR_TRIG, value++);
    FSMC::WriteToFPGA8(WR_TRIG, (uint8)(value % 2));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Stop(bool)
{
    isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Update() // -V2506
{
    if (!isRunning)
    {
        return;
    };

    int number = (IN_RANDOMIZE_MODE) ? Kr[SET_TBASE] : 1;

    for (int i = 0; i < number; i++)
    {
        ReadFlag();

        if (GetFlag::PRED() && !givingStart)
        {
            if (START_MODE_IS_AUTO && GetFlag::HOLD_OFF())
            {
                GiveStart();
                givingStart = true;
            }
            if(!GetFlag::TRIG_READY())
            {
                Trig::pulse = false;
            }
        }

        if (GetFlag::DATA_READY())
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
void FPGA::ReadFlag()
{
    flag = (uint16)(FSMC::ReadFromFPGA(RD_FLAG_LO) | (FSMC::ReadFromFPGA(RD_FLAG_HI) << 8));

    if(GetFlag::TRIG_READY() && !givingStart)
    {
        Trig::pulse = true;
    }

    FrequencyCounter::Update();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FPGA::ReadLastRecord()
{
    return (uint16)(FSMC::ReadFromFPGA(RD_LAST_RECORD_LO) + ((FSMC::ReadFromFPGA(RD_LAST_RECORD_HI)) << 8));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::OnPressStart()
{
    isRunning = !isRunning;
    if (isRunning)
    {
        Start();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Start()
{
    //AD9286::Tune();

    givingStart = false;
    addrRead = 0xffff;

    FSMC::WriteToFPGA16(WR_PRED_LO, pred);
    FSMC::WriteToFPGA16(WR_POST_LO, post);
    FSMC::WriteToFPGA8(WR_START, 0xff);

    timeStart = TIME_MS;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ForTester::Start() // -V2506
{
    // У нас двенадцать делений. На двенадцать делений должно приходиться не менее 2.5 мс
    // 2.5мс / 12дел = 0.2 мс/дел = 10мкс/тчк

    SET_TBASE = TBase::_500us;

    SET::TBase::Load();
    
    FSMC::WriteToFPGA16(WR_POST_LO, (uint16)(~(400 + 1)));
    FSMC::WriteToFPGA16(WR_PRED_LO, (uint16)(~(0+ 3)));
    FSMC::WriteToFPGA8(WR_START, 0xff);

    uint start = TIME_US;
    flag = 0;
    while (!GetFlag::PRED())
    {
        ReadFlag();
        if(TIME_US - start > 1000) /// \todo Временная затычка. Надо сделать так, чтобы такие ситуации были исключены. Сбои происходят, во время
        {                               /// нажатия кнопок
            return;
        }
    }

    GiveStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::ForTester::Read(uint8 *dataA, uint8 *dataB) // -V2506
{
    uint start = TIME_MS;
    flag = 0;
    while (!GetFlag::DATA_READY())    // Ждём флага готовности данных
    {
        ReadFlag();

        if(TIME_MS - start > 20)        /// \todo Временная затычка. Надо сделать так, чтобы такие ситуации были исключены. Сбои происходят, во время
        {                               /// нажатия кнопок
            return false;
        }
    }

    uint16 aRead = (uint16)(ReadLastRecord() - TESTER_NUM_POINTS);

    FSMC::WriteToFPGA16(WR_PRED_LO, aRead);             // Указываем адрес, с которого будем читать данные
    FSMC::WriteToFPGA8(WR_START_ADDR, 0xff);            // И даём команду ПЛИС, чтобы чтение начиналось с него

    uint8 *addrA = RD_DATA_A; // -V566
    addrA++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные первого канала
    {
        *dataA++ = *addrA;
    }

    FSMC::WriteToFPGA16(WR_PRED_LO, aRead);             // Указываем адрес, с котонрого будем читать данные
    FSMC::WriteToFPGA8(WR_START_ADDR, 0xff);            // И даём команду ПЛИС, чтобы чтение начиналось с него

    uint8 *addrB = RD_DATA_B; // -V566
    addrB++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные второго канала
    {
        *dataB++ = *addrB;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenl(Chan::E ch, uint8 data[FPGA_MAX_NUM_POINTS])
{
    uint numPoints = NumPoints();

    if (addrRead == 0xffff)
    {
        addrRead = (uint16)(ReadLastRecord() - (int)numPoints);
    }
    
    FSMC::WriteToFPGA16(WR_PRED_LO, (uint16)(addrRead));
    FSMC::WriteToFPGA8(WR_START_ADDR, 0xff);


    uint8 *addr0 = Chan(ch).IsA() ? RD_DATA_A : RD_DATA_B;  // -V566
    uint8 *addr1 = addr0 + 1;

    if (IN_RANDOMIZE_MODE)
    {
        ReadDataChanenlRand(ch, addr1, data);
    }
    else
    {
        uint8 *p = data;

        *p = *addr0;    // Первая точка почему-то неправильная читается. Просто откидываем её.
        *p = *addr1;    // -V519

        if(SET_PEAKDET_EN)
        {
            for(uint i = 0; i < numPoints / 2U; i++)
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
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenlRand(Chan::E ch, const uint8 *address, uint8 *data) // -V2506
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

    while(index < 0)
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
int FPGA::CalculateShift() // -V2506
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
bool FPGA::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax) // -V2506
{
    static float minGate = 0.0F;
    static float maxGate = 0.0F;

    if (rand < 500 || rand > 4000)
    {
        ///LOG_WRITE("ОШИБКА!!! считано %d", rand);
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

    if(rand < *eMin || rand > *eMax)
    {
        return false;
    }

    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::GPIO_Init()
{
    GPIO_InitTypeDef isGPIO =
    {
        0,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };

    for (int i = 0; i < Pin::Number; i++)
    {
        isGPIO.Pin = GetPin((Pin::E)i);
        HAL_GPIO_Init(GetPort((Pin::E)i), &isGPIO);
    }

    for (int i = 0; i < Pin::Number; i++)
    {
        gpio.SetOutputPP_PullDown(GetPort((Pin::E)i), (uint)Math::LowSignedBit(GetPin((Pin::E)i)));
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::GetPin(Pin::E pin)
{
    return pins[pin].pin;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
GPIO_TypeDef *FPGA::GetPort(Pin::E pin)
{
    return pins[pin].gpioTD;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteRegisters(Pin::E cs, uint16 value)
{
    ResetPin(cs);

    if (cs == Pin::SPI3_CS1)
    {
        for (int i = 15; i >= 0; --i)
        {
            WritePin(Pin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(Pin::SPI3_SCK);
            ResetPin(Pin::SPI3_SCK);
        }
    }
    else if (cs == Pin::SPI3_CS2)
    {
        for (int i = 0; i < 16; ++i)
        {
            WritePin(Pin::SPI3_DAT, _GET_BIT(value, i));
            PAUSE_ON_TICKS(100);
            SetPin(Pin::SPI3_SCK);
            ResetPin(Pin::SPI3_SCK);
        }
    }
    else
    {
        // нет действий
    }

    SetPin(cs);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WritePin(Pin::E pin, int enable)
{
    if (enable)
    {
        GetPort(pin)->BSRR = GetPin(pin);
    }
    else
    {
        GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetPin(Pin::E pin)
{
    GetPort(pin)->BSRR = GetPin(pin);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ResetPin(Pin::E pin)
{
    GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SET::SetModeCouple(Chan::E ch, ModeCouple::E modeCoupe)
{
    SET_COUPLE(ch) = modeCoupe;
    SET::Range::LoadBoth();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetBandwidth(Chan::E ch)
{
    static const Pin::E pinsLF[2] = {Pin::LF1, Pin::LF2};
    WritePin(pinsLF[ch], SET_BANDWIDTH(ch) == Bandwidth::_20MHz);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetDeltaTShift(int16 /*shift*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetNumberMeasuresForGates(int /*number*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetENumSignalsInSec(int /*numSigInSec*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Reset()
{
    DataStorage::Init(Device::CurrentMode());

    SET::TShift::Load();

    HAL::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::EnableRecorderMode(bool)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::FindAndSetTrigLevel()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::DATA_READY()
{
    return _GET_BIT(flag, Flag::_DATA_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::TRIG_READY()
{
    return _GET_BIT(flag, Flag::_TRIG_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::HOLD_OFF()
{
    return _GET_BIT(flag, Flag::_HOLD_OFF_FLAG) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PRED()
{
    return _GET_BIT(flag, Flag::_PRED) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::FREQ_READY()
{
    return _GET_BIT(flag, Flag::_FREQ_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PERIOD_READY()
{
    return _GET_BIT(flag, Flag::_PERIOD_READY) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::FREQ_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_FREQ_OVERFLOW) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PERIOD_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_PERIOD_OVERFLOW) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::FREQ_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_FREQ_IN_PROCESS) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PERIOD_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_PERIOD_IN_PROCESS) == 1;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::NumPoints()
{
    return (uint)(1 << (FPGA_ENUM_POINTS + 9));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::DoCalibration()
{
    state.needCalibration = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetValueADC(uint16 value)
{
    valueADC = value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::IsRunning()
{
    return isRunning;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
ADC_HandleTypeDef *FPGA::HandleADC()
{
    return &handleADC;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ClearDataRand()
{
    std::memset(dataRand, 0, FPGA_MAX_NUM_POINTS * 2 * sizeof(uint8));  // -V512
}
