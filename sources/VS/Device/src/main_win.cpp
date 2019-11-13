#include "stdafx.h"
#include "defines.h"
#include "device.h"
#include "Test/Test.h"



void init()
{
    Device::Init();
}



void update()
{
    static bool first = true;

    if (first)
    {
        first = false;
        set.dbg.runTest = true;
        Test::Run();
    }

    Device::Update();
}
