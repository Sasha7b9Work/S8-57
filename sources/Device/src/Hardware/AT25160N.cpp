//#include <stm32f4xx_hal.h>
#include "defines.h"
#include "AT25160N.h"
#include <cstdlib>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PIN_OUT     GPIOC, GPIO_PIN_3
#define PIN_IN      GPIOC, GPIO_PIN_2
#define PIN_CLK     GPIOB, GPIO_PIN_10
#define PIN_CS      GPIOB, GPIO_PIN_12

#define WREN    BIN_U8(00000110)        ///< Set Write Enable Latch
#define WRDI    BIN_U8(00000100)        ///< Reset Write Enable Latch
#define RDSR    BIN_U8(00000101)        ///< Read Status Register
#define READ    BIN_U8(00000011)        ///< Read Data from Memory Array
#define WRITE   BIN_U8(00000010)        ///< Write Data to Memory Array
#define WRSR    BIN_U8(00000001)        ///< Write Status Register


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Разрешить запись
static void SetWriteLatch();
/// Запретить запись
static void ResetWriteLatch();
/// Читать регистр статуса
static uint8 ReadStatusRegister();
/// Записать регистр статуса
static void WriteStatusRegister(uint8 data);
/// Записывает size байт, начиная с адреса address
static void WriteData(uint address, uint8 *data, uint size);
/// Посылает порцию буфера по данному адресу. Порция не может быть больше 32 байт
static void Write32BytesOrLess(uint address, const uint8 *data, uint size);
/// Читает size байт, начиная с адреса address
static void ReadData(uint address, uint8 *data, uint size);

/// Записывает байт в микросхему
static void WriteByte(uint8 byte);
/// Читает байт из микросхемы
static uint8 ReadByte();

