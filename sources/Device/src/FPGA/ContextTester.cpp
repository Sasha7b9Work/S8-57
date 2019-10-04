#include "defines.h"
#include "FPGA/ContextTester.h"
#include "FPGA/FPGA.h"


void ContextTester::OnPressStart()
{
    FPGA::OnPressStart();
}

bool ContextTester::Start()
{
    return FPGA::ForTester::Start();
}

bool ContextTester::Read(uint16 *dataA, uint8 *dataB)
{
    return FPGA::ForTester::Read(dataA, dataB);
}
