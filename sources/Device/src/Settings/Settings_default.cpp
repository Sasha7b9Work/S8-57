#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Settings/Settings.h"


using namespace FPGA::Settings;
using namespace Osci::Settings;
using namespace Recorder::Settings;

using Osci::Measurements::Measure;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Settings Settings::defaultSettings =
{
    0,  // size
    0,  // crc32

    // Меню ДИСПЛЕЙ

    Display::ModeDrawSignal::Lines,                             // disp_modeDrawSignal;      
//    Display::ThicknessSignal::_1,                             // disp_thickness;           
    Display::Background::Black,                                 // disp_background;          
    Display::ShowStrNavi::Temp,                                 // disp_showStringNavigation;
    Display::ENumMinMax::_1,                                    // disp_ENumMinMax;          
    0,                                                          // not used
    Display::ENumAverage::_1,                                   // disp_ENumAverage;         
    Display::ENumAccum::_1,                                     // disp_ENumAccum;           
    Display::ModeAccumulation::NoReset,                         // disp_modeAccumulation;    
    Display::ENumSmoothing(Display::ENumSmoothing::Disable),    // disp_ENumSmoothing;       
    Display::ENumSignalsInSec(Display::ENumSignalsInSec::_25),  // disp_ENumSignalsInSec;    
    Grid::Type::_1,                                             // disp_typeGrid;            
    50,                                                         // disp_brightnessGrid;      
    Display::LinkingRShift::Position,                           // disp_linkingRShift;       
    100,                                                        // disp_brightness;          
    5,                                                          // disp_timeShowLevels;      
    5,                                                          // disp_timeMessages;        
    Display::AltMarkers::AutoHide,                              // disp_altMarkers;          
    Menu::AutoHide::None,                                       // disp_menuAutoHide;
    370,                                                        // disp_shiftInMemory;
    Chan::A,                                                    // disp_lastAffectedChannel;

    // Меню КАНАЛ 1, КАНАЛ 2

    {RShift::ZERO, RShift::ZERO},                               // chan_shift[Chan::Size];  
    {Range::_500mV, Range::_500mV},                             // chan_range[Chan::Size];  
    {ModeCouple::DC, ModeCouple::DC},                           // chan_couple[Chan::Size]; 
    {true, true},                                               // chan_enabled[Chan::Size];
    {0, 0},                                                     // chan_balanceShiftADC[2];   
    {Bandwidth(Bandwidth::Full), Bandwidth(Bandwidth::Full)},   // chan_bandwidth[2];         
    {false, false},                                             // chan_inverse[2];           
    {Divider::_1, Divider::_1},                                 // chan_divider[2];           
    {CalibrationMode::Disable, CalibrationMode::Disable},       // chan_calibrationMode[2];   

    // Меню СИНХРОНИЗАЦИЯ

    Trig::Source::A,                        // trig_source;           
    Trig::Input::Full,                      // trig_input;            
    Trig::Polarity::Rising,                 // trig_polarity;         
    {Trig::Level::ZERO, Trig::Level::ZERO}, // trig_lev[Chan::Size];
    Trig::StartMode::Auto,                  // trig_startMode;        
    Trig::ModeFind::Hand,                   // trig_modeFind;         
    10,                                     // trig_holdOff;          
    false,                                  // trig_holdOffEnable     

    // Меню РАЗВЁРТКА

    0,                      // time_shift;         
    TBase::_5ms,            // time_base;         
    PeakDetMode::Disabled,  // time_peakDet;      
    TPos::Center,           // time_TPos;         
    SampleType::Real,       // time_sampleType;   
    FunctionTime::Time,     // time_timeDivXPos;  
    LinkingTShift::Time,    // time_linkingTShift;
    SampleType::Real,       // time_sampleTypeOld;

    // Меню КУРСОРЫ

    false,                                                                                          // curs_showCursors;
    {Osci::Measurements::Cursors::LookMode::None, Osci::Measurements::Cursors::LookMode::None},     // curs_lookMode[2];
    true,                                                                                           // curs_showFreq;
    Osci::Measurements::Cursors::Active::None,                                                      // curs_active;
    Chan::A,                                                                                        // curs_source;
    {Osci::Measurements::Cursors::Control::Disable, Osci::Measurements::Cursors::Control::Disable}, // curs_cntrlU[Chan::Size];
    {Osci::Measurements::Cursors::Control::Disable, Osci::Measurements::Cursors::Control::Disable}, // curs_cntrlT[Chan::Size];
    Osci::Measurements::Cursors::Movement::Pixels,                                                  // curs_movement;                
    {80,  80},                                              // curs_deltaU100percents[2];    
    {120, 120},                                             // curs_deltaT100percents[2];    
    {                                                       // curs_posCurU[Chan::Size][2];
    { 60, 140 },                                            // Канал 1
    { 60, 140 }},                                           // Канал 2
    {                                                       // curs_posCurT[Chan::Size][2];
    { 80, 200 },                                            // Канал 1
    { 80, 200 }},                                           // Канал 2

    // Меню ПАМЯТЬ

    Osci::Settings::Memory::ENumPointsFPGA::_1k,    // mem_enumPoints;                            
    ModeBtnMemory::Menu,                            // mem_modeBtnMemory;                         
    ModeWork::Dir,                                  // mem_modeWork;                              
    false,                                          // mem_flashAutoConnect;                      
    0,                                              // mem_indexCurSymbolNameMask;                
    ModeSaveSignal::BMP,                            // mem_modeSaveSignal;                        
    {},                                             // mem_fileName[MAX_SYMBOLS_IN_FILE_NAME];    
    ModeShowIntMem::Saved,                          // mem_modeShowIntMem;                        
    FileNamingMode::Mask,                           // mem_fileNamingMode;                        
    {},                                             // mem_fileNameMask[MAX_SYMBOLS_IN_FILE_NAME];
    
    // Меню ИЗМЕРЕНИЯ
    
    false,                                                  // meas_show;  
    Osci::Measurements::Settings::OnDisplay::_1_5,          // meas_number;
    Osci::Measurements::Settings::Source::A_B,              // meas_source;
    Osci::Measurements::Settings::ModeViewSignals::AsIs,    // meas_modeViewSignals; 
    {                                                       // meas_measures[15];
        Measure::Type::VoltageMax,          Measure::Type::VoltageMin,  Measure::Type::VoltagePic,      Measure::Type::VoltageMaxSteady,
        Measure::Type::VoltageMinSteady,
        Measure::Type::VoltageAverage,      Measure::Type::VoltageAmpl, Measure::Type::VoltageRMS,      Measure::Type::VoltageVybrosPlus,
        Measure::Type::VoltageVybrosMinus,
        Measure::Type::Period,              Measure::Type::Freq,        Measure::Type::TimeNarastaniya, Measure::Type::TimeSpada,
        Measure::Type::DurationPlus
    },
    Measure::Type::Freq,    // meas_marked;

    // Меню СЕРВИС

    0,                                  // notUsed
    Color::Scheme::WhiteLetters,        // serv_colorScheme
    false,                              // serv_recorder
    CalibratorMode::Freq,               // serv_calibratorMode
    false,                              // serv_soundEnable
    100,                                // serv_soundVolume
    false,                              // serv_showInfoVoltage
    
    // Настройки математики
    
    FuncModeDraw::Disable,      // math_modeDraw;     
    false,                      // math_enableFFT;    
    ScaleFFT::Log,              // math_scaleFFT;     
    SourceFFT::Both,            // math_sourceFFT;    
    WindowFFT::Hamming,         // math_windowFFT;    
    0,                          // math_currentCursor;
    { 50, 50 },                 // math_posCur[2];    
    FFTmaxDB::_60,              // math_FFTmaxDB;     
    Function::Sum,              // math_function;     
    ModeRegSet::Range,          // math_modeRegSet;   
    Range::_1V,                 // math_range;        
    Divider::_1,                // math_divider;      
    RShift::ZERO,               // math_rShift;       
    
    // Настройки частотомера
    
    FrequencyCounter::Enabled::Off,         // freq_enabled;      
    FrequencyCounter::ModeView::Frequency,  // freq_modeView      
    FrequencyCounter::TimeCounting::_1s,    // freq_timeCounting; 
    FrequencyCounter::FreqClc::_1MHz,       // freq_freqClc;      
    FrequencyCounter::NumberPeriods::_10,   // freq_numberPeriods;
    
    // Меню ОТЛАДКА

    false,                                  // dbg_showConsole
    30,                                     // dbg_numStrings;         
    8,                                      // dbg_sizeFont;           
    false,                                  // dbg_modePauseConsole;   
    false,                                  // dbg_showAll;            
    false,                                  // dbg_showFlag;           
    { false, false },                       // dbg_showRShift[2];      
    false,                                  // dbg_showTrigLev;        
    { false, false },                       // dbg_showRange[2];       
    { false, false },                       // dbg_showChanParam[2];   
    false,                                  // dbg_showTrigParam;      
    false,                                  // dbg_showTShift;         
    false,                                  // dbg_showTBase;          
    10,                                     // dbg_numMeasuresForGates;
    false,                                  // dbg_showRandInfo;       
    false,                                  // dbg_showRandStat;       
    0,                                      // dbg_timeCompensation;   
    0,                                      // dbg_pretriggered;       
    { Bandwidth::Full, Bandwidth::Full },   // dbg_bandwidth[2];       
    false,                                  // dbg_ShowStats;          
    Display::Orientation::Normal,           // dbg_Orientation;        
    
    // Настройки тестер-компонента
    
    Tester::Settings::Control::Voltage,       // test_control; 
    Tester::Settings::Polarity::Positive,     // test_polarity;
    Tester::Settings::StepU::_100mV,          // test_stepU;   
    Tester::Settings::StepI::_4mA,            // test_stepI;   
    Tester::Settings::Smoothing::_1,          // test_smooth;  
    Tester::Settings::ViewMode::Lines,        // test_viewMode 
    
    // Настройки меню
    
    false,                          // menu_show;                         
    {                               // menu_posActItem[Page_NumPages];    
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 
        0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f, 0x7f
    },                             
    {},                             // menu_currentSubPage[Page_NumPages];
    
    // Несбрасываемые настройки калибровки
    
    {},                             // nrst_balanceADC[Chan::Size];               
    BalanceADC::Disable,            // nrst_balanceADCtype;                         
    StretchADC::Disable,            // nrst_stretchADCtype;                         
    {},                             // nrst_StretchADC[Chan::Size][3];            
    {},                             // nrst_rShiftAddStable[Chan::Size][3];       
    0,                              // nrst_numAveForRand;                          
    0,                              // nrst_numSmoothForRand;                       
    {},                             // nrst_rShiftAdd[Chan::Size][Range::Size][2];
    0,                              // nrst_correctionTime;                         
    
    // Настройки мультиметра

    Multimeter::Settings::AVP::Off,                 // multi_avp
    Multimeter::Measure::VoltageDC,                 // multi_meas
    Multimeter::Settings::RangeDC::_2V,             // multi_rangeCV
    Multimeter::Settings::RangeAC::_2V,             // multi_rangeVV
    Multimeter::Settings::RangeCurrent::_2A,        // multi_rangeCurrentAC
    Multimeter::Settings::RangeCurrent::_2A,        // multi_rangeCurrentDC
    Multimeter::Settings::RangeResistance::_10M,    // multi_rangeResist
    
    // Настройки регистратора

    ViewAlways::InMode,                 // rec_viewAlways
    true,                               // rec_sourceChanA
    true,                               // rec_sourceChanB
    false,                              // rec_sourceSensor
    TypeStorage::Flash,                 // rec_storageRecord
    TypeStorage::Flash,                 // rec_storagePlay
    Axis::X,                            // rec_axisMove
    Axis::X,                            // rec_axisZoom
    ScaleX(ScaleX::_100ms),             // rec_scaleX
    { Range::_1V, Range::_1V },         // rec_range

    0,  // dbg_gate_max
    0,  // dbg_gate_min

    {
        {0}, {0}
    }
};
