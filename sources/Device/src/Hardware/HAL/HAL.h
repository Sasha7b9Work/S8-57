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


struct Packet;

struct Sector
{
    enum E
    {
        _00_BOOT_1,
        _01_BOOT_2,
        _02,
        _03,
        _04,
        _05_FIRM_1,
        _06_FIRM_2,
        _07_FIRM_3,
        _08,
        _09,
        /// ѕервый сектор дл€ сохранени€ настроек. ѕри его заполнении начинает использоватьс€ сектор 2.
        _10_SETTINGS_1,
        _11_SETTINGS_2,
        _12,
        _13,
        _14,
        _15,
        _16,
        _17_RECORDER_1,
        _18_RECORDER_2,
        _19_DATA_1,
        _20_DATA_2,
        _21_DATA_3,
        _22_DATA_4,
        _23_DATA_5,
        Count
    } number;

    uint address;

    uint size;
    /// јдрес первого байта за сектором
    uint End() const;

    void Erase() const;
    /// ѕеремещает пакеты из сектора src. ѕакеты в исходном секторе src затираютс€
    void MovePacketsFromSector(const Sector *src) const;
    /// ¬озвращает указатель на первый пакет с данными. nullptr, если пакетов с данными нет
    const Packet *GetFirstPacketWithData() const;
    /// ¬озвращает номер сектора, которому принадлежит address
    static int Number(uint address);
};

#define SECTOR(i) HAL_FLASH::sectors[i]
#define ADDR_SECTOR(i) (SECTOR(i).address)
#define SIZE_SECTOR(i) (SECTOR(i).size)
#define END_SECTOR(i) (ADDR_SECTOR(i) + SIZE_SECTOR(i))

struct HAL_FLASH
{
    static const Sector sectors[Sector::Count];

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

