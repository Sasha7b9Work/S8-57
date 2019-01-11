#include "stdafx.h"
#ifndef WIN32
#include <stm32f4xx.h>
#include "defines.h"
#include "device.h"
#include "log.h"
#include "FPGA.h"
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


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NULL_TSHIFT 1000000


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
HardwareFPGA FPGA::hardware;
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

    hardware.LoadTBase();
    
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
void FPGA::ChangeRange(Chan::E ch, int delta)
{
    if (delta > 0)
    {
        Math::LimitationIncrease<uint8>((uint8 *)(&SET_RANGE(ch)), (uint8)(Range::Number - 1)); // -V206
    }
    else
    {
        Math::LimitationDecrease<uint8>((uint8 *)(&SET_RANGE(ch)), 0);  // -V206
    }
    hardware.LoadRanges();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TBaseChange(int delta) // -V2506
{
    if (delta > 0)
    {
        Math::LimitationIncrease<uint8>((uint8 *)(&SET_TBASE), (uint8)(TBase::Number - 1)); // -V206
    }
    else
    {
        if (SET_PEAKDET_EN &&                               // Если вклюён режим пикового детектора
            SET_TBASE == TBase::MIN_PEAK_DET)               // и установлен масштаб по времени, соответствующий минмальному в режиме пикового детектора :
        {
            Display::ShowWarning(Warning::EnabledPeakDet);  // выводим сообщение об этом
            return;                                         // и выходим
        }

        Math::LimitationDecrease<uint8>((uint8 *)(&SET_TBASE), 0); // -V206
    }

    hardware.LoadTBase();
    Start();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::RShiftChange(Chan::E ch, int delta)
{
    Math::AdditionThisLimitation<uint16>(&SET_RSHIFT(ch), STEP_RSHIFT * delta, RShift::MIN, RShift::MAX);

    HardwareFPGA::LoadRShift(ch);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TrigLevChange(int delta)
{
    Math::AdditionThisLimitation<uint16>(&SET_TRIGLEV_SOURCE, STEP_TRIGLEV * delta, Trig::MIN, Trig::MAX);

    HardwareFPGA::LoadTrigLev();

    Trig::NeedForDraw(2000);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TShiftChange(int delta)
{
    SetTShift(SET_TSHIFT + delta);
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

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 HardwareFPGA::ValueForRange(Chan::E ch) // -V2506
{
    static const uint8 datas[ModeCouple::Size] =
    {
        BIN_U8(01000001),  // -V2501  // DC
        BIN_U8(10000001),  // -V2501  // AC
        BIN_U8(00000010)   // -V2501  // GND
    };

    if (SET_COUPLE(ch) == ModeCouple::GND && Device::CurrentMode() == Device::Mode::Osci)
    {
        return datas[ModeCouple::GND];
    }

    DEF_STRUCT(StructRange, uint16) values[Range::Number][2] =
    {   //             A                    B
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 2mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 5mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 10mV
        { BIN_U8(00100100), BIN_U8(00100100) }, // -V2501  // 20mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 50mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 100mV
        { BIN_U8(00010100), BIN_U8(00010100) }, // -V2501  // 200mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 500mV
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 1V
        { BIN_U8(00101000), BIN_U8(00101000) }, // -V2501  // 2V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 5V
        { BIN_U8(00011000), BIN_U8(00011000) }, // -V2501  // 10V
        { BIN_U8(00011000), BIN_U8(00011000) }  // -V2501  // 20V
    };

    return (uint8)(values[SET_RANGE(ch)][ch].val | datas[SET_COUPLE(ch)]);
}

#ifdef WIN32
#pragma warning(pop)
#endif

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
void HardwareFPGA::LoadTrigLev()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::MAX + Trig::MIN) - SET_TRIGLEV_SOURCE);

    FPGA::WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));
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
void HardwareFPGA::LoadTShift()
{
    FPGA::post = (uint16)(SET_TSHIFT - TShift::Min());
    int Pred = (int)FPGA::NumPoints() - (int)FPGA::post;

    if(Pred < 0)
    {
        Pred = 0;
    }
    FPGA::pred = (uint16)Pred;

    FPGA::post = (uint16)(~(FPGA::post + 1));
    FPGA::pred = (uint16)(~(FPGA::pred + 3));

    FSMC::WriteToFPGA16(WR_PRED_LO, FPGA::post);
    FSMC::WriteToFPGA16(WR_POST_LO, FPGA::pred);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetRShift(Chan::E ch, uint16 rShift)
{
    Math::Limitation<uint16>(&rShift, RShift::MIN, RShift::MAX);
    SET_RSHIFT(ch) = rShift;
    HardwareFPGA::LoadRShift(ch);
}

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HardwareFPGA::LoadTBase()
{
    static const uint8 values[TBase::Number] =
    {
        BIN_U8(00000000),  // -V2501  // 2ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 5ns     1       200MHz
        BIN_U8(00000000),  // -V2501  // 10ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 20ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 50ns    1       200MHz
        BIN_U8(00000000),  // -V2501  // 100ns   1       200MHz
        BIN_U8(00100000),  // -V2501  // 200ns   2       100MHz
        BIN_U8(00100001),  // -V2501  // 500ns   5       40MHz
        BIN_U8(00100010),  // -V2501  // 1us     10      20MHz
        BIN_U8(00100011),  // -V2501  // 2us     20      10MHz
        BIN_U8(01000101),  // -V2501  // 5us     50      4MHz
        BIN_U8(01000110),  // -V2501  // 10us    100     2MHz
        BIN_U8(01000111),  // -V2501  // 20us    200     1MHz
        BIN_U8(01001001),  // -V2501  // 50us    500     400kHz
        BIN_U8(01001010),  // -V2501  // 100us   1k      200kHz
        BIN_U8(01001011),  // -V2501  // 200us   2k      100kHz
        BIN_U8(01001101),  // -V2501  // 500us   5k      40kHz
        BIN_U8(01001110),  // -V2501  // 1ms     10k     20kHz
        BIN_U8(01001111),  // -V2501  // 2ms     20k     10kHz
        BIN_U8(01010001),  // -V2501  // 5ms     50k     4kHz
        BIN_U8(01010010),  // -V2501  // 10ms    100k    2kHz
        BIN_U8(01010011),  // -V2501  // 20ms    200k    1kHz
        BIN_U8(01010101),  // -V2501  // 50ms    500k    400Hz
        BIN_U8(01010110),  // -V2501  // 100ms   1M      200Hz
        BIN_U8(01010111),  // -V2501  // 200ms   2M      100Hz
        BIN_U8(01011001),  // -V2501  // 500ms   5M      40Hz
        BIN_U8(01011010),  // -V2501  // 1s      10M     20Hz
        BIN_U8(01011011),  // -V2501  // 2s      20M     10Hz
        BIN_U8(01011101),  // -V2501  // 5s      50M     4Hz
        BIN_U8(01011110)   // -V2501  // 10s     100M    2Hz
    };

    std::memset(dataRand, 0, FPGA_MAX_NUM_POINTS * 2 * sizeof(uint8));  // -V512

    FSMC::WriteToFPGA8(WR_TBASE, values[SET_TBASE]);

    LoadTShift();
}

#ifdef _WIN32
#pragma warning(pop)
#endif

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------

/*
void FPGA::LoadTrigMode()
{

}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigInput()
{
    hardware.LoadTrigSourceInput();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HardwareFPGA::LoadTrigSource()
{
    LoadTrigSourceInput();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigPolarity()
{
    GiveStart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetTrigLev(Trig::Source::E /*ch*/, uint16 /*trigLev*/)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetTShift(int tShift)
{
    SET_TSHIFT.Set(tShift);

    hardware.LoadTShift();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeCouple(Chan::E ch, ModeCouple::E modeCoupe)
{
    SET_COUPLE(ch) = modeCoupe;
    hardware.LoadRanges();
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
    DataStorage::Init();

    hardware.LoadTShift();

    LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadRegUPR()
{
    uint8 data = 0;

    if (SET_PEAKDET_EN)
    {
        data |= 1 << BIT_UPR_PEAK;
    }

    static const uint8 mask[3] =
    {
        (1 << BIT_UPR_CALIBR_AC_DC),
        (1 << BIT_UPR_CALIBR_ZERO),
        (0)
    };

    data |= mask[CALIBRATOR_MODE];

    FSMC::WriteToFPGA8(WR_UPR, data);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void HardwareFPGA::LoadCalibratorMode()
{
    FPGA::LoadRegUPR();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::EnableRecorderMode(bool)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
void FPGA::SetTBase(TBase tBase)
{
}
*/

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::FindAndSetTrigLevel()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/*
StateWorkFPGA FPGA::GetStateWork()
{
    return fpgaStateWork;
}
*/

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
void HardwareFPGA::LoadTrigSourceInput()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // -V2501      // ПС 
        {BIN_U8(00000011), BIN_U8(00000101)}, // -V2501      // ВЧ
        {BIN_U8(00000000), BIN_U8(00000110)}  // -V2501      // НЧ
    };

    FPGA::WritePin(Pin::A1S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 2));
    FPGA::WritePin(Pin::A0S, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 1));
    FPGA::WritePin(Pin::LFS, _GET_BIT(datas[TRIG_INPUT][TRIG_SOURCE], 0));
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
bool FPGA::InStateStop()
{
    return false;
}
