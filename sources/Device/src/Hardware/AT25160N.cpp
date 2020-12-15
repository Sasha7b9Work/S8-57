#include "defines.h"
#include "log.h"
#include "AT25160N.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Utils/Values.h"
#include <cstdlib>


#define WREN    BIN_U8(00000110)        // Set Write Enable Latch
#define WRDI    BIN_U8(00000100)        // Reset Write Enable Latch
#define RDSR    BIN_U8(00000101)        // Read Status Register
#define READ    BIN_U8(00000011)        // Read Data from Memory Array
#define WRITE   BIN_U8(00000010)        // Write Data to Memory Array
#define WRSR    BIN_U8(00000001)        // Write Status Register



struct Pin
{
    Pin(HPort::E _port, uint16 _pin) : port(_port), pin(_pin) {}
    HPort::E port;
    uint16 pin;
};


struct PinIn : public Pin
{
    PinIn(HPort::E port, uint16 pin) : Pin(port, pin) { }
    void Init() { HAL_PIO::Init(port, pin, HMode::Input, HPull::Up); }
    uint8 Read() { return HAL_PIO::Read(port, pin); }
};


struct PinOut : public Pin
{
    PinOut(HPort::E port, uint16 pin) : Pin(port, pin) { }
    void Init()  { HAL_PIO::Init(port, pin, HMode::Output_PP, HPull::Up); }
    void Set()   { HAL_PIO::Set(port, pin); }
    void Reset() { HAL_PIO::Reset(port, pin); }
};


static PinOut CLK(PIN_AT2516_CLK);
static PinOut CS(PIN_AT2516_CS);
static PinOut OUT(PIN_AT2516_OUT);
static PinIn  IN(PIN_AT2516_IN);


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

    CLK.Init();
    CS.Init();
    OUT.Init();
    IN.Init();

    CS.Set();
    OUT.Reset();
    CLK.Reset();
}


void AT25160N::Test()
{
    const uint size = 1000;
    uint8 data[size];
    uint8 read[size];
    for(uint i = 0; i < size; i++)
    {
        do
        {
            data[i] = static_cast<uint8>(std::rand());
        } while(data[i] == 0);
    }

    SetWriteLatch();

    WriteData(0, data, size);

    uint8 status = BIN_U8(00000000); //-V2501

    LOG_WRITE("Пишу статус %d", status);

    WriteStatusRegister(status);

    WaitFinishWrite();

    LOG_WRITE("status = %d", ReadStatusRegister());

    ReadData(0, read, size);
    ResetWriteLatch();

    bool testIsOk = true;

    for(uint i = 0; i < size; i++)
    {
        if(data[i] != read[i])
        {
            testIsOk = false;

            LOG_WRITE("ошибка на %d-м элементе", i);

            break;
        }
    }

    if(testIsOk)
    {
        LOG_WRITE("Тест пройден успешно");
    }

    LOG_WRITE("status = %d", ReadStatusRegister());
}


void AT25160N::WriteData(uint address, uint8 *data, int size)
{
    while(1)
    {
        if(size <= 32)
        {
            if(size)
            {
                Write32BytesOrLess(address, data, size);
            }
            break;
        }
        Write32BytesOrLess(address, data, 32);
        address += 32;
        data += 32;
        size -= 32;    
    }
}


void AT25160N::Write32BytesOrLess(uint address, const uint8 *, int size)
{
    WaitFinishWrite();

    SetWriteLatch();

    CS.Reset();

    WriteByte(WRITE); //-V2501

    WriteByte((address >> 8) & 0xff);

    WriteByte(address & 0xff);

    for (int i = 0; i < size; i++)
    {
        //uint8 byte = data[i];

        for (int bit = 7; bit >= 0; bit--)
        {
        }
    }

    CS.Set();
}


void AT25160N::SetWriteLatch()
{
    CS.Reset();

    WriteByte(WREN); //-V2501

    CS.Set();
}


void AT25160N::ResetWriteLatch()
{
    WaitFinishWrite();

    CS.Reset();

    WriteByte(WRDI); //-V2501

    CS.Set();
}


uint8 AT25160N::ReadStatusRegister()
{
    CS.Reset();

    WriteByte(RDSR); //-V2501

    uint8 result = ReadByte();

    CS.Set();

    if(result)
    {
        char buffer[36];
        LOG_WRITE("регистр статуса %s", Hex(result).ToBin(8, buffer));
    }

    return result;
}


void AT25160N::WriteStatusRegister(uint8 data)
{
    WaitFinishWrite();

    CS.Reset();

    WriteByte(WRSR); //-V2501

    WriteByte(data);

    CS.Set();
}


void AT25160N::WaitFinishWrite()
{
    while (_GET_BIT(ReadStatusRegister(), 0))
    {
    }
}


void AT25160N::Save(Settings &)
{
}


void AT25160N::Load(Settings &)
{
    /*
    Алгоритм загрузки настроек
    */
}


void AT25160N::WriteByte(uint8 byte)
{
    for(int bit = 7; bit >= 0; bit--)
    {
        if (_GET_BIT(byte, bit))
        {
            OUT.Set();
        }

        CLK.Set();
        CLK.Reset();
        OUT.Reset();
    }
}


uint8 AT25160N::ReadByte()
{
    uint8 retValue = 0;

    for(int i = 0; i < 8; i++)
    {
        CLK.Set();

        CLK.Reset();

        if(IN.Read())
        {
            retValue |= 0x01;
        }

        retValue <<= 1;
    }

    return retValue;
}


void AT25160N::ReadData(uint address, uint8 *data, int size)
{
    WaitFinishWrite();

    CS.Reset();

    WriteByte(READ); //-V2501
    WriteByte((address >> 8) & 0xff);
    WriteByte(address & 0xff);

    for(int i = 0; i < size; i++) //-V756
    {
        data[i] = 0;

        for (int j = 0; j < 8; j++)
        {
            //GPIOB->BSRR = GPIO _PIN_10;
            //
            //data[i] <<= 1;
            //if (HAL_PIO::Read(PIN_IN))
            //{
            //    data[i] |= 0x01;
            //}
            //
            //GPIOB->BSRR = GPIO _PIN_10 << 16U;
        }
    }

    CS.Set();
}
