#include <stm32f4xx_hal.h>
#include "defines.h"
#include "HAL.h"
#include "Hardware/Timer.h"
#include "Keyboard/DecoderDevice.h"
#include "Utils/Debug.h"


using HAL::FSMC;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define ADDR_ALTERA1    ((uint8 *)NOR_MEMORY_ADRESS1)
#define ADDR_ALTERA2    ((uint8 *)NOR_MEMORY_ADRESS2)
#define ADDR_ALTERA3    ((uint8 *)NOR_MEMORY_ADRESS3)
#define ADDR_DISPLAY    ((uint8 *)NOR_MEMORY_ADRESS4)

#define PORT_NE4        GPIOG
#define PIN_NE4         GPIO_PIN_12

#define PORT_PAN_1      GPIOC
#define PIN_PAN_1       GPIO_PIN_4

#define PORT_PAN_0      GPIOA
#define PIN_PAN_0       GPIO_PIN_7

#define NE4_SET         (PORT_NE4->BSRR = PIN_NE4)
#define NE4_RESET       (PORT_NE4->BSRR = (uint)PIN_NE4 << 16U)

#define PAN_BUSY                        (ReadPAN() == 0)
#define PAN_READY_TRANSMIT              (ReadPAN() == 1)
#define PAN_READY_RECEIVE               (ReadPAN() == 2)
#define PAN_RECIEVE_TRANSMIT_CONFIRM    (ReadPAN() == 3)

enum ModeFSMC
{
    ModeNone,
    ModePanelWrite,
    ModePanelRead,
    ModeFPGA
};

static      ModeFSMC    mode = ModeNone;
bool        interchangeWithPanel = false;
/// Если не равно нулю, нужно выполнить эту функцию после завершения обмена с панелью
pFuncTester funcAfterInteractionWithPanel = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CONFIGURE_TO_READ_PANEL                                             \
    /* Обнуляем пины 15, 14, 1, 0. Сразу и установятся в режим ввода */     \
    GPIOD->MODER &= HEX_FROM_2(0fff, fff0);                                 \
    /* Обнуляем пины 7, 8, 9, 10. Сразу и установятся в режим ввода */      \
    GPIOE->MODER &= HEX_FROM_2(ffc0, 3fff);                                 \
    mode = ModePanelRead;

#define CONFIGURE_TO_WRITE_PANEL                                            \
    /* Настроим пины 14, 15, 0, 1 на запись D0, D1, D2, D3 */               \
    /* Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP. */                 \
    GPIOD->MODER &= HEX_FROM_2(0fff, fff0);                                 \
    GPIOD->MODER |= HEX_FROM_2(5000, 0005);                                 \
    /* Настроим пины 7, 8, 9, 10 на запись D4, D5, D6, D7 */                \
    /* Устанавливаем для этих пинов GPIO_MODE_OUTPUT_PP. */                 \
    GPIOE->MODER &= HEX_FROM_2(ffc0, 3fff);                                 \
    GPIOE->MODER |= HEX_FROM_2(0015, 4000);                                 \
    mode = ModePanelWrite;


static void ReadByte();

