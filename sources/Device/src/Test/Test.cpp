#include "defines.h"
#include "Device.h"
#include "Hardware/Timer.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>



static void RunTest(bool (*func)());


void Test::Run()
{
    if (!set.dbg.runTest)
    {
        return;
    }

    std::srand(TIME_MS);

    set.dbg.runTest = false;
    
    Display::Init();

    RunTest(RAM::Test);

    RunTest(ROM::Data::Test);

    set.Save();

    Device::Reset();

    Display::DeInit();
}


static void RunTest(bool (*func)())
{
    if (!func())
    {
        Test::Display::AddMessage("!!! Ошибка !!!");
        while (true)
        {

        }
    }
}