/// Ожидает, пока не закончится внутреннй цикл записи
static void WaitFinishWrite();


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void AT25160N::Init()
{
    //__HAL_RCC_GPIOB_CLK_ENABLE();
    //__HAL_RCC_GPIOC_CLK_ENABLE();

    /*  Аппаратные ресурсы
    SPI2
    PB12 - NSS
    PB10 - SCK
    PC3  - MOSI
    PC2 -  MISO   */

    //__HAL_RCC_SPI2_CLK_ENABLE();

    GPIO_InitTypeDef isGPIO =
    {//     SCK         NSS
        GPIO_PIN_10 | GPIO_PIN_12,
        GPIO_MODE_OUTPUT_PP,
        GPIO_PULLDOWN
    };
    HAL_GPIO_Init(GPIOB, &isGPIO);

    HAL::PORTS::Init(HAL::PORTS::P)

    //             MOSI
    isGPIO.Pin = GPIO_PIN_3;
    HAL_GPIO_Init(GPIOC, &isGPIO);

    //             MISO
    isGPIO.Pin = GPIO_PIN_2;
    isGPIO.Mode = GPIO_MODE_INPUT;
    HAL_GPIO_Init(GPIOC, &isGPIO);

    SetPin(PIN_CS);
    ResetPin(PIN_OUT);
    ResetPin(PIN_CLK);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Test()
{
    const uint size = 1000;
    uint8 data[size];
    uint8 out[size];
    for(uint i = 0; i < size; i++)
    {
        data[i] = (uint8)std::rand();
    }

//    uint timeStart = TIME_MS;

    SetWriteLatch();

//    uint time1 = TIME_MS;

    WriteData(0, data, size);

//    uint time2 = TIME_MS;

    //WaitFinishWrite();

    ReadData(0, out, size);
    ResetWriteLatch();

//    uint time3 = TIME_MS;

    //LOG_WRITE("1 = %d, 2 = %d, 3 = %d, %d", time1 - timeStart, time2 - time1, time3 - time2, time3 - timeStart);

    bool testIsOk = true;

    for(uint i = 0; i < size; i++)
    {
        if(data[i] != out[i])
        {
            testIsOk = false;
            LOG_WRITE("ошибка на %d-м элементе", i);
            break;
        }
    }

    WriteStatusRegister(0);

    if(testIsOk)
    {
        LOG_WRITE("Test is OK!!!");
    }
    else
    {
//        LOG_WRITE("WARNING!!! Test is failed!!!");
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteData(uint address, uint8 *data, uint size)
{
    while(1)
    {
        if(size <= 32)
        {
            Write32BytesOrLess(address, data, size);
            break;
        }
        Write32BytesOrLess(address, data, 32);
        address += 32;
        data += 32;
        size -= 32;    
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Write32BytesOrLess(uint address, const uint8 *data, uint size)
{
    WaitFinishWrite();

    SetWriteLatch();

    ResetPin(PIN_CS);

    WriteByte(WRITE); //-V2501

    WriteByte((address >> 8) & 0xff);

    WriteByte(address & 0xff);

    for (uint i = 0; i < size; i++)
    {
        uint8 byte = data[i];

        for (int bit = 7; bit >= 0; bit--)
        {
            if (_GET_BIT(byte, bit))
            {
                GPIOC->BSRR = GPIO_PIN_3;
            }
            GPIOB->BSRR = GPIO_PIN_10;
            GPIOC->BSRR = GPIO_PIN_3 << 16U;
            GPIOB->BSRR = GPIO_PIN_10 << 16U;
        }
    }

    HAL_GPIO_WritePin(PIN_CS, GPIO_PIN_SET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetWriteLatch()
{
    //WaitFinishWrite();

    ResetPin(PIN_CS);
    WriteByte(WREN); //-V2501
    SetPin(PIN_CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ResetWriteLatch()
{
    WaitFinishWrite();

    ResetPin(PIN_CS);
    WriteByte(WRDI); //-V2501
    SetPin(PIN_CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ReadStatusRegister()
{
    ResetPin(PIN_CS);
    WriteByte(RDSR); //-V2501
    uint8 result = ReadByte();
    SetPin(PIN_CS);
    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteStatusRegister(uint8 data)
{
    WaitFinishWrite();

    ResetPin(PIN_CS);
    WriteByte(WRSR); //-V2501
    WriteByte(data);
    SetPin(PIN_CS);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WaitFinishWrite()
{
    while (_GET_BIT(ReadStatusRegister(), 0))
    {
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Save(Settings &)
{
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void AT25160N::Load(Settings &)
{
    /*
    Алгоритм загрузки настроек
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteByte(uint8 byte)
{
    for(int bit = 7; bit >= 0; bit--)
    {
        if (_GET_BIT(byte, bit))
        {
            SetPin(PIN_OUT);
        }
        SetPin(PIN_CLK);
        ResetPin(PIN_CLK);
        ResetPin(PIN_OUT);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 ReadByte()
{
    uint8 retValue = 0;

    for(int i = 0; i < 8; i++)
    {
        SetPin(PIN_CLK);
        retValue <<= 1;
        if(HAL_GPIO_ReadPin(PIN_IN) == GPIO_PIN_SET)
        {
            retValue |= 0x01;
        }
        ResetPin(PIN_CLK);
    }

    return retValue;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SetPin(GPIO_TypeDef *_gpio, uint16 pin)
{
    HAL_GPIO_WritePin(_gpio, pin, GPIO_PIN_SET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ResetPin(GPIO_TypeDef *_gpio, uint16 pin)
{
    HAL_GPIO_WritePin(_gpio, pin, GPIO_PIN_RESET);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadData(uint address, uint8 *data, uint size)
{
    WaitFinishWrite();

    ResetPin(PIN_CS);

    WriteByte(READ); //-V2501
    WriteByte((address >> 8) & 0xff);
    WriteByte(address & 0xff);

    for(uint i = 0; i < size; i++) //-V756
    {
        data[i] = 0;

        for (int j = 0; j < 8; j++)
        {
            GPIOB->BSRR = GPIO_PIN_10;

            data[i] <<= 1;
            if (HAL_GPIO_ReadPin(PIN_IN) == GPIO_PIN_SET)
            {
                data[i] |= 0x01;
            }

            GPIOB->BSRR = GPIO_PIN_10 << 16U;
        }
    }

    SetPin(PIN_CS);
}