static void ConfigureForFPGA();
/// Расставляет биты значенния data по ножкам D0...D7
static void SetOutData(uint8 data);
/// Возвращает значение с ножек D0...D7
static uint8 GetOutData();
/// Возвращает состояние PAN_0, PAN_1
static uint8 ReadPAN();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void FSMC::Init()
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ConfigureForFPGA()
{
    static const GPIO_InitTypeDef isGPIO =
    {   //    NOE          NWE          NE1
        GPIO_PIN_4 | GPIO_PIN_5 | GPIO_PIN_7,
        GPIO_MODE_AF_PP,
        GPIO_PULLDOWN,
        GPIO_SPEED_FREQ_VERY_HIGH,
        GPIO_AF12_FMC
    };

    /// \todo Здесь не довеедно - не хотит, почему-то

    HAL_GPIO_Init(GPIOD, (GPIO_InitTypeDef *)&isGPIO);

    //uint startTime = TIME_TICKS;

    // Инициализируем GPIOD 0, 1, 4, 5, 7, 14, 15 - D2, D3, NOE, NWE, NE1, D0, D1

    // Инициализируем GPIOD 0, 1, 4, 5 - D2, D3, NOE, NWE
    GPIOD->AFR[0] &= HEX_FROM_2(ffff, ff00);
    GPIOD->AFR[0] |= HEX_FROM_2(0000, 00cc);

    GPIOD->MODER &= HEX_FROM_2(ffff, fff0);
    GPIOD->MODER |= HEX_FROM_2(0000, 000a);

    // Инициализируем GPIOD 14, 15 - D0, D1
    GPIOD->AFR[1] &= HEX_FROM_2(00ff, ffff);    // PIN_14, PIN_15
    GPIOD->AFR[1] |= HEX_FROM_2(cc00, 0000);    // GPIO_AF12_FMC

    GPIOD->MODER &= HEX_FROM_2(0fff, ffff);
    GPIOD->MODER |= HEX_FROM_2(a000, 0000);     // Alternate function mode

    // Инициализируем GPIOE 7...8 - D4...D7
    GPIOE->AFR[0] &= HEX_FROM_2(0fff, ffff);
    GPIOE->AFR[0] |= HEX_FROM_2(c000, 0000);
    GPIOE->AFR[1] &= HEX_FROM_2(ffff, f000);
    GPIOE->AFR[1] |= HEX_FROM_2(0000, 0ccc);    // GPIO_AF12_FMC

    GPIOE->MODER &= HEX_FROM_2(ffc0, 3fff);
    GPIOE->MODER |= HEX_FROM_2(002a, 8fff);     // Alternate function mode



    mode = ModeFPGA;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteToPanel2bytes(uint8 byte0, uint8 byte1)
{
    uint8 buffer[2] = { byte0, byte1 };
    WriteToPanel(buffer, 2);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteToPanel(const uint8 *, uint)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadByte()
{
    CONFIGURE_TO_READ_PANEL;
LabelReadByte:
    NE4_RESET;
    while (PAN_READY_TRANSMIT) {};
    if (PAN_RECIEVE_TRANSMIT_CONFIRM)
    {
        uint8 data = GetOutData();
        Decoder::AddData(data);
        NE4_SET;
        while (PAN_RECIEVE_TRANSMIT_CONFIRM) {};
    }
    if (PAN_READY_TRANSMIT)
    {
        goto LabelReadByte; //-V2505
    }
    CONFIGURE_TO_WRITE_PANEL;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FSMC::ReadByteNow()
{
    uint8 result = 0;

    CONFIGURE_TO_READ_PANEL;
LabelReadByte:
    NE4_RESET;
    while (PAN_READY_TRANSMIT) {};
    if (PAN_RECIEVE_TRANSMIT_CONFIRM)
    {
        result = GetOutData();
        NE4_SET;
        while (PAN_RECIEVE_TRANSMIT_CONFIRM) {};
    }
    if (PAN_READY_TRANSMIT)
    {
        goto LabelReadByte; //-V2505
    }
    CONFIGURE_TO_WRITE_PANEL;

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ReadPAN()
{
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetOutData(uint8 data)
{
    if (mode != ModePanelWrite)
    {
        CONFIGURE_TO_WRITE_PANEL;
    }

    //                                                        биты 0,1                           биты 2, 3
    GPIOD->ODR = (GPIOD->ODR & 0x3ffc) + (uint16)(((int16)data & 0x03) << 14) + (((uint16)(data & 0x0c)) >> 2);

    //                                                    Биты 4,5,6,7
    GPIOE->ODR = (GPIOE->ODR & 0xf87f) + (uint16)(((int16)data & 0xf0) << 3);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 GetOutData()
{
    if (mode != ModePanelRead)
    {
        CONFIGURE_TO_READ_PANEL;
    }
    uint dataD = GPIOD->IDR;
    uint dataE = GPIOE->IDR;

    return (uint8)(((dataD >> 14) & 0x3) | ((dataD & 0x3) << 2) | ((dataE & 0x780) >> 3));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool FSMC::InterchangeWithPanel()
{
    return interchangeWithPanel;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::RunFunctionAfterInteractionWitchPanel(pFuncTester func)
{
    funcAfterInteractionWithPanel = func;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteToFPGA16(uint8 *, uint16)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void FSMC::WriteToFPGA8(uint8 *, uint8)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint8 FSMC::ReadFromFPGA(const uint8 *address)
{
    if (mode != ModeFPGA)
    {
        ConfigureForFPGA();
    }

    return *address;
}
