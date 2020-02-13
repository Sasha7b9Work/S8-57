#include "defines.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Hardware/Memory/ExtRAM.h"
#include <cstdlib>
#include <cstring>


uint8 *ExtRAM::Begin()
{
    return reinterpret_cast<uint8 *>(0x68000000U);
}


uint8 *ExtRAM::End()
{
    return Begin() + 512 * 1024;
}


void ExtRAM::Write(uint8 *buffer, uint size, uint8 *address)
{
    HAL_BUS::ConfigureToFSMC();

    std::memcpy(address, buffer, size);
}


void ExtRAM::Read(uint8 *buffer, uint size, uint8 *address)
{
    HAL_BUS::ConfigureToFSMC();

    std::memcpy(buffer, address, size);
}


float ExtRAM::Test1()
{
    HAL_BUS::ConfigureToFSMC();

    int SIZE = 1024 * 512;

    int bad = 0;

    uint8 *address = Begin();

    for(int i = 0; i < SIZE; i++)
    {
        *address = static_cast<uint8>(i);

        uint8 out = *address;

        if(out != static_cast<uint8>(i))
        {
            bad++;
        }

        address++;
    }

    return bad * 100.0F / SIZE;
}


float ExtRAM::Test2()
{
#define SIZE 1024

    int bad = 0;

    uint8 bufferIN[SIZE];
    uint8 bufferOUT[SIZE];
    std::memset(bufferOUT, 0, SIZE);

    for(int x = 0; x < SIZE; x++)
    {
        bufferIN[x] = static_cast<uint8>(std::rand());
    }

    uint8 *address = Begin() + (std::rand() % (500 * 1024));

    Write(bufferIN, SIZE, address);

    Read(bufferOUT, SIZE, address);

    for(int z = 0; z < SIZE; z++)
    {
        volatile uint8 in = bufferIN[z];
        volatile uint8 out = bufferOUT[z];

        if(in != out)
        {
            bad++;
        }
    }

    return (bad * 100.0F) / SIZE;
}


float ExtRAM::TestTime(uint sizekB)
{
    float result = 0;

    for(uint i = 0; i < sizekB; i++)
    {
        float time = TestTime1kB(Begin() + i * 1024);

        if(time == -1.0F)
        {
            return -1.0F;
        }

        result += time;
    }

    return result;
}


float ExtRAM::TestTime1kB(uint8 *address)
{
#define SIZE_BUFFER 1024

    uint8 data[SIZE_BUFFER];
    uint8 out[SIZE_BUFFER];

    for(int i = 0; i < SIZE_BUFFER; i++)
    {
        data[i] = static_cast<uint8>(std::rand());
    }

    uint start = Timer::TimeUS();

    Write(data, SIZE_BUFFER, address);

    Read(out, SIZE_BUFFER, address);

    float time = (Timer::TimeUS() - start) / 1e6F;

    for(int i = 0; i < SIZE_BUFFER; i++)
    {
        if(data[i] != out[i])
        {
            return -1.0F;
        }
    }

    return time;
}


uint8 *RandRD()
{
    return RD::DATA_A + (std::rand() % 25);
}


uint8 *RandWR()
{
    return WR::START + (std::rand() % 19);
}


bool ExtRAM::Test3(int *num)
{
    uint8 *array1 = Begin();

    uint8 *array2 = Begin() + 256 * 1024;

    HAL_BUS::ConfigureToFSMC();

    for(int i = 0; i < 256 * 1024; i++)
    {
        //array1[i] = array2[i] = static_cast<uint8>(std::rand());

        array1[i] = array2[i] = static_cast<uint8>(std::rand());

        *RandWR() = *RandRD();

        if(array1[i] != array2[i])
        {
            *num = i;
            return false;
        }
    }

    array1 = Begin();
    array2 = Begin() + 256 * 1024;
    
    for(int i = 0; i < 256 * 1024; i++)
    {
        *RandWR() = *RandRD();

        uint8 data1 = *array1++;
        uint8 data2 = *array2++;
        
        if(data1 != data2)
        {
            *num = i;
            return false;
        }
    }

    return true;
}
