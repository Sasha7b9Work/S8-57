#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA.h"
#include "AD9286.h"
#include "Hardware/CPU.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Timer.h"
#include "Utils/Math.h"
#include "Settings/Settings.h"
#include "Data/Buffer.h"
#include "Data/Storage.h"
#include "TrigLev.h"
#include <string.h>
#include <stdlib.h>
#endif


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define NULL_TSHIFT 1000000


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
ADC_HandleTypeDef FPGA::handleADC;
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


//static bool isRunning = false;

volatile static int numberMeasuresForGates = 1000;

static uint8 dataRand[Chan::Number][FPGA_MAX_NUM_POINTS];    ///< Здесь будут данные рандомизатора
              //  2нс 5нс 10нс 20нс 50нс
const int Kr[] = {50, 20, 10,  5,   2};
/// Здесь хранится адрес, начиная с которого будем читать данные по каналам. Если addrRead == 0xffff, то адрес вначале нужно считать
static uint16 addrRead = 0xffff;

static uint8 ValueForRange(Chan ch);

bool          FPGA::isRunning = false;
bool          FPGA::givingStart = false;
uint          FPGA::timeStart = 0;
StateWorkFPGA FPGA::fpgaStateWork = StateWorkFPGA_Stop;
FPGA::State   FPGA::state = {false, StateWorkFPGA_Stop, StateCalibration_None};


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FPGA::GiveStart()
{
    uint8 value = (uint8)TRIG_POLARITY;
    value = (uint8)((value + 1) % 2);
    FSMC::WriteToFPGA8(WR_TRIG, value++);
    FSMC::WriteToFPGA8(WR_TRIG, (uint8)(value % 2));
    Trig::timeSwitchingTrig = TIME_MS;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Stop(bool)
{
    isRunning = false;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Update()
{
    Buffer::Update();

    if (!isRunning)
    {
        return;
    };

    ReadFlag();

    if (GetFlag::PRED() && !givingStart)
    {
        if (START_MODE_IS_AUTO)
        {
            GiveStart();
            givingStart = true;
        }
    }

    if (GetFlag::DATA_READY())
    {
        ReadData();
        if (START_MODE_IS_SINGLE)
        {
            OnPressStart();
        }
        else
        {
            Start();
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadFlag()
{
    flag = (uint16)(FSMC::ReadFromFPGA(RD_FLAG_LO) | (FSMC::ReadFromFPGA(RD_FLAG_HI) << 8));

    Trig::pulse = GetFlag::TRIG_READY() && timeStart > Trig::timeSwitchingTrig;

    FrequencyCounter::Update();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint16 FPGA::ReadLastRecord()
{
    return (uint16)(FSMC::ReadFromFPGA(RD_LAST_RECORD_LO) + ((FSMC::ReadFromFPGA(RD_LAST_RECORD_HI)) << 8));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::OnPressStart()
{
    isRunning = !isRunning;
    if (isRunning)
    {
        Start();
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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


//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::StartForTester(int)
{
    // У нас двенадцать делений. На двенадцать делений должно приходиться не менее 2.5 мс
    // 2.5мс / 12дел = 0.2 мс/дел = 10мкс/тчк

    SET_TBASE = TBase::_500us;

    LoadTBase();
    
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::ReadForTester(uint8 *dataA, uint8 *dataB)
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

    uint8 *addrA = RD_DATA_A;
    addrA++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные первого канала
    {
        *dataA++ = *addrA;
    }

    FSMC::WriteToFPGA16(WR_PRED_LO, aRead);             // Указываем адрес, с котонрого будем читать данные
    FSMC::WriteToFPGA8(WR_START_ADDR, 0xff);            // И даём команду ПЛИС, чтобы чтение начиналось с него

    uint8 *addrB = RD_DATA_B;
    addrB++;
    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные второго канала
    {
        *dataB++ = *addrB;
    }

    return true;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenl(Chan ch, uint8 data[FPGA_MAX_NUM_POINTS])
{
    if (addrRead == 0xffff)
    {
        addrRead = (uint16)(ReadLastRecord() - FPGA_NUM_POINTS);
    }
    
    FSMC::WriteToFPGA16(WR_PRED_LO, (uint16)(addrRead));
    FSMC::WriteToFPGA8(WR_START_ADDR, 0xff);


    uint8 *addr0 = ch.IsA() ? RD_DATA_A : RD_DATA_B;
    uint8 *addr1 = addr0 + 1;

    if (IN_RANDOMIZE_MODE)
    {
        ReadDataChanenlRand(ch, addr1, data);
    }
    else
    {
        uint8 *p = data;

        *p = *addr0;    // Первая точка почему-то неправильная читается. Просто откидываем её.
        *p = *addr1;

        if(SET_PEAKDET_EN)
        {
            for(int i = 0; i < FPGA_NUM_POINTS / 2; i++)
            {
                *p++ = *addr0;
                *p++ = *addr1;

            }
        }
        else
        {
            for (int i = 0; i < FPGA_NUM_POINTS / 4; ++i)
            {
                *p++ = *addr1;
                *p++ = *addr1;
                *p++ = *addr1;
                *p++ = *addr1;
            }
        }
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadDataChanenlRand(Chan ch, uint8 *address, uint8 *data)
{
    int Tsm = CalculateShift(ch);

    if (Tsm == NULL_TSHIFT)
    {
        return;
    }

    int step = Kr[SET_TBASE];

    int index = Tsm - step;

    uint8 *dataRead = &dataRand[ch][0];
    dataRead = &dataRand[ch][index];

    while(index < 0)
    {
        volatile uint8 d = *address;
        d = d;
        index += step;
        dataRead += step;
    }

    if(ch == Chan::A)
    {
        //LOG_WRITE("%d %d", Tsm, dataRead - data0);
    }

    uint8 *last = &dataRand[ch][FPGA_NUM_POINTS];

    while (dataRead < last)
    {
        *dataRead = *address;
        dataRead += step;
    }

    memcpy(data, &dataRand[ch][0], (uint)FPGA_NUM_POINTS);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int FPGA::CalculateShift(Chan ch)
{
    uint16 min = 0;
    uint16 max = 0;

    if (!CalculateGate(valueADC, &min, &max))
    {
        return NULL_TSHIFT;
    }

    if (IN_RANDOMIZE_MODE)
    {
        float tin = (float)(valueADC - min) / (max - min);
        int retValue = (int)(tin * Kr[SET_TBASE]);

        if(ch == Chan::A)
        {
            //LOG_WRITE("%d %d %d %d", adcValueFPGA, min, max, retValue);
        }

        return retValue;
    }

    return NULL_TSHIFT;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::CalculateGate(uint16 rand, uint16 *eMin, uint16 *eMax)
{
    static float minGate = 0.0f;
    static float maxGate = 0.0f;

    if (rand < 500 || rand > 4000)
    {
        LOG_WRITE("ОШИБКА!!! считано %d", rand);
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

    if (minGate == 0.0f)
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
        minGate = 0.8f * minGate + min * 0.2f;
        maxGate = 0.8f * maxGate + max * 0.2f;

        numElements = 0;
        min = 0xffff;
        max = 0;
    }

    *eMin = (uint16)(minGate);
    //*eMax = (uint16)(maxGate);
    *eMax = (uint16)(maxGate - 50);

    if(rand < *eMin || rand > *eMax)
    {
        return false;
    }

    return retValue;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ReadData()
{
    uint8 *dataA = (uint8 *)malloc((uint)FPGA_NUM_POINTS);
    uint8 *dataB = (uint8 *)malloc((uint)FPGA_NUM_POINTS);

    ReadDataChanenl(Chan::A, dataA);
    ReadDataChanenl(Chan::B, dataB);

    DataSettings ds;
    ds.Fill();

    Storage::AddData(dataA, dataB, ds);
    
    free(dataA);
    
    free(dataB);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::IncreaseRange(Chan ch)
{
    Math::LimitationIncrease<uint8>((uint8 *)(&SET_RANGE(ch)), (uint8)(Range::Number - 1));
    LoadRanges();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::DecreaseRange(Chan ch)
{
    Math::LimitationDecrease<uint8>((uint8 *)(&SET_RANGE(ch)), 0);
    LoadRanges();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::IncreaseTBase()
{
    Math::LimitationIncrease<uint8>((uint8 *)(&SET_TBASE), (uint8)(TBase::Number - 1));
    LoadTBase();
    Start();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::DecreaseTBase()
{
    if(SET_PEAKDET_EN &&                    // Если вклюён режим пикового детектора
       SET_TBASE == TBase::MIN_PEAK_DET)    // и установлен масштаб по времени, соответствующий минмальному в режиме пикового детектора :
    {                                       
        Display::ShowWarning(Warning::EnabledPeakDet);  // выводим сообщение об этом
        return;                                         // и выходим
    }

    Math::LimitationDecrease<uint8>((uint8 *)(&SET_TBASE), 0);
    LoadTBase();
    Start();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::RShiftChange(Chan ch, int delta)
{
    Math::AdditionThisLimitation<uint16>(&SET_RSHIFT(ch), STEP_RSHIFT * delta, RShift::MIN, RShift::MAX);

    LoadRShift(ch);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TrigLevChange(int delta)
{
    Math::AdditionThisLimitation<uint16>(&SET_TRIGLEV_SOURCE, STEP_TRIGLEV * delta, Trig::MIN, Trig::MAX);

    LoadTrigLev();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::TShiftChange(int delta)
{
    SetTShift(SET_TSHIFT + delta);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint FPGA::GetPin(Pin pin)
{
    return pins[pin].pin;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
GPIO_TypeDef *FPGA::GetPort(Pin pin)
{
    return pins[pin].gpioTD;
}

#ifdef WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ValueForRange(Chan ch)
{
    static const uint8 datas[ModeCouple::Size] =
    {
        BIN_U8(01000001),    // DC
        BIN_U8(10000001),    // AC
        BIN_U8(00000010)     // GND
    };

    if (SET_COUPLE(ch) == ModeCouple::GND)
    {
        return datas[ModeCouple::GND];
    }

    static const struct StructRange
    {
        uint16 val;
        StructRange(uint16 v) : val(v) {}
    }
    values[Range::Number][2] =
    {   //             A                    B
        { BIN_U8(00100101), BIN_U8(00100101) },   // 2mV
        { BIN_U8(00100101), BIN_U8(00100101) },   // 5mV
        { BIN_U8(00100101), BIN_U8(00100101) },   // 10mV
        { BIN_U8(00100101), BIN_U8(00100101) },   // 20mV
        { BIN_U8(00010101), BIN_U8(00010101) },   // 50mV
        { BIN_U8(00010101), BIN_U8(00010101) },   // 100mV
        { BIN_U8(00010101), BIN_U8(00010101) },   // 200mV
        { BIN_U8(00101001), BIN_U8(00101001) },   // 500mV
        { BIN_U8(00101001), BIN_U8(00101001) },   // 1V
        { BIN_U8(00101001), BIN_U8(00101001) },   // 2V
        { BIN_U8(00011001), BIN_U8(00011001) },   // 5V
        { BIN_U8(00011001), BIN_U8(00011001) },   // 10V
        { BIN_U8(00011001), BIN_U8(00011001) }    // 20V
    };

    return (uint8)(values[SET_RANGE(ch)][ch].val | datas[SET_COUPLE(ch)]);
}

#ifdef WIN32
#pragma warning(pop)
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadRanges()
{
    uint16 value = (uint16)(ValueForRange(Chan::B) + (ValueForRange(Chan::A) << 8));

    WriteRegisters(Pin::SPI3_CS2, value);

    PAUSE_ON_MS(10);                // Задержка нужна, чтобы импульсные реле успели отработать

    WriteRegisters(Pin::SPI3_CS2, 0);    // Записываем ноль, чтобы реле не потребляли энергии

    static struct StructRange
    {
        uint8 value;
        StructRange(uint8 v) : value(v) {};
    } vals[Range::Number] =
    {
        StructRange(BIN_U8(00000000)),  // 2mV
        StructRange(BIN_U8(00000001)),  // 5mV
        StructRange(BIN_U8(00000010)),  // 10mV
        StructRange(BIN_U8(00000011)),  // 20mV
        StructRange(BIN_U8(00000001)),  // 50mV
        StructRange(BIN_U8(00000010)),  // 100mV
        StructRange(BIN_U8(00000011)),  // 200mV
        StructRange(BIN_U8(00000001)),  // 500mV
        StructRange(BIN_U8(00000010)),  // 1V
        StructRange(BIN_U8(00000011)),  // 2V
        StructRange(BIN_U8(00000001)),  // 5V
        StructRange(BIN_U8(00000010)),  // 10V
        StructRange(BIN_U8(00000011))   // 20V
    };

    uint8 valueA = vals[SET_RANGE_A].value;

    WritePin(Pin::A1, _GET_BIT(valueA, 1));
    WritePin(Pin::A2, _GET_BIT(valueA, 0));

    uint8 valueB = vals[SET_RANGE_B].value;

    WritePin(Pin::A3, _GET_BIT(valueB, 1));
    WritePin(Pin::A4, _GET_BIT(valueB, 0));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WriteRegisters(Pin cs, uint16 value)
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

    SetPin(cs);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadRShift(Chan ch)
{
    LAST_AFFECTED_CH = ch;

    static const uint16 mask[2] = {0x2000, 0x6000};

    WriteRegisters(Pin::SPI3_CS1, (uint16)(mask[ch] | (SET_RSHIFT(ch) << 2)));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigLev()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::MAX + Trig::MIN) - SET_TRIGLEV_SOURCE);

    WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::WritePin(Pin pin, int enable)
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

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetPin(Pin pin)
{
    GetPort(pin)->BSRR = GetPin(pin);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::ResetPin(Pin pin)
{
    GetPort(pin)->BSRR = (uint)GetPin(pin) << 16;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTShift()
{
    post = (uint16)(SET_TSHIFT - TShift::Min());
    int Pred = (int)FPGA_NUM_POINTS - (int)post;

    if(Pred < 0)
    {
        Pred = 0;
    }
    pred = (uint16)Pred;

    //LOG_WRITE("tShift = %d, tShiftMin = %d, post = %D", SET_TSHIFT, TShift::Min(), post);
    //LOG_WRITE("NUM_POINTS = %d, pred = %d", FPGA_NUM_POINTS, pred);

    post = (uint16)(~(post + 1));
    pred = (uint16)(~(pred + 3));

    FSMC::WriteToFPGA16(WR_PRED_LO, post);
    FSMC::WriteToFPGA16(WR_POST_LO, pred);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetRShift(Chan ch, uint16 rShift)
{
    Math::Limitation<uint16>(&rShift, RShift::MIN, RShift::MAX);
    SET_RSHIFT(ch) = rShift;
    LoadRShift(ch);
}

#ifdef _WIN32
#pragma warning(push)
#pragma warning(disable:4310)
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTBase()
{
    static const uint8 values[TBase::Number] =
    {
        BIN_U8(00000000),    // 2ns     1       200MHz
        BIN_U8(00000000),    // 5ns     1       200MHz
        BIN_U8(00000000),    // 10ns    1       200MHz
        BIN_U8(00000000),    // 20ns    1       200MHz
        BIN_U8(00000000),    // 50ns    1       200MHz
        BIN_U8(00000000),    // 100ns   1       200MHz
        BIN_U8(00100000),    // 200ns   2       100MHz
        BIN_U8(00100001),    // 500ns   5       40MHz
        BIN_U8(00100010),    // 1us     10      20MHz
        BIN_U8(00100011),    // 2us     20      10MHz
        BIN_U8(01000101),    // 5us     50      4MHz
        BIN_U8(01000110),    // 10us    100     2MHz
        BIN_U8(01000111),    // 20us    200     1MHz
        BIN_U8(01001001),    // 50us    500     400kHz
        BIN_U8(01001010),    // 100us   1k      200kHz
        BIN_U8(01001011),    // 200us   2k      100kHz
        BIN_U8(01001101),    // 500us   5k      40kHz
        BIN_U8(01001110),    // 1ms     10k     20kHz
        BIN_U8(01001111),    // 2ms     20k     10kHz
        BIN_U8(01010001),    // 5ms     50k     4kHz
        BIN_U8(01010010),    // 10ms    100k    2kHz
        BIN_U8(01010011),    // 20ms    200k    1kHz
        BIN_U8(01010101),    // 50ms    500k    400Hz
        BIN_U8(01010110),    // 100ms   1M      200Hz
        BIN_U8(01010111),    // 200ms   2M      100Hz
        BIN_U8(01011001),    // 500ms   5M      40Hz
        BIN_U8(01011010),    // 1s      10M     20Hz
        BIN_U8(01011011),    // 2s      20M     10Hz
        BIN_U8(01011101),    // 5s      50M     4Hz
        BIN_U8(01011110)     // 10s     100M    2Hz
    };

    memset(&dataRand[0][0], 0, FPGA_MAX_NUM_POINTS * 2);

    FSMC::WriteToFPGA8(WR_TBASE, values[SET_TBASE]);

    LoadTShift();
}

#ifdef _WIN32
#pragma warning(pop)
#endif

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigMode()
{

}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigInput()
{
    LoadTrigSourceInput();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigSource()
{
    LoadTrigSourceInput();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigPolarity()
{
    GiveStart();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::LoadTrigSourceInput()
{
    static const uint8 datas[3][2] =
    {//       A                 B
        {BIN_U8(00000010), BIN_U8(00000100)}, // ПС
        {BIN_U8(00000011), BIN_U8(00000101)}, // ВЧ
        {BIN_U8(00000000), BIN_U8(00000110)}  // НЧ
    };
    
    WritePin(Pin::A1S, _GET_BIT(datas[TRIG_INPUT.value][TRIG_SOURCE.value], 2));
    WritePin(Pin::A0S, _GET_BIT(datas[TRIG_INPUT.value][TRIG_SOURCE.value], 1));
    WritePin(Pin::LFS, _GET_BIT(datas[TRIG_INPUT.value][TRIG_SOURCE.value], 0));
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetTrigLev(Trig::Source ch, uint16 trigLev)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetTShift(int tShift)
{
    SET_TSHIFT.Set(tShift);

    LoadTShift();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetModeCouple(Chan ch, ModeCouple modeCoupe)
{
    SET_COUPLE(ch) = modeCoupe;
    LoadRanges();
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetBandwidth(Chan ch)
{
    static const Pin pinsLF[2] = {Pin::LF1, Pin::LF2};
    WritePin(pinsLF[ch], SET_BANDWIDTH(ch) == Bandwidth::_20MHz);
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetDeltaTShift(int16 shift)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetNumberMeasuresForGates(int number)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetENumSignalsInSec(int numSigInSec)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::Reset()
{
    LoadTShift();

    if(SET_PEAKDET_EN)
    {
        FSMC::WriteToFPGA8(WR_UPR, 1 << BIT_UPR_PEAK);
    }
    else
    {
        FSMC::WriteToFPGA8(WR_UPR, 0);
    }
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetCalibratorMode(CalibratorMode)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::EnableRecorderMode(bool)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::SetTBase(TBase tBase)
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FPGA::FindAndSetTrigLevel()
{
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
StateWorkFPGA FPGA::GetStateWork()
{
    return fpgaStateWork;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::DATA_READY()
{
    return _GET_BIT(flag, Flag::_DATA_READY) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::TRIG_READY()
{
    return _GET_BIT(flag, Flag::_TRIG_READY) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PRED()
{
    return _GET_BIT(flag, Flag::_PRED) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::FREQ_READY()
{
    return _GET_BIT(flag, Flag::_FREQ_READY) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PERIOD_READY()
{
    return _GET_BIT(flag, Flag::_PERIOD_READY) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::FREQ_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_FREQ_OVERFLOW) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PERIOD_OVERFLOW()
{
    return _GET_BIT(flag, Flag::_PERIOD_OVERFLOW) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::FREQ_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_FREQ_IN_PROCESS) == 1;
}

//-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FPGA::GetFlag::PERIOD_IN_PROCESS()
{
    return _GET_BIT(flag, Flag::_PERIOD_IN_PROCESS) == 1;
}
