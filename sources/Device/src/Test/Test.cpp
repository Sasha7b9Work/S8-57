#include "defines.h"
#include "Test/Test.h"


void Test::Run()
{
    Display::Init();

    ROM::Data::Test();

    Display::DeInit();
}
