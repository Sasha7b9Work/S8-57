#pragma once
#include "defines.h"
#include "Hardware/Memory/Sector.h"
#include "Osci/DataSettings.h"


#define PRIORITY_SOUND_DMA1_STREAM5 5, 0

#define ERROR_HANDLER()            ::HAL::ErrorHandler(__FILE__, __LINE__)


struct HAL_RTC
{
    static void Init();
    static PackedTime GetPackedTime();
    static bool SetPackedTime(const PackedTime &time);
};


struct HAL_ADC1
{
    static void Init();
    /// Читает АЦП батареи
    static uint ReadValueAKK();
    /// Читает АЦП зарядного устройства
    static uint ReadValuePOW();
private:
    static void SetActiveChannel2();
    static void SetActiveChannel9();
    static uint ReadValue();
};


struct HAL_ADC3
{
    static void Init();
};


struct HAL_CRC32
{
    static void Init();
    static uint Calculate(uint address, uint size);
};


struct HAL_DAC1
{
    static void Init();
    static void StartDMA(void *points, uint numPoints);
    static void StopDMA();
    static void ConfigTIM7(uint16 prescaler, uint16 period);
};


struct HAL_DAC2
{
    static void Init();
    static void SetValue(uint value);
};


struct HAL_BUS
{
    /// *********** Функции взаимодействия с альтерой ****************
    static void Init();
    static void WriteToFPGA8(uint8 *address, uint8 value);
    static void WriteToFPGA16(uint8 *address, uint16 value);
    static uint8 ReadFromFPGA(const uint8 *address);

    /// Установить адрес для чтения данных
    static void SetAddrData(uint8 *address0, uint8 *address1 = nullptr);
    /// Читать данные с установленного адреса
    static uint8 ReadData0();
    static uint8 ReadData1();

    /// ************* Функции взаимодействия с памятью ****************
    struct RAM
    {
        /// Записывает во внешнюю память по адресу address данные buffer, size кратен 4
        static void Write(uint8 *buffer, uint size, uint8 *address);
        /// Читает из внешней памяти address в буфер buffer данные, size кратен 4
        static void Read(uint8 *buffer, uint size, uint8 *address);

    };

    /// Возвращает сколько процентов данных считанных неправильно.
    static float TestRAM1();
    static float TestRAM2();
    /// Возвращает время в секундах, которое занимает запись/чтение sizekB килобайт во внешнюю память. Возвращает -1, если произошла ошибка
    static float TestTimeRAM(uint sizekB);

    /// ************* Функции взаимодействия с панелью ****************

    static void SendToPanel(uint8 byte);
    static void SendToPanel(uint8 byte0, uint8 byte1);
    static void SendToPanel(uint8 *data, uint size);

    static bool Receive();

    static bool InteractionWithPanel();
    /// Возвращает растяжку для данного адреса
    static float GetStretch(const uint8 *address);
    /// Конфигурировать для работы по шине FSMC с альтерой и памятью
    static void ConfigureToFSMC();
private:
    /// Первый адрес чтения данных
    static uint8 *addrData0;
    /// Второй адрес чтения данных
    static uint8 *addrData1;
    /// Инициализация пинов панели, которые не изменяют свой режим во время всей работы программы
    static void InitPanel();
    /// Настроить FSMC для работы с внешней RAM
    static void InitRAM();
    /// Тест скорости записи/чтение 1кБ внешней RAM
    static float TestTime1kB(uint8 *address);

    struct Mode
    {
        enum E
        {
            FPGA,
            PanelRead,
            PanelWrite
        };
    };

    static Mode::E mode;
};


struct HAL_HCD
{
    static void Init();
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
    /// Возвращает число свободных мест для записи. Если 0, то места в OTP уже не осталось.
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

    static void WriteBytes(uint address, const uint8 *data, uint size);
    static void WriteBufferBytes(uint address, const void *data, uint size);
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


struct HAL_USART3
{
    static void Init(pFuncVV recvCallback);
    static void Transmit(void *buffer, uint size, uint timeout);
    static void StartReceiveIT(void *buffer, uint size);
};


struct HAL
{
    static void Init();

    static void ErrorHandler(const char *, int);

    static void Delay(uint timeMS);

    static uint TimeMS();
};

