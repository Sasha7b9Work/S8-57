#include "defines.h"
#include "Device.h"
#include "Hardware/Timer.h"
#include "SCPI/SCPI.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Test/Test.h"
#include <cstdlib>



static void RunTest(bool (*func)(), char *message);


void Test::Run()
{
    if (!set.dbg.runTest)
    {
        return;
    }

    std::srand(TIME_MS);

    set.dbg.runTest = false;
    
    Display::Init();

    //RunTest(RAM::Test, "Тест ОЗУ");
    //
    //RunTest(ROM::Data::Test, "Тест ППЗУ");

    RunTest(SCPI::Test, "Тест SCPI");

    set.Save();

    Device::Reset();

    Display::DeInit();
}


static void RunTest(bool (*func)(), char *message)
{
    Test::Display::AddMessage(message);

    if (!func())
    {
        Test::Display::AddMessage("!!! Ошибка !!!");
        while (true)
        {

        }
    }
}
