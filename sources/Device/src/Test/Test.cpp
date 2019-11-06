#include "defines.h"
#include "Test/Test.h"


void Test::Run()
{
    Display::Init();

    FlashMemory::Data::Test();

    Display::DeInit();
}
