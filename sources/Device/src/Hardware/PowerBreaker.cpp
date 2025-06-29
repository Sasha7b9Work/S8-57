// 2023/11/22 13:39:14 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "device.h"
#include "Display/Display.h"
#include "Hardware/Battery.h"
#include "Hardware/PowerBreaker.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Keyboard/BufferButtons.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


// ��������� ��������������, ���� ���� �������
static void DisablePowerIfBatteryDischarged();

// ��������� �������������� ���� �� ������ ������ � ������� ������������ �������
static void DisableDisplayIfTimeout();


void PowerBreaker::Init()
{
    HAL_PIO::Init(PIN_POWER, HMode::Output_PP, HPull::Up);
    HAL_PIO::Set(PIN_POWER);
}


void PowerBreaker::Update()
{
    DisablePowerIfBatteryDischarged();

    DisableDisplayIfTimeout();
}


static void DisablePowerIfBatteryDischarged()
{
    if (Battery::GetVoltage() < Battery::SHUTDOWN_VOLTAGE)
    {
        HAL_PIO::Reset(PIN_POWER);
    }
}


static void DisableDisplayIfTimeout()
{
    if (S_SERV_TIME_DISABLE_DISPLAY)
    {
        uint timeWait = static_cast<uint>(S_SERV_TIME_DISABLE_DISPLAY * 60 * 1000);

        uint timeout = BufferButtons::TimeAfterControlMS();

        if (timeout >= timeWait)
        {
            Display::Breaker::PowerOff();
        }
    }
}

