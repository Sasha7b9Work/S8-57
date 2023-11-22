#include "defines.h"
#include "Menu/Pages/Include/PageFFT.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"

namespace S_FFT
{
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

    static void EnableFFT(int i);
}


const StructSCPI SCPI::fft[] =
{
    SCPI_LEAF(":DATA?",   S_FFT::FuncData,    S_FFT::TestData,    "", S_FFT::HintData),
    SCPI_LEAF(":DISPLAY", S_FFT::FuncDisplay, S_FFT::TestDisplay, "", S_FFT::HintDisplay),
    SCPI_LEAF(":SCALE",   S_FFT::FuncScale,   S_FFT::TestScale,   "", S_FFT::HintScale),
    SCPI_LEAF(":SOURCE",  S_FFT::FuncSource,  S_FFT::TestSource,  "", S_FFT::HintSource),
    SCPI_LEAF(":WINDOW",  S_FFT::FuncWindow,  S_FFT::TestWindow,  "", S_FFT::HintWindow),
    SCPI_EMPTY()
};


pCHAR S_FFT::FuncData(pCHAR buffer)
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

void S_FFT::EnableFFT(int i)
{
    S_FFT_ENABLED = (i == 0);
}

pCHAR S_FFT::FuncDisplay(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(display[S_FFT_ENABLED ? 0 : 1]));

    SCPI_PROCESS_ARRAY(display, EnableFFT(i));
}


static pString scale[] =
{
    " LOG",
    " LINEAR",
    ""
};

pCHAR S_FFT::FuncScale(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(scale[S_FFT_SCALE]));

    SCPI_PROCESS_ARRAY(scale, S_FFT_SCALE = static_cast<ScaleFFT::E>(i));
}


static pString source[] =
{
    " 1",
    " 2",
    " BOTH",
    ""
};

pCHAR S_FFT::FuncSource(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(source[S_FFT_SOURCE]));

    SCPI_PROCESS_ARRAY(source, S_FFT_SOURCE = static_cast<SourceFFT::E>(i));
}


static pString window[] =
{
    " RECTANGLE",
    " HAMMING",
    " BLACKMAN",
    " HANN",
    ""
};

pCHAR S_FFT::FuncWindow(pCHAR buffer)
{
    SCPI_REQUEST(SCPI::SendAnswer(window[S_FFT_WINDOW]));

    SCPI_PROCESS_ARRAY(window, S_FFT_WINDOW = static_cast<WindowFFT::E>(i));
}


bool S_FFT::TestData()
{
    return false;
}


bool S_FFT::TestDisplay()
{
    return false;
}


bool S_FFT::TestScale()
{
    return false;
}


bool S_FFT::TestSource()
{
    return false;
}


bool S_FFT::TestWindow()
{
    return false;
}


void S_FFT::HintData(String *)
{

}


void S_FFT::HintDisplay(String *)
{

}


void S_FFT::HintScale(String *)
{

}


void S_FFT::HintSource(String *)
{

}


void S_FFT::HintWindow(String *)
{

}
