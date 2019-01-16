#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "FPGA/FPGA_Settings.h"
#include "Settings/Settings.h"
#endif


using namespace FPGA::Settings;
using namespace Osci::Settings;
using namespace Recorder::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const Settings Settings::defaultSettings =
{
    /* size                       */    0,
    /* crc32                      */    0,

    // Меню ДИСПЛЕЙ

    /* disp_modeDrawSignal;       */    Display::ModeDrawSignal::Lines,
    /* disp_thickness;            */    Display::ThicknessSignal::_1,
    /* disp_background;           */    Display::Background::Black,
    /* disp_showStringNavigation; */    Display::ShowStrNavi::Temp,
    /* disp_ENumMinMax;           */    Display::ENumMinMax::_1,
    /* disp_modeAveraging;        */    Display::ModeAveraging::Accurately,
    /* disp_ENumAverage;          */    Display::ENumAverage::_1,
    /* disp_ENumAccum;            */    Display::ENumAccum::_1,
    /* disp_modeAccumulation;     */    Display::ModeAccumulation::NoReset,
    /* disp_ENumSmoothing;        */    Display::ENumSmoothing::Disable,
    /* disp_ENumSignalsInSec;     */    Display::ENumSignalsInSec::_25,
    /* disp_typeGrid;             */    Grid::Type::_1,
    /* disp_brightnessGrid;       */    50,
    /* disp_linkingRShift;        */    Display::LinkingRShift::Position,
    /* disp_brightness;           */    100,
    /* disp_timeShowLevels;       */    5,
    /* disp_timeMessages;         */    5,
    /* disp_altMarkers;           */    Display::AltMarkers::AutoHide,
    /* disp_menuAutoHide;         */    Menu::AutoHide::None,
    /* disp_shiftInMemory;        */    370,                       // Это соответствует 
    /* disp_lastAffectedChannel;  */    Chan::A,

    // Меню КАНАЛ 1, КАНАЛ 2

    /* chan_shift[Chan::Number];    */     {RShift::ZERO, RShift::ZERO},
    /* chan_range[Chan::Number];    */     {Range::_500mV, Range::_500mV},
    /* chan_couple[Chan::Number];   */     {ModeCouple::DC, ModeCouple::DC},
    /* chan_enabled[Chan::Number];  */     {true, true},
    /* chan_balanceShiftADC[2];     */     {0, 0},
    /* chan_bandwidth[2];           */     {Bandwidth::Full, Bandwidth::Full},
    /* chan_inverse[2];             */     {false, false},
    /* chan_divider[2];             */     {Divider::_1, Divider::_1},
    /* chan_calibrationMode[2];     */     {CalibrationMode::Disable, CalibrationMode::Disable},

    // Меню СИНХРОНИЗАЦИЯ

    /* trig_source;             */      Trig::Source::A,
    /* trig_input;              */      Trig::Input::Full,
    /* trig_polarity;           */      Trig::Polarity::Rising,
    /* trig_lev[Chan::Number];  */      {Trig::ZERO, Trig::ZERO},
    /* trig_startMode;          */      Trig::StartMode::Auto,
    /* trig_modeFind;           */      Trig::ModeFind::Hand,
    /* trig_holdOff;            */      10,
    /* trig_holdOffEnable       */      false,

    // Меню РАЗВЁРТКА

    /* time_shift;          */          TShift(0),
    /* time_base;           */          TBase::_500us,
    /* time_peakDet;        */          PeakDetMode::Disabled,
    /* time_TPos;           */          TPos::Center,
    /* time_sampleType;     */          SampleType::Real,
    /* time_timeDivXPos;    */          FunctionTime::Time,
    /* time_linkingTShift;  */          LinkingTShift::Time,
    /* time_sampleTypeOld;  */          SampleType::Real,

    // Меню КУРСОРЫ

    /* curs_showCursors;             */ false,
    /* curs_lookMode[2];             */ {Cursors::LookMode::None, Cursors::LookMode::None},
    /* curs_showFreq;                */ true,
    /* curs_active;                  */ Cursors::Active::None,
    /* curs_source;                  */ Chan::A,
    /* curs_cntrlU[Chan::Number];    */ {Cursors::Control::Disable, Cursors::Control::Disable},
    /* curs_cntrlT[Chan::Number];    */ {Cursors::Control::Disable, Cursors::Control::Disable},
    /* curs_movement;                */ Cursors::Movement::Pixels,
    /* curs_deltaU100percents[2];    */ {80,  80},
    /* curs_deltaT100percents[2];    */ {120, 120},
    /* curs_posCurU[Chan::Number][2];*/ {
                                        { 60, 140 },        // Канал 1
                                        { 60, 140 }},       // Канал 2
                                        /* curs_posCurT[Chan::Number][2];*/ {
                                                                            { 80, 200 },        // Канал 1
                                                                            { 80, 200 }},       // Канал 2

                                                                                // Меню ПАМЯТЬ

                                                                                /* mem_enumPoints;                             */   ENumPointsFPGA::_1k,
                                                                                /* mem_modeBtnMemory;                          */   ModeBtnMemory::Menu,
                                                                                /* mem_modeWork;                               */   ModeWork::Dir,
                                                                                /* mem_flashAutoConnect;                       */   false,
                                                                                /* mem_indexCurSymbolNameMask;                 */   0,
                                                                                /* mem_modeSaveSignal;                         */   ModeSaveSignal::BMP,
                                                                                /* mem_fileName[MAX_SYMBOLS_IN_FILE_NAME];     */   {},
                                                                                /* mem_modeShowIntMem;                         */   ModeShowIntMem::Saved,
                                                                                /* mem_fileNamingMode;                         */   FileNamingMode::Mask,
                                                                                /* mem_fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; */   {},

                                                                                // Меню ИЗМЕРЕНИЯ

                                                                                /* meas_show;            */         false,
                                                                                /* meas_number;          */         Measure::OnDisplay::_1_5,
                                                                                /* meas_source;          */         Measure::Source::A_B,
                                                                                /* meas_modeViewSignals; */         Measure::ModeViewSignals::AsIs,
                                                                                /* meas_measures[15];    */
                                                                                {
                                                                                    Measure::Type::VoltageMax,     Measure::Type::VoltageMin,  Measure::Type::VoltagePic,      Measure::Type::VoltageMaxSteady,  Measure::Type::VoltageMinSteady,
                                                                                    Measure::Type::VoltageAverage, Measure::Type::VoltageAmpl, Measure::Type::VoltageRMS,      Measure::Type::VoltageVybrosPlus, Measure::Type::VoltageVybrosMinus,
                                                                                    Measure::Type::Period,         Measure::Type::Freq,        Measure::Type::TimeNarastaniya, Measure::Type::TimeSpada,         Measure::Type::DurationPlus
                                                                                },
    /* meas_marked;          */         Measure::Type::Freq,

    // Меню СЕРВИС

    /* serv_lang;           */          Language::RU,
    /* serv_colorScheme;    */          Color::Scheme::WhiteLetters,
    /* serv_recorder;       */          false,
    /* serv_calibratorMode; */          CalibratorMode::Freq,
    /* serv_soundEnable;    */          true,
    /* serv_soundVolume;    */          100,

    // Настройки математики

    /* math_modeDraw;      */           FuncModeDraw::Disable,
    /* math_enableFFT;     */           false,
    /* math_scaleFFT;      */           ScaleFFT::Log,
    /* math_sourceFFT;     */           SourceFFT::Both,
    /* math_windowFFT;     */           WindowFFT::Hamming,
    /* math_currentCursor; */           0,
    /* math_posCur[2];     */           { 50, 50 },
    /* math_FFTmaxDB;      */           FFTmaxDB::_60,
    /* math_function;      */           Function::Sum,
    /* math_modeRegSet;    */           ModeRegSet::Range,
    /* math_range;         */           Range::_1V,
    /* math_divider;       */           Divider::_1,
    /* math_rShift;        */           RShift::ZERO,

    // Настройки частотомера

    /* freq_enabled;       */           FrequencyCounter::Enabled::Off,
    /* freq_modeView       */           FrequencyCounter::ModeView::Frequency,
    /* freq_timeCounting;  */           FrequencyCounter::TimeCounting::_1s,
    /* freq_freqClc;       */           FrequencyCounter::FreqClc::_1MHz,
    /* freq_numberPeriods; */           FrequencyCounter::NumberPeriods::_10,

    // Меню ОТЛАДКА

    /* dbg_numStrings;          */      30,
    /* dbg_sizeFont;            */      8,
    /* dbg_modePauseConsole;    */      false,
    /* dbg_showAll;             */      false,
    /* dbg_showFlag;            */      false,
    /* dbg_showRShift[2];       */      { false, false },
    /* dbg_showTrigLev;         */      false,
    /* dbg_showRange[2];        */      { false, false },
    /* dbg_showChanParam[2];    */      { false, false },
    /* dbg_showTrigParam;       */      false,
    /* dbg_showTShift;          */      false,
    /* dbg_showTBase;           */      false,
    /* dbg_numMeasuresForGates; */      10,
    /* dbg_showRandInfo;        */      false,
    /* dbg_showRandStat;        */      false,
    /* dbg_timeCompensation;    */      0,
    /* dbg_pretriggered;        */      0,
    /* dbg_bandwidth[2];        */      {Bandwidth::Full, Bandwidth::Full},
    /* dbg_ShowStats;           */      false,
    /* dbg_Orientation;         */      Display::Orientation::Direct,

    // Настройки тестер-компонента

    /* test_control;  */                Tester::Control::Voltage,
    /* test_polarity; */                Tester::Polarity::Positive,
    /* test_stepU;    */                Tester::StepU::_100mV,
    /* test_stepI;    */                Tester::StepI::_4mA,
    /* test_smooth;   */                Tester::Smoothing::_1,
    /* test_viewMode  */                Tester::ViewMode::Lines,

    // Настройки меню

    /* menu_currentPage;                   */ 0,
    /* menu_show;                          */ false,
    /* menu_posActItem[Page_NumPages];     */ {},
    /* menu_currentSubPage[Page_NumPages]; */ {},

    // Несбрасываемые настройки калибровки

    /* nrst_balanceADC[Chan::Number];                */ {},
    /* nrst_balanceADCtype;                       */ BalanceADC::Disable,
    /* nrst_stretchADCtype;                       */ StretchADC::Disable,
    /* nrst_StretchADC[Chan::Number][3];             */ {},
    /* nrst_rShiftAddStable[Chan::Number][3];        */ {},
    /* nrst_numAveForRand;                        */ 0,
    /* nrst_numSmoothForRand;                     */ 0,
    /* nrst_rShiftAdd[Chan::Number][Range::Size][2]; */ {},
    /* nrst_correctionTime;                       */ 0,

    // Настройки мультиметра
    /* multi_avp */         Multimeter::AVP::Off,
    /* multi_meas */        Multimeter::Measure::VoltageDC,
    /* multi_rangeCV */     Multimeter::RangeDC::_2V,
    /* multi_rangeVV */     Multimeter::RangeAC::_2V,
    /* multi_rangeResist */ Multimeter::RangeResistance::_10M,

    // Настройки регистратора
    /* rec_viewAlways */    ViewAlways::InMode,
    /* rec_sourceChanA */   false,
    /* rec_sourceChanB */   false,
    /* rec_sourceSensor */  false,
    /* rec_storageRecord */ Storage::Flash,
    /* rec_storagePlay */   Storage::Flash,
    /* rec_axisMove */      Axis::X,
    /* rec_axisZoom */      Axis::X,

    /* dbg_gate_max */      0,
    /* dbg_gate_min */      0
};
