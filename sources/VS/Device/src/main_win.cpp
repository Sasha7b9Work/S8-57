#include "stdafx.h"
#include "defines.h"
#include "device.h"
#include "Test/Test.h"



void init()
{
    Device::Init();

    Test::Run();
}



void update()
{
    Device::Update();
}
