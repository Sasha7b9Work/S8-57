#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Display/Display.h"
#include "FPGA/FPGA_HAL.h"
#include "Hardware/FSMC.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Settings.h"
#include "Recorder/Recorder_Storage.h"
#endif


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Чтение точки по прерыванию от FPGA
static void ReadPoint();
/// Если true - идёт чтение точек
static bool isRunning = false;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Recorder::Init()
{
    Osci::Settings::Range::LoadBoth();
    Osci::Settings::RShift::Load(Chan::A);
    Osci::Settings::RShift::Load(Chan::B);
    Osci::Settings::Trig::Input::Load();
    Settings::ScaleX::Load();
    Osci::Settings::TShift::Load();
    Osci::Settings::LoadHoldfOff();

    FPGA::HAL::Interrupt::P2P::Init(ReadPoint);

    isRunning = false;

    Stop();

    Storage::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::DeInit()
{
    Stop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Update()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadPoint()
{
    if (Display::InProcess())
    {
        Display::SetFuncAfterUpadteOnce(ReadPoint);
    }
    else
    {
        BitSet16 dataA(FSMC::ReadFromFPGA(RD_DATA_A), FSMC::ReadFromFPGA(RD_DATA_A + 1));
        BitSet16 dataB(FSMC::ReadFromFPGA(RD_DATA_B), FSMC::ReadFromFPGA(RD_DATA_B + 1));

        Recorder::Storage::CurrentFrame().AddPoint(dataA, dataB);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Start()
{
    Storage::CreateNewFrame();

    FSMC::WriteToFPGA16(WR_PRED_LO, 0); //-V525
    FSMC::WriteToFPGA16(WR_POST_LO, 0);
    FSMC::WriteToFPGA8(WR_START, 0xff);

    FPGA::HAL::Interrupt::P2P::Enable();

    isRunning = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Stop()
{
    FPGA::HAL::Interrupt::P2P::Disable();

    isRunning = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::OnPressStart()
{
    if (isRunning)
    {
        Stop();
    }
    else
    {
        Start();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Recorder::IsRunning()
{
    return isRunning;
}
