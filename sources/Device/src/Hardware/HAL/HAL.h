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
        #define SEC_00_BOOT_1     0
        #define SEC_01_BOOT_2     1
        #define SEC_02            2
        #define SEC_03            3
        #define SEC_04            4
        #define SEC_05_FIRM_1     5
        #define SEC_06_FIRM_2     6
        #define SEC_07_FIRM_3     7
        #define SEC_08            8
        #define SEC_09            9
        /// ѕервый сектор дл€ сохранени€ настроек. ѕри его заполнении начинает использоватьс€ сектор 2.
        #define SEC_10_SETTINGS_1 10
        #define SEC_11_SETTINGS_2 11
        #define SEC_12            12
        #define SEC_13            13
        #define SEC_14            14
        #define SEC_15            15
        #define SEC_16            16
        #define SEC_17_RECORDER_1 17
        #define SEC_18_RECORDER_2 18
        #define SEC_19_DATA_1     19
        #define SEC_20_DATA_2     20
        #define SEC_21_DATA_3     21
        #define SEC_22_DATA_4     22
        #define SEC_23_DATA_5     23
        #define SEC_NUM_SECTORS   24

        static const uint address[SEC_NUM_SECTORS];
        static const uint size[SEC_NUM_SECTORS];

        static void Erase(int num);
        /// ¬озвращает номер сектора, которому принадлежит address
        static int Num(uint address);

        static bool IsConsist(int num, uint address);
    };

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

