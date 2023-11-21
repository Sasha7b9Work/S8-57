#include "defines.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"


namespace S_MEAS
{
    // :MEASURE:DISPLAY
    static pCHAR FuncDisplay(pCHAR);
    static void HintDisplay(String *);
    static bool TestDisplay();

    // :MEASURE:SOURCE
    static pCHAR FuncSource(pCHAR);
    static void HintSource(String *);
    static bool TestSource();

    // :MEASURE:VALUE
    static pCHAR FuncValue(pCHAR);
    static void HintValue(String *);
    static bool TestValue();


    static pString display[] =
    {
        " OFF",
        " ON",
        ""
    };

    static void DisplayMeasures(int i)
    {
        S_MEAS_SHOW = (i == 1);
    }

    static pCHAR FuncDisplay(pCHAR buffer) //-V2506
    {
        SCPI_REQUEST(SCPI::SendAnswer(S_MEAS_SHOW ? " ON" : " OFF"));

        SCPI_PROCESS_ARRAY(display, DisplayMeasures(i));
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
        SCPI_REQUEST(SCPI::SendAnswer(source[S_MEAS_SOURCE]));

        SCPI_PROCESS_ARRAY(source, S_MEAS_SOURCE = static_cast<MeasuresSource::E>(i));
    }


    static pString values[] =
    {
        " NNNNNNN",     // None,
        " VMAX",        // VoltageMax,
        " VMIN",        // VoltageMin,
        " VPP",         // VoltagePic,
        " VMAXEST",     // VoltageMaxSteady,
        " VMINEST",     // VoltageMinSteady,
        " VAMPL",       // VoltageAmpl,
        " VAVE",        // VoltageAverage,
        " VRMS",        // VoltageRMS,
        " VOVER+",      // VoltageVybrosPlus,
        " VOVER-",      // VoltageVybrosMinus,
        " PERIOD",      // Period,
        " FREQUENCY",   // Freq,
        " TIMERISE",    // TimeNarastaniya,
        " TIMEFALL",    // TimeSpada,
        " WIDTHP",      // DurationPlus,
        " WIDTHN",      // DurationMinus,
        " DUTY+",       // SkvaznostPlus,
        " DUTY-",       // SkvaznostMinus,
        " DELAY+",      // DelayPlus,
        " DELAY-",      // DelayMinus,
        " PHASE+",      // PhazaPlus,
        " PHASE-",      // PhazaMinus,
        ""
    };

    static pCHAR FuncValue(pCHAR buffer) //-V2506
    {
        pCHAR end = nullptr;

        end = SCPI::BeginWith(buffer, " VMAXEST");

        SCPI_RUN_IF_END(AutoMeasuresSender::DesignateForSending(TypeMeasure::VoltageMaxSteady));

        end = SCPI::BeginWith(buffer, " VMINEST");

        SCPI_RUN_IF_END(AutoMeasuresSender::DesignateForSending(TypeMeasure::VoltageMinSteady));

        end = SCPI::BeginWith(buffer, " TIMERISE");

        SCPI_RUN_IF_END(AutoMeasuresSender::DesignateForSending(TypeMeasure::TimeNarastaniya));

        SCPI_PROCESS_ARRAY(values, AutoMeasuresSender::DesignateForSending(static_cast<TypeMeasure::E>(i)));
    }


    static void HintDisplay(String *)
    {

    }


    static void HintSource(String *)
    {

    }


    static void HintValue(String *)
    {

    }


    static bool TestDisplay()
    {
        return false;
    }


    static bool TestSource()
    {
        return false;
    }


    static bool TestValue()
    {
        return false;
    }
}


const StructSCPI SCPI::measures[] =
{
    SCPI_LEAF(":DISPLAY", S_MEAS::FuncDisplay, S_MEAS::TestDisplay, "", S_MEAS::HintDisplay),
    SCPI_LEAF(":SOURCE",  S_MEAS::FuncSource,  S_MEAS::TestSource,  "", S_MEAS::HintSource),
    SCPI_LEAF(":VALUE",   S_MEAS::FuncValue,   S_MEAS::TestValue,   "", S_MEAS::HintValue),
    SCPI_EMPTY()
};
