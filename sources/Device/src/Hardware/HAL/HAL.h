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
#define HAL_IWDG_REFRESH()  (*(reinterpret_cast<uint *>(0x40003000U)) = IWDG_KEY_RELOAD)            // �������� � #define ��� ���������� ��������
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


// ������������ ��� ������ � ��������
namespace HAL_ADC1
{
    void Init();

    // ������ ��� �������
    uint ValueBattery();

    // ������ ��� ��������� ����������
    uint ValueCharger();
};


// ����������� � ������ �������������
namespace HAL_ADC3
{
    void Init();
    uint16 ValueRandomizer();
};


namespace HAL_CRC32
{
    void Init();

    // ����������� ����������� ����� ������������������ ���� �� buffer. ���� size �� ������ ������, ��������� ����� �� ��������� � ���������� ����������� �����
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

    // ������������� ����� ������, ������� �� �������� ���� ����� �� ����� ���� ������ ���������
    void InitPanel();

    // ��������������� ��� ������ �� ���� FSMC � �������� � �������
    void ConfigureToFSMC();

    struct Panel
    {
        static void Send(uint8 byte);
        static void Send(uint8 byte0, uint8 byte1);
        static void Send(const uint8 *data, int size);
        static bool Receive();
        static bool InInteraction();
        static void RunAfterInteraction(void (*func)());

        // ��������� ��� �������� ����� ��������� (�.�. ������ � ���������� ������ � ������������)
        static void ProhibitOtherActions();

        // ��������� ��������� ������ �������� ����� ��������� (������ � ���������� ������ � ������������)
        static void AllowOtherActions();
    };

    namespace FPGA
    {
        void Write8(uint8 *address, uint8 value);
        void Write16(uint8 *address, uint16 value);
        uint8 Read(const uint8 *address);
        // ���������� ����� ��� ������ ������
        void SetAddrData(uint8 *address0, uint8 *address1 = nullptr);
        // ������ ������ � �������������� ������
        uint8 ReadA0();
        uint8 ReadA1();
        // ���������� �������� ��� ������� ������
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


namespace HAL_OTP
{
    // ���������� ����� ��������� ���� ��� ������. ���� 0, �� ����� � OTP ��� �� ��������.
    int GetSerialNumber(char buffer[17]);
    bool SaveSerialNumber(char *servialNumber);
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


// ������������ ��� ���������� ������� � ������ ���������
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
