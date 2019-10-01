#include "defines.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"
#include "Recorder/Recorder.h"
#include "Device.h"
#include "Hardware/HAL/HAL.h"
#include "Utils/Math.h"


using namespace HAL::ADDRESSES::FPGA;

using HAL::FSMC;

extern const Page pRecorder;



static bool IsActive_Destination()
{
    return !Recorder::IsRunning();
}

DEF_CHOICE_3( cDestination,                                                                                                                     //--- ÔÓÍÊÖÈß - ÐÅÃÈÑÒÐÀÒÎÐ - Ñîõðàíÿòü â ---
    "Ñîõðàíÿòü â",
    "Êóäà ñîõðàíÿòü äàííûå",
    "ÎÇÓ",
    "Âíóòð ÇÓ",
    "Âíåøí ÇÓ",
    set.rec.storageRecord, &PageRecorder::self, IsActive_Destination, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Start()
{
    return set.rec.enabledChanA || set.rec.enabledChanB || set.rec.enabledSensor;
}

static void Draw_Start(int x, int y)
{
    String("ÏÓÑÊ").Draw(x, y + 5);
}

static void Draw_Stop(int x, int y)
{
    String("ÑÒÎÏ").Draw(x, y + 5);
}

static void Draw_StartStop(int x, int y)
{
    Recorder::IsRunning() ? Draw_Stop(x, y) : Draw_Start(x, y);
}

static void OnPress_Start()
{
    Recorder::OnPressStart();
}

DEF_GRAPH_BUTTON_HINTS_2( bStart,                                                                                                                 //--- ÔÓÍÊÖÈß - ÐÅÃÈÑÒÐÀÒÎÐ - ÏÓÑÊ/ÑÒÎÏ ---
    "Ïóñê",
    "Çàïóñê/îñòàíîâ ïðîöåññà ðåãèñòðàöèè",
    &PageRecorder::self, IsActive_Start, OnPress_Start, Draw_StartStop,
    Draw_Start, "Çàïóñê",
    Draw_Stop, "Îñòàíîâ"
)



static void OnOpenClose_Recorder(bool enter)
{
    Device::State::SetMode(enter ? Device::Mode::Recorder : Device::Mode::Osci);
}

DEF_PAGE_4( pRecorder, // -V641 // -V1027                                                                                                                     //--- ÔÓÍÊÖÈß - ÐÅÃÈÑÒÐÀÒÎÐ ---
    "ÐÅÃÈÑÒÐÀÒÎÐ",
    "Çàïèñü è âîñïðîèçâåäåíèå ñèãíàëîâ âõîäîâ è äàò÷èêîâ",
    PageRecorder::PageSource::self,
    &cDestination,
    PageRecorder::PageShow::self,
    &bStart,
    PageName::Recorder, &PageFunction::self, Item::Active, OnOpenClose_Recorder, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page *const PageRecorder::self = (const Page *)& pRecorder;

#ifdef WIN32
#pragma warning(disable:4310)
#endif


void RecorderScaleX::Load()
{
    static const uint8 values[RecorderScaleX::Size] =
    {
        BIN_U8(01010110),  // -V2501  // 100ms  
        BIN_U8(01010111),  // -V2501  // 200ms  
        BIN_U8(01011001),  // -V2501  // 500ms  
        BIN_U8(01011010),  // -V2501  // 1s     
        BIN_U8(01011011),  // -V2501  // 2s     
        BIN_U8(01011101),  // -V2501  // 5s     
        BIN_U8(01011110)   // -V2501  // 10s
    };

    FSMC::WriteToFPGA8(WR::TBASE, values[RecorderScaleX::Current().value]);

    if (Recorder::IsRunning())
    {
        Recorder::Stop();
        Recorder::Start();
    }
}


void RecorderScaleX::Change(int delta)
{
    if (Recorder::IsRunning())
    {
        return;
    }

    if (delta > 0)
    {
        ::Math::LimitationIncrease<uint8>((uint8 *)(&set.rec.scaleX), (uint8)(RecorderScaleX::Size - 1));
    }
    else
    {
        ::Math::LimitationDecrease<uint8>((uint8 *)(&set.rec.scaleX), 0);
    }

    Load();
}


RecorderScaleX &RecorderScaleX::Current()
{
    return set.rec.scaleX;
}


pString RecorderScaleX::ToString() const
{
    static const struct StructScaleX
    {
        const char *name;
        StructScaleX(pString nRU)
        {
            name = nRU;
        };
    }
    scales[RecorderScaleX::Size] =
    {
        StructScaleX("0.1\x10ñ"),
        StructScaleX("0.2\x10ñ"),
        StructScaleX("0.5\x10ñ"),
        StructScaleX("1\x10ñ"),
        StructScaleX("2\x10ñ"),
        StructScaleX("5\x10ñ"),
        StructScaleX("10\x10ñ")
    };

    return scales[value].name;
}


uint RecorderScaleX::BytesToSec() const
{
    static const struct StructBytes
    {
        uint value;
        StructBytes(uint v) : value(v) {};
    }
    bytes[RecorderScaleX::Size] =
    {
        800,
        400,
        160,
        80,
        40,
        16,
        8
    };

    return bytes[value].value;
}


uint RecorderScaleX::TimeForPointMS() const
{
    static const struct StructTime
    {
        uint value;
        StructTime(uint v) : value(v) {};
    }
    bytes[RecorderScaleX::Size] =
    {
        5,
        10,
        25,
        50,
        100,
        250,
        500
    };

    return bytes[value].value;
}
