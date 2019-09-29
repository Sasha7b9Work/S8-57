#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Settings/Settings.h"


using namespace FPGA::Settings;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Settings Settings::defaultSettings =
{
    0,  // size
    0,  // crc32

    // Меню ДИСПЛЕЙ
    {
        ModeDrawSignal::Lines,                                      // modeDrawSignal
        Display::Background::Black,                                 // background
        Display::ENumMinMax::_1,                                    // ENumMinMax
        0,                                                          // not used
        Display::ENumAverage::_1,                                   // ENumAverage
        Display::ENumAccum::_1,                                     // ENumAccum
        Display::ModeAccumulation::NoReset,                         // modeAccumulation
        Display::ENumSmoothing(Display::ENumSmoothing::Disable),    // ENumSmoothing
        Display::ENumSignalsInSec(Display::ENumSignalsInSec::_25),  // ENumSignalsInSec
        Display::TypeGrid::_1,                                      // typeGrid
        50,                                                         // brightnessGrid
        Display::LinkingRShift::Position,                           // linkingRShift
        100,                                                        // brightness
        Menu::AutoHide::None,                                       // menuAutoHide
        370,                                                        // shiftInMemory
        Chan::A,                                                    // lastAffectedChannel
    },

    // Меню КАНАЛ 1, КАНАЛ 2
    {
        {RShift::ZERO, RShift::ZERO},                               // shift[Chan::Size]
        {Range::_500mV, Range::_500mV},                             // range[Chan::Size]
        {ModeCouple::DC, ModeCouple::DC},                           // couple[Chan::Size]
        {true, true},                                               // enabled[Chan::Size]
        {0, 0},                                                     // balanceShiftADC[2]
        {Bandwidth(Bandwidth::Full), Bandwidth(Bandwidth::Full)},   // bandwidth[2]
        {false, false},                                             // inverse[2]
        {Divider::_1, Divider::_1},                                 // divider[2]
        {CalibrationMode::Disable, CalibrationMode::Disable},       // calibrationMode[2]
    },

    // Меню СИНХРОНИЗАЦИЯ
    {
        Trig::Source::A,                        // source
        Trig::Input::Full,                      // input
        Trig::Polarity::Rising,                 // polarity
        {Trig::Level::ZERO, Trig::Level::ZERO}, // lev[Chan::Size]
        Trig::StartMode::Auto,                  // startMode
        Trig::ModeFind::Hand,                   // modeFind
        10,                                     // holdOff
        false,                                  // holdOffEnable
    },

    // Меню РАЗВЁРТКА
    {
        0,                      // time_shift
        TBase::_5ms,            // time_base
        PeakDetMode::Disabled,  // time_peakDet
        TPos::Center,           // time_TPos
        SampleType::Real,       // time_sampleType
        FunctionTime::Time,     // time_timeDivXPos
        LinkingTShift::Time,    // time_linkingTShift
        SampleType::Real,       // time_sampleTypeOld
    },

    // Меню КУРСОРЫ
    {
        false,                                                                                          // showCursors
        {Osci::Measurements::Cursors::LookMode::None, Osci::Measurements::Cursors::LookMode::None},     // lookMode[2]
        true,                                                                                           // showFreq
        Osci::Measurements::Cursors::Active::None,                                                      // active
        Chan::A,                                                                                        // source
        {Osci::Measurements::Cursors::Control::Disable, Osci::Measurements::Cursors::Control::Disable}, // cntrlU[Chan::Size]
        {Osci::Measurements::Cursors::Control::Disable, Osci::Measurements::Cursors::Control::Disable}, // cntrlT[Chan::Size]
        Osci::Measurements::Cursors::Movement::Pixels,                                                  // movement
        {80,  80},                                                                                      // deltaU100percents[2]
        {120, 120},                                                                                     // deltaT100percents[2]
        {                                                                                               // posCurU[Chan::Size][2]
        { 60, 140 },                                                                                    // Канал 1
        { 60, 140 }},                                                                                   // Канал 2
        {                                                                                               // posCurT[Chan::Size][2]
        { 80, 200 },                                                                                    // Канал 1
        { 80, 200 }},                                                                                   // Канал 2
    },

    // Меню ПАМЯТЬ
    {
        Osci::Settings::Memory::ENumPointsFPGA::_1k,    // enumPoints
        ModeBtnMemory::Menu,                            // modeBtnMemory
        ModeWork::Dir,                                  // modeWork
        false,                                          // flashAutoConnect
        0,                                              // indexCurSymbolNameMask
        ModeSaveSignal::BMP,                            // modeSaveSignal
        {},                                             // fileName[MAX_SYMBOLS_IN_FILE_NAME]
        ModeShowIntMem::Saved,                          // modeShowIntMem
        FileNamingMode::Mask,                           // fileNamingMode
        {},                                             // fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]
    },

    // Настройки математики
    {
        FuncModeDraw::Disable,      // modeDraw
        false,                      // enableFFT
        ScaleFFT::Log,              // scaleFFT
        SourceFFT::Both,            // sourceFFT
        WindowFFT::Hamming,         // windowFFT
        0,                          // currentCursor
        { 50, 50 },                 // posCur[2]
        FFTmaxDB::_60,              // FFTmaxDB
        MathFunction::Sum,          // function
        ModeRegSet::Range,          // modeRegSet
        Range::_1V,                 // range
        Divider::_1,                // divider
        RShift::ZERO,               // rShift
    },
    
    // Меню ИЗМЕРЕНИЯ
    {
        false,                              // show
        MeasuresOnDisplay::_1_5,            // number
        MeasuresSource::A_B,                // source
        MeasuresModeViewSignals::AsIs,      // modeViewSignals
        {                                   // measures[15]
            Measure::Type::VoltageMax,          Measure::Type::VoltageMin,  Measure::Type::VoltagePic,      Measure::Type::VoltageMaxSteady,
            Measure::Type::VoltageMinSteady,
            Measure::Type::VoltageAverage,      Measure::Type::VoltageAmpl, Measure::Type::VoltageRMS,      Measure::Type::VoltageVybrosPlus,
            Measure::Type::VoltageVybrosMinus,
            Measure::Type::Period,              Measure::Type::Freq,        Measure::Type::TimeNarastaniya, Measure::Type::TimeSpada,
            Measure::Type::DurationPlus
        },
        Measure::Type::Freq,                // marked
    },

    // Меню СЕРВИС
    {
        Color::Scheme::WhiteLetters,        // colorScheme
        CalibratorMode::Freq,               // calibratorMode
        0,                                  // soundVolume
        false                               // showInfoVoltage
    },
    
    // Настройки частотомера
    {
        FreqMeterEnabled::Off,         // enabled
        FreqMeterModeView::Frequency,  // modeView
        FreqMeterTimeCounting::_1s,    // timeCounting
        FreqMeterFreqClc::_1MHz,       // freqClc
        FreqMeterNumberPeriods::_10,   // numberPeriods
    },
    
    // Настройки тестер-компонента
    {
        TesterControl::Voltage,     // control
        TesterPolarity::Positive,   // polarity
        TesterStepU::_100mV,        // stepU
        TesterStepI::_4mA,          // stepI
        Display::ENumAverage::_1,   // smooth
        TesterViewMode::Lines,      // viewMode
    },
    
    // Настройки меню
    {
        false,                          // menu_show;
        {                               // menu_posActItem[Page_NumPages];    
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f
        },
        {},                             // menu_currentSubPage[Page_NumPages];
    },
    
    // Несбрасываемые настройки калибровки
    {
        {},                             // nrst_balanceADC[Chan::Size]
        BalanceADC::Disable,            // nrst_balanceADCtype
        StretchADC::Disable,            // nrst_stretchADCtype
        {},                             // nrst_StretchADC[Chan::Size][3]
        {},                             // nrst_rShiftAddStable[Chan::Size][3]
        0,                              // nrst_numAveForRand
        0,                              // nrst_numSmoothForRand
        {},                             // nrst_rShiftAdd[Chan::Size][Range::Size][2]
        0,                              // nrst_correctionTime
    },
    
    // Настройки мультиметра
    {
        MultimeterAVP::Off,                 // avp
        Multimeter::Measure::VoltageDC,     // meas
        MultimeterRangeDC::_2V,             // rangeCV
        MultimeterRangeAC::_2V,             // rangeVV
        MultimeterRangeCurrent::_2A,        // rangeCurrentAC
        MultimeterRangeCurrent::_2A,        // rangeCurrentDC
        MultimeterRangeResistance::_10M,    // rangeResist
    },
    
    // Настройки регистратора
    {
        true,                                   // sourceChanA
        true,                                   // sourceChanB
        false,                                  // sourceSensor
        RecorderTypeStorage::RAM,               // storageRecord
        RecorderTypeStorage::RAM,               // storagePlay
        RecorderAxis::X,                        // axisMove
        RecorderAxis::X,                        // axisZoom
        RecorderScaleX(RecorderScaleX::_100ms), // scaleX
        0,                                      // currentCursor
        { Range::_1V, Range::_1V },             // range
    },

    // Меню ОТЛАДКА
    {
        true,                                   // showConsole
        30,                                     // numStrings
        8,                                      // sizeFont
        false,                                  // modePauseConsole
        false,                                  // showAll
        false,                                  // showFlag
        { false, false },                       // showRShift[2]
        false,                                  // showTrigLev
        { false, false },                       // showRange[2]
        { false, false },                       // showChanParam[2]
        false,                                  // showTrigParam
        false,                                  // showTShift
        false,                                  // showTBase
        10,                                     // numMeasuresForGates
        false,                                  // showRandInfo
        false,                                  // showRandStat
        0,                                      // timeCompensation
        0,                                      // pretriggered
        { Bandwidth::Full, Bandwidth::Full },   // bandwidth[2]
        false,                                  // ShowStats
        0,                                      // gate_max
        0,                                      // gate_min
        {
            {0}, {0}
        }
    }
};
