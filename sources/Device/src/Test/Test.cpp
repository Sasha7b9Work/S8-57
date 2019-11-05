#include "defines.h"
#include "Data/Heap.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Test/Test.h"
#include <cstdlib>


bool Test::Run()
{
    FlashMemory::Data::Test();
    
    return false;
}


bool Test::FlashMemory::Data::Test()
{
    for (int i = 0; i < 100; i++)
    {
        uint enumPoints = std::rand() % ENumPointsFPGA::Count;
    }

    return false;
}
