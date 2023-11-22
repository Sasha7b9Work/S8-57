// 2023/11/21 09:57:33 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Hardware/Memory/Sector.h"
#include "Osci/DeviceSettings.h"


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0

#define ERROR_HANDLER()             ::HAL::ErrorHandler(__FILE__, __LINE__)


#define HAL_BUS_CONFIGURE_TO_FSMC() if(HAL_BUS::mode != HAL_BUS::Mode::FSMC) { HAL_BUS::ConfigureToFSMC(); }


typedef struct _USBH_HandleTypeDef USBH_HandleTypeDef;


#ifdef WIN32
#define HAL_IWDG_REFRESH()
#else
#define HAL_IWDG_REFRESH()  (*(reinterpret_cast<uint *>(0x40003000U)) = IWDG_KEY_RELOAD)            // Вынесено в #define для уменьшения задержки
#endif


struct HAL
{
    static void Init();

    static void ErrorHandler(pCHAR, int);

    static void Delay(uint timeMS);

    static uint TimeMS();
};


struct HAL_RTC
{
    static void Init();
    static PackedTime GetPackedTime();
    static bool SetPackedTime(const PackedTime &time);
};


// Используется для работы с питанием
namespace HAL_ADC1
{
    void Init();

    // Читает АЦП батареи
    uint ValueBattery();

    // Читает АЦП зарядного устройства
    uint ValueCharger();
};


// Исользуется в режиме рандомизатора
namespace HAL_ADC3
{
    void Init();
    uint16 ValueRandomizer();
};


namespace HAL_CRC32
{
    void Init();

    // Расчитывает контрольную сумму последовательности байт из buffer. Если size не кратно четырём, последние байты не участвуют в вычислении контрольной суммы
    uint Calculate8bit(uint8 *buffer, int size);
};


namespace HAL_DAC1
{
    void Init();
    void StartDMA(void *points, uint numPoints);
    void StopDMA();
    void ConfigTIM7(uint16 prescaler, uint16 period);
};


namespace HAL_DAC2
{
    void Init();
    void SetValue(uint value);
};


namespace HAL_BUS
{
    void Init();

    // Инициализация пинов панели, которые не изменяют свой режим во время всей работы программы
    void InitPanel();

    // Конфигурировать для работы по шине FSMC с альтерой и памятью
    void ConfigureToFSMC();

    struct Panel
    {
        static void Send(uint8 byte);
        static void Send(uint8 byte0, uint8 byte1);
        static void Send(const uint8 *data, int size);
        static bool Receive();
        static bool InInteraction();
        static void RunAfterInteraction(void (*func)());

        // Запретить все действия кроме указааных (т.е. чтение в поточечном режиме и регистраторе)
        static void ProhibitOtherActions();

        // Разрешить некоторые другие действия кроме указанных (чтение в поточечном режиме и регистраторе)
        static void AllowOtherActions();
    };

    namespace FPGA
    {
        void Write8(uint8 *address, uint8 value);
        void Write16(uint8 *address, uint16 value);
        uint8 Read(const uint8 *address);
        // Установить адрес для чтения данных
        void SetAddrData(uint8 *address0, uint8 *address1 = nullptr);
        // Читать данные с установленного адреса
        uint8 ReadA0();
        uint8 ReadA1();
        // Возвращает растяжку для данного адреса
        float GetStretch(const uint8 *address);
    };

    struct Mode
    {
        enum E
        {
            FSMC,
            PanelRead,
            PanelWrite
        };
    };

    extern Mode::E mode;
};


struct HAL_HCD
{
    static void Init();

    static void InitUSBH_LL(USBH_HandleTypeDef *phost);

    static void *handleHCD;
};


struct HAL_NVIC
{
    static uint irqEXTI1;
    static uint irqEXTI9_5;
    static void EnableIRQ(uint irq);
    static void DisableIRQ(uint irq);
    static void SetPriorityIRQ(uint irq, uint main, uint sub);
    static void SystemReset();
};


struct HAL_OTP
{
    // Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
    static int GetSerialNumber(char buffer[17]);
    static bool SaveSerialNumber(char *servialNumber);
};


struct HAL_PCD
{
    static void Init();
};


struct HAL_ROM
{
    static const Sector sectors[Sector::Count];

    static void WriteBytes(uint address, const uint8 *data, int size);
    static void WriteBufferBytes(uint address, const void *data, int size);
    static void Fill(uint address, uint8 value, int size);
};


struct HAL_SPI4
{
    static void Init();
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


// Используется для выключения прибора в случае зависания
struct HAL_TIM5
{
    static void Init();
    static void ElapsedCallback();
};


struct HAL_USART3
{
    static void Init(pFuncVV recvCallback);
    static void Transmit(void *buffer, int size, uint timeout);
    static void StartReceiveIT(void *buffer, int size);
};
