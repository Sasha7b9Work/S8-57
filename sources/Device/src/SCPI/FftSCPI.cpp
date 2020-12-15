#include "defines.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"

// :FFT:DATA?
static pCHAR FuncData(pCHAR);
static void HintData(String *);
static bool TestData();

// :FFT:DISPLAY
static pCHAR FuncDisplay(pCHAR);
static void HintDisplay(String *);
static bool TestDisplay();

// :FFT:SCALE
static pCHAR FuncScale(pCHAR);
static void HintScale(String *);
static bool TestScale();

// :FFT:SOUCRE
static pCHAR FuncSource(pCHAR);
static void HintSource(String *);
static bool TestSource();

// :FFT:WINDOW
static pCHAR FuncWindow(pCHAR);
static void HintWindow(String *);
static bool TestWindow();


const StructSCPI SCPI::fft[] =
{
    SCPI_LEAF(":DATA?",   FuncData,    TestData,    "", HintData),
    SCPI_LEAF(":DISPLAY", FuncDisplay, TestDisplay, "", HintDisplay),
    SCPI_LEAF(":SCALE",   FuncScale,   TestScale,   "", HintScale),
    SCPI_LEAF(":SOURCE",  FuncSource,  TestSource,  "", HintSource),
    SCPI_LEAF(":WINDOW",  FuncWindow,  TestWindow,  "", HintWindow),
    SCPI_EMPTY()
};


static pCHAR FuncData(pCHAR buffer) //-V2506
{
    SCPI_PROLOG(buffer);

    SCPI::Sender::fft = true;

    SCPI_EPILOG(buffer);
}


static pString display[] =
{
    " ON",
    " OFF",
    ""
};

static void EnableFFT(int i)
{
    S_FFT_ENABLED = (i == 0);
}

static pCHAR FuncDisplay(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(display[S_FFT_ENABLED ? 0 : 1]));

    SCPI_PROCESS_ARRAY(display, EnableFFT(i)); //-V2563
}


static pString scale[] =
{
    " LOG",
    " LINEAR",
    ""
};

static pCHAR FuncScale(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(scale[S_FFT_SCALE]));

    SCPI_PROCESS_ARRAY(scale, S_FFT_SCALE = static_cast<ScaleFFT::E>(i)); //-V2563
}


static pString source[] =
{
    " 1",
    " 2",
    " BOTH",
    ""
};

static pCHAR FuncSource(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(source[S_FFT_SOURCE]));

    SCPI_PROCESS_ARRAY(source, S_FFT_SOURCE = static_cast<SourceFFT::E>(i)); //-V2563
}


static pString window[] =
{
    " RECTANGLE",
    " HAMMING",
    " BLACKMAN",
    " HANN",
    ""
};

static pCHAR FuncWindow(pCHAR buffer) //-V2506
{
    SCPI_REQUEST(SCPI::SendAnswer(window[S_FFT_WINDOW]));

    SCPI_PROCESS_ARRAY(window, S_FFT_WINDOW = static_cast<WindowFFT::E>(i)); //-V2563
}


static bool TestData()
{
    return false;
}


static bool TestDisplay()
{
    return false;
}


static bool TestScale()
{
    return false;
}


static bool TestSource()
{
    return false;
}


static bool TestWindow()
{
    return false;
}


static void HintData(String *)
{

}


static void HintDisplay(String *)
{

}


static void HintScale(String *)
{

}


static void HintSource(String *)
{

}


static void HintWindow(String *)
{

}
