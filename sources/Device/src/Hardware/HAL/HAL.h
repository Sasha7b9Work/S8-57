#pragma once
#include "defines.h"
#include "Data/DataSettings.h"


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0

#define ERROR_HANDLER()                             ::HAL::ErrorHandler(__FILE__, __LINE__)

typedef void(*pFuncTester)();


class Battery;


struct HAL_RTC
{
    static void Init();
    static PackedTime GetPackedTime();
    static bool SetPackedTime(const PackedTime &time);
};

struct HAL_ADC1
{
    static void Init();
    static void SetActiveChannel2();
    static void SetActiveChannel9();
    static uint ReadValue();
};

struct HAL_ADC3
{
    static void Init();
};

struct HAL_DAC1
{
    static void Init();
    static void StartDMA(void *points, uint numPoints);
    static void StopDMA();
    static void ConfigTIM7(uint16 prescaler, uint16 period);
};

struct HAL_TIM2
{
    static uint TimeUS();
    static uint TimeTicks();
    static void Init(uint prescaler, uint period);
    static void DeInit();
    static void Start();
    static void Stop();
    static void StartMultiMeasurement();
};

struct HAL_TIM3
{
    static void Init(uint prescaler, uint period);
    static void DeInit();
    static void Start();
    static void Stop();
    static void EnableIRQ(uint mainPriority, uint subPriority);
    static void DisableIRQ();
    static void StartIT(uint period);
    static void StopIT();
};

struct HAL_USART3
{
    static void Init(pFuncVV recvCallback);
    static void Transmit(void *buffer, uint size, uint timeout);
    static void StartReceiveIT(void *buffer, uint size);
};

struct HAL_DAC2
{
    static void Init();
    static void SetValue(uint value);
};

struct HAL_FLASH
{
    struct Sector
    {
        static const int _00_BOOT_1     = 0;
        static const int _01_BOOT_2     = 1;
        static const int _02            = 2;
        static const int _03            = 3;
        static const int _04            = 4;
        static const int _05_FIRM_1     = 5;
        static const int _06_FIRM_2     = 6;
        static const int _07_FIRM_3     = 7;
        static const int _08            = 8;
        static const int _09            = 9;
        /// ѕервый сектор дл€ сохранени€ настроек. ѕри его заполнении начинает использоватьс€ сектор 2.
        static const int _10_SETTINGS_1 = 10;
        static const int _11_SETTINGS_2 = 11;
        static const int _12            = 12;
        static const int _13            = 13;
        static const int _14            = 14;
        static const int _15            = 15;
        static const int _16            = 16;
        static const int _17_RECORDER_1 = 17;
        static const int _18_RECORDER_2 = 18;
        static const int _19_DATA_1     = 19;
        static const int _20_DATA_2     = 20;
        static const int _21_DATA_3     = 21;
        static const int _22_DATA_4     = 22;
        static const int _23_DATA_5     = 23;
        static const int NUM_SECTORS    = 24;

        static const uint address[NUM_SECTORS];
        static const uint size[NUM_SECTORS];
    };

    static void EraseSector(uint startAddress);
    static void WriteBytes(uint address, const uint8 *data, int size);
    static void WriteBufferBytes(uint address, const void *buffer, int size);
};

struct HAL_OTP
{
    /// ¬озвращает число свободных мест дл€ записи. ≈сли 0, то места в OTP уже не осталось.
    static int GetSerialNumber(char buffer[17]);
    static bool SaveSerialNumber(char *servialNumber);

};

struct HAL_CRC32
{
    static void Init();
    static uint Calculate(uint address, uint size);
};

struct HAL_NVIC
{
    static uint irqEXTI1;
    static uint irqEXTI9_5;
    static void EnableIRQ(uint irq);
    static void DisableIRQ(uint irq);
    static void SetPriorityIRQ(uint irq, uint main, uint sub);
};

struct HAL_FSMC
{
    static void Init();
    static void WriteToFPGA8(uint8 *address, uint8 value);
    static void WriteToFPGA16(uint8 *address, uint16 value);
    static uint8 ReadFromFPGA(const uint8 *address);
};

struct HAL
{
    static void Init();

    static void ErrorHandler(const char *, int);

    static void Delay(uint timeMS);

    static uint TimeMS();
};

