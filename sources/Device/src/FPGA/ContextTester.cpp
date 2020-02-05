#include "defines.h"
#include "FPGA/ContextTester.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Settings/Settings.h"
#include "Hardware/Timer.h"


void ContextTester::OnPressStart()
{
    FPGA::OnPressStart();
}

bool ContextTester::Start()
{
    // У нас двенадцать делений. На двенадцать делений должно приходиться не менее 2.5 мс
    // 2.5мс / 12дел = 0.2 мс/дел = 10мкс/тчк

    TBase::Set500us();

    FPGA::GiveStart(static_cast<uint16>(~(400)), static_cast<uint16>(~(1)));

    FPGA::ForcedStart();

    return true;
}

bool ContextTester::Read(uint16 *dataA, uint8 *dataB)
{
    uint start = TIME_MS;
    FPGA::flag.flag = 0;
    while (!FPGA::flag.DataReady())         // Ждём флага готовности данных
    {
        FPGA::ReadFlag();

        if (TIME_MS - start > 20)           /// \todo Временная затычка. Надо сделать так, чтобы такие ситуации были исключены. Сбои происходят, во время
        {                                   /// нажатия кнопок
            return false;
        }
    }

    uint16 aRead = (uint16)(FPGA::ReadLastRecord(Chan::A) - TESTER_NUM_POINTS);

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // Указываем адрес, с которого будем читать данные
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // И даём команду ПЛИС, чтобы чтение начиналось с него

    HAL_BUS::FPGA::SetAddrData(RD::DATA_A + 1, RD::DATA_B + 1);

    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные первого канала
    {
        *dataA++ = HAL_BUS::FPGA::ReadA0();
    }

    HAL_BUS::FPGA::Write16(WR::PRED_LO, aRead);         // Указываем адрес, с котонрого будем читать данные
    HAL_BUS::FPGA::Write8(WR::START_ADDR, 0xff);        // И даём команду ПЛИС, чтобы чтение начиналось с него

    for (int i = 0; i < TESTER_NUM_POINTS; i++)         // Читаем данные второго канала
    {
        *dataB++ = HAL_BUS::FPGA::ReadA1();
    }

    return true;
}
