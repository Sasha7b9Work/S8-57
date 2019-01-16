#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "Osci/Osci_Settings.h"
#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Settings.h"
#endif


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
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::DeInit()
{

}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Recorder::Update()
{

}
