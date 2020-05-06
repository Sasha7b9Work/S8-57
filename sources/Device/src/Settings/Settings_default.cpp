#include "defines.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"


const Settings Settings::defaultSettings =
{
    0,  // size
    0,  // crc32

    // Настройки меню
    {
        false,                                  // menu_show;
        {                                       // menu_posActItem[Page_NumPages];    
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f,
            0x7f, 0x7f
        },
        {},                                     // menu_currentSubPage[Page_NumPages];
        PageName::None
    },

    // Меню ДИСПЛЕЙ
    {
        DisplayMapping::Lines,              // modeDrawSignal
        ENumAverage::_1,                    // ENumAverage
        ENumAccum::_1,                      // ENumAccum
        ModeAccumulation::NoReset,          // modeAccumulation
        ENumSmoothing::Disable,             // ENumSmoothing
        ENumSignalsInSec::_25,              // ENumSignalsInSec
        TypeGrid::_1,                       // typeGrid
        100,                                // brightness
        MenuAutoHide::None,                 // menuAutoHide
        370,                                // shiftInMemory
        ChanA                             // lastAffectedChannel
    },

    // Меню КАНАЛ 1, КАНАЛ 2
    {
        {
            0,                              // shift
            Range::_500mV,                  // range
            ModeCouple::DC,                 // couple
            true,                           // enabled
            Bandwidth::Full,                // bandwidth
            false,                          // inverse
            Divider::_1                     // divider
        },
        {
            0,                              // shift
            Range::_500mV,                  // range
            ModeCouple::DC,                 // couple
            true,                           // enabled
            Bandwidth::Full,                // bandwidth
            false,                          // inverse
            Divider::_1                     // divider
        }
    },

    // Меню СИНХРОНИЗАЦИЯ
    {
        ChanA,                            // source
        TrigInput::Full,                    // input
        TrigPolarity::Rising,               // polarity
        { 0, 0},                            // lev[Chan::Count]
        TrigStartMode::Auto,                // startMode
        TrigModeFind::Hand,                 // modeFind
        10,                                 // holdOff
        false                               // holdOffEnable
    },

    // Меню РАЗВЁРТКА
    {
        0,                                  // shift
        TBase::_5ms,                        // base
        PeakDetMode::Disabled,              // peakDet
        TPos::Center,                       // TPos
        SampleType::Real,                   // sampleType
        FunctionTime::Time,                 // timeDivXPos
        LinkingTShift::Time,                // linkingTShift
        SampleType::Real                    // sampleTypeOld
    },

    // Меню ПАМЯТЬ
    {
        ENumPointsFPGA::_1k,        // enumPoints
        ModeBtnMemory::Menu,        // modeBtnMemory
        TypeSignalROM::Recorded,    // typeSignalROM
        ModeWork::Dir,              // modeWork
        false,                      // flashAutoConnect
        0,                          // indexCurSymbolNameMask
        ModeSaveSignal::BMP,        // modeSaveSignal
        {},                         // fileName[MAX_SYMBOLS_IN_FILE_NAME]
        ModeShowIntMem::Saved,      // modeShowIntMem
        FileNamingMode::Mask,       // fileNamingMode
        {}                          // fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]
    },

    // Курсорные измерения
    {
        false,                                                // showCursors
        {CursorsLookMode::None, CursorsLookMode::None},       // lookMode[2]
        true,                                                 // showFreq
        CursorsActive::None,                                  // active
        ChanA,                                              // source
        {CursorsControl::Disabled, CursorsControl::Disabled}, // cntrlU[Chan::Count]
        {CursorsControl::Disabled, CursorsControl::Disabled}, // cntrlT[Chan::Count]
        CursorsMovement::Pixels,                              // movement
        {80,  80},                                            // deltaU100percents[2]
        {120, 120},                                           // deltaT100percents[2]
        {                                                     // posCurU[Chan::Count][2]
        { 60, 140 },                                          // Канал 1
        { 60, 140 }},                                         // Канал 2
        {                                                     // posCurT[Chan::Count][2]
        { 80, 200 },                                          // Канал 1
        { 80, 200 }}                                          // Канал 2
    },

    // Меню ИЗМЕРЕНИЯ
    {
        false,                              // show
        MeasuresOnDisplay::_1_5,            // number
        MeasuresSource::A_B,                // source
        MeasuresModeViewSignals::AsIs,      // modeViewSignals
        {                                   // measures[15]
            TypeMeasure::VoltageMax,          TypeMeasure::VoltageMin,  TypeMeasure::VoltagePic,      TypeMeasure::VoltageMaxSteady,
            TypeMeasure::VoltageMinSteady,
            TypeMeasure::VoltageAverage,      TypeMeasure::VoltageAmpl, TypeMeasure::VoltageRMS,      TypeMeasure::VoltageVybrosPlus,
            TypeMeasure::VoltageVybrosMinus,
            TypeMeasure::Period,              TypeMeasure::Freq,        TypeMeasure::TimeNarastaniya, TypeMeasure::TimeSpada,
            TypeMeasure::DurationPlus
        },
        TypeMeasure::Freq                 // marked
    },

    // Настройки математики
    {
        FuncModeDraw::Disabled,     // modeDraw
        MathFunction::Sum,          // function
        ModeRegSet::Range,          // modeRegSet
        Divider::_1,                // divider
        0,
        Range::_500mV
    },

    // FFT
    {
        false,                      // enabled
        { 50, 50 },                 // posCur[2]
        ScaleFFT::Log,              // scale
        SourceFFT::Both,            // source
        WindowFFT::Hamming,         // window
        MaxDBFFT::_60,              // maxDB
        0                           // cursor
    },

    // Настройки частотомера
    {
        0,                              // enabled
        FreqMeter::ModeView::Frequency, // modeView
        FreqMeter::TimeCounting::_1s,   // timeCounting
        FreqMeter::FreqClc::_1MHz,      // freqClc
        FreqMeter::NumberPeriods::_10   // numberPeriods
    },

    // Настройки тестер-компонента
    {
        Tester::Control::Voltage,       // control
        Tester::Polarity::Positive,     // polarity
        Tester::StepU::_600mV,          // stepU
        Tester::StepI::_4uA,            // stepI
        ENumAverage::_1,                // smooth
        Tester::ViewMode::Lines         // viewMode
    },

    // Настройки мультиметра
    {
        ModeAVP::Off,                   // avp
        MultimeterMeasure::VoltageDC,   // meas
        RangeVoltageDC::_2V,            // rangeCV
        RangeVoltageAC::_2V,            // rangeVV
        RangeCurrentAC::_2A,            // rangeCurrentAC
        RangeCurrentDC::_2A,            // rangeCurrentDC
        RangeResistance::_10M,          // rangeResist
        false                           // showAlways
    },

    // Настройки регистратора
    {
        true,                           // sourceChanA
        true,                           // sourceChanB
        false,                          // sourceSensor
        Recorder::TypeMemory::RAM,      // typeMemory
        Recorder::Axis::X,              // axisMove
        Recorder::Axis::X,              // axisZoom
        Recorder::ScaleX::_100ms,       // scaleX
        0                               // currentCursor
    },

    // Меню СЕРВИС
    {
        Calibrator::Mode::Freq,         // calibratorMode
        0,                              // soundVolume
        false                           // showInfoVoltage
    },
        
    // Меню ОТЛАДКА
    {
        true,                       // showConsole
        30,                         // numStrings
        false,                      // showAll
        false,                      // showFlag
        { false, false },           // showRShift[2]
        false,                      // showTrigLev
        { false, false },           // showRange[2]
        { false, false },           // showChanParam[2]
        false,                      // showTrigParam
        false,                      // showTShift
        false,                      // showTBase
        10,                         // numMeasuresForGates
        false,                      // showRandInfo
        false,                      // showRandStat
        0,                          // timeCompensation
        0,                          // pretriggered
        false,                      // ShowStats
        false,                      // runTest
        false                       // showBattery
    },
    0
};
