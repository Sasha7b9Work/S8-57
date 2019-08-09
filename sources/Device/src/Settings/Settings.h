#pragma once
#include "SettingsMath.h"
#include "SettingsMemory.h"
#include "SettingsDebug.h"
#include "SettingsNRST.h"
#include "SettingsService.h"
#include "SettingsTime.h"
#include "Tester/Tester.h"
#include "Osci/FrequencyCounter.h"
#include "Osci/Measurements/Cursors.h"
#include "Menu/Menu.h"
#include "Display/Grid_Types.h"
#include "Display/Colors.h"
#include "Osci/Measurements/Measures.h"
#include "Osci/Measurements/Measures_Settings.h"
#include "Multimeter/Multimeter.h"
#include "Recorder/Recorder_Settings.h"

#include "Osci/Measurements/Cursors_Settings.h"
#include "Tester/Tester_Settings.h"
#include "Multimeter/Multimeter_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_SIZE                    (set.size)
#define SET_CRC32                   (set.crc32)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TRIG_MODE                   (set.trig_mode)
#define TRIG_HOLDOFF                (set.trig_holdOff)
#define TRIG_HOLDOFF_ENABLED        (set.trig_holdOffEnabled)

/// Возвращает позицию активного пункта на странице Page::Name.
#define MENU_POS_ACT_ITEM(name)     (set.menu_posActItem[name])

#define SET_DIVIDER(ch)             (divider[ch])
#define SET_DIVIDER_IS_1(ch)        (SET_DIVIDER(ch) == 0)
#define SET_DIVIDER_IS_10(ch)       (SET_DIVIDER(ch) == 1)

#define SET_DIVIDER_ABS(ch)         ((divider[ch] == 0) ? 1 : 10)


#pragma pack(push, 1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Settings
{
public:
    static void Load(bool _default = false);
    static void Save();
    static void Reset();

    void SetData(uint8 data[1024]);
    /// Сохраняет значение перечислимого типа. Возвращает адрес первой свободной ячейки (туда надо записывать следующее значение)
    void SaveEnum16(uint8 name, uint8 value);

    uint                size;                               ///< Размер данной структуры в байтах
    uint                crc32;                              ///< \brief Контрольная сумма данной структуры с хранящимися в ней настройками

    // Меню ДИСПЛЕЙ
                                                                ///< вычисляется от первого байта, следующего за этим полем
    Display::ModeDrawSignal::E      disp_modeDrawSignal;
    //Display::ThicknessSignal::E     disp_thickness;             ///< Толщина линии, которой рисуется сигнал
    Display::Background::E          disp_background;            ///< Цвет фона.
    Display::ShowStrNavi::E         disp_showStringNavigation;  ///< Режим показа строки навигации меню.
    Display::ENumMinMax::E          disp_ENumMinMax;            ///< Перечисление количества измерений для определения минимумов и масимумов.
    uint8                           not_using0;
    Display::ENumAverage::E         disp_ENumAverage;           ///< Число усреднений сигнала.
    Display::ENumAccum::E           disp_ENumAccum;             ///< Число накоплений сигнала на экране.
    Display::ModeAccumulation::E    disp_modeAccumulation;      ///< Режим накопления сигналов.
    Display::ENumSmoothing          disp_ENumSmoothing;         ///< Перечисление количества точек для скользящего фильтра.
    Display::ENumSignalsInSec       disp_ENumSignalsInSec;      ///< Перечисление числа считываний сигнала в секунда.
    Grid::Type::E                   disp_typeGrid;              ///< Тип сетки
    int                             disp_brightnessGrid;        ///< Яркость сетки от 0 до 100.
    Display::LinkingRShift::E       disp_linkingRShift;         ///< Тип привязки к смещению по вертикали.
    int16                           disp_brightness;            ///< Яркость дисплея.
    int16                           disp_timeShowLevels;        ///< Время, в течение которого нужно показывать уровин смещения.
    int16                           disp_timeMessages;          ///< Время в секундах, на которое сообщение остаётся на экрне.
    Display::AltMarkers::E          disp_altMarkers;            ///< Режим отображения дополнительных боковых маркеров смещений.
    Menu::AutoHide::E               disp_menuAutoHide;          ///< Через сколько времени после последнего нажатия клавиши прятать меню.
    int16                           disp_shiftInMemory;         ///< \brief Показывает смещение левого края стеки относительно нулевого байта памяти в 
                                                                ///< байтах. Т.е. для пикового детектора будет в два раза больше количества точек на экране.
    Chan::E                         disp_lastAffectedChannel;   ///< \brief Последний управляемый канал. Используется для правильного вывода сигналов
                                                                ///< на экран с наложением один поверх другого

    // Меню КАНАЛ 1, КАНАЛ 2

    uint16                         chan_shift[Chan::Size];      ///< Сдвиг канала по вертикали
    Osci::Settings::Range::E       chan_range[Chan::Size];      ///< Масштаб канала по вертикали
    FPGA::Settings::ModeCouple::E  chan_couple[Chan::Size];     ///< Связь по входу
    bool                           chan_enabled[Chan::Size];    ///< Включен/выключен канал
    int8                           chan_balanceShiftADC[2];     ///< Добавочное смещение для балансировки АЦП.
    FPGA::Settings::Bandwidth      chan_bandwidth[2];           ///< Ограничение полосы.
    bool                           chan_inverse[2];
    Divider::E                     chan_divider[2];             ///< Множитель.
    CalibrationMode::E             chan_calibrationMode[2];     ///< Режим калибровки.

    // Меню СИНХРОНИЗАЦИЯ

    Osci::Settings::Trig::Source::E    trig_source;
    Osci::Settings::Trig::Input::E     trig_input;
    Osci::Settings::Trig::Polarity::E  trig_polarity;
    uint16                             trig_lev[Chan::Size];
    Osci::Settings::Trig::StartMode::E trig_startMode;           ///< Режим запуска.
    Osci::Settings::Trig::ModeFind::E  trig_modeFind;            ///< Поиск синхронизации - вручную или автоматически.
    /// \todo разрядности trig_holdOff недостаточно
    int16                              trig_holdOff;             ///< Удержание синхронизации
    bool                               trig_holdOffEnabled;      ///< Включение удержания синхронизации

    // Меню РАЗВЁРТКА

    int                              time_shift;
    Osci::Settings::TBase::E         time_base;
    Osci::Settings::PeakDetMode::E   time_peakDet;
    Osci::Settings::TPos::E          time_TPos;
    Osci::Settings::SampleType::E    time_sampleType;
    Osci::Settings::FunctionTime::E  time_timeDivXPos;
    Osci::Settings::LinkingTShift::E time_linkingTShift;                 ///< Тип привязки смещения по горизонтали
    Osci::Settings::SampleType::E    time_sampleTypeOld;

    // Меню КУРСОРЫ

    bool                                     curs_showCursors;             ///< Показывать ли курсоры.
    Osci::Measurements::Cursors::LookMode::E curs_lookMode[2];             ///< Режимы слежения за курсорами для двух пар курсоров.
    bool                                     curs_showFreq;                ///< Установленное в true значение, что нужно показывать на экране 1/dT между курсорами.
    Osci::Measurements::Cursors::Active::E   curs_active;                  ///< Какие курсоры сейас активны.
    Chan::E                                  curs_source;                  ///< Источник - к какому каналу относятся курсоры.
    Osci::Measurements::Cursors::Control::E  curs_cntrlU[Chan::Size];      ///< Активные курсоры напряжения.
    Osci::Measurements::Cursors::Control::E  curs_cntrlT[Chan::Size];      ///< Активные курсоры времени.
    Osci::Measurements::Cursors::Movement::E curs_movement;                ///< Как перемещаться курсорам - по точкам или процентам.
    float                                    curs_deltaU100percents[2];    ///< Расстояние между курсорами напряжения для 100%, для обоих каналов.
    float                                    curs_deltaT100percents[2];    ///< Расстояние между курсорами времени для 100%, для обоих каналов.
    float                                    curs_posCurU[Chan::Size][2];  ///< Текущие позиции курсоров напряжения обоих каналов.
    float                                    curs_posCurT[Chan::Size][2];  ///< Текущие позиции курсоров времени обоих каналов.

    // Меню ПАМЯТЬ

#define MAX_SYMBOLS_IN_FILE_NAME 35
    Osci::Settings::Memory::ENumPointsFPGA::E   mem_enumPoints;                             ///< Число точек
    ModeBtnMemory::E                            mem_modeBtnMemory;
    ModeWork::E                                 mem_modeWork;                               ///< Режим работы.
    bool                                        mem_flashAutoConnect;                       ///< Если true, при подключении флешки автоматически выводится Файл-Менеджер.
    int8                                        mem_indexCurSymbolNameMask;                 ///< Индекс текущего символа в режиме задания маски или выбора имени.
    ModeSaveSignal::E                           mem_modeSaveSignal;                         ///< В каком виде сохранять сигнал.
    char                                        mem_fileName[MAX_SYMBOLS_IN_FILE_NAME];     ///< Имя файла для режима ручного задания.
    ModeShowIntMem::E                           mem_modeShowIntMem;                         ///< Что показывать в режиме ВНУТР ЗУ - считанный или записанный сигнал.
    FileNamingMode::E                           mem_fileNamingMode;                         ///< Режим именования файлов.
    char                                        mem_fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; ///< \brief Здесь маска для автоматического именования файлов.
                        ///< \details Правила именования.\n
                        /// \code
                        /// %y('\x42') - год, %m('\x43') - месяц, %d('\x44') - день, %H('\x45') - часы, %M('\x46') - минуты, %S('\x47') - секунды
                        /// %Nn('\x48''n') - порядковый номер, котрый занимает не менее n знакомест, например, 7 в %3N будет преобразовано в 007
                        /// Примеры
                        /// name_%4N_%y_%m_%d_%H_%M_%S будет генерировать файлы вида name_0043_2014_04_25_14_45_32
                        /// При этом обратите внимание, что если спецификатор %4N стоИт после временнЫх параметров, то, скорее всего, этот параметр 
                        /// будет всегда равен 0001, т.к. для определения номера просматриваются.
                        /// \endcode

    // Меню ИЗМЕРЕНИЯ

    bool                                                meas_show;              ///< Показывать ли измерения.
    Osci::Measurements::Settings::OnDisplay::E          meas_number;            ///< Сколько измерений выводить.
    Osci::Measurements::Settings::Source::E             meas_source;            ///< Для каких каналов выводить измерения.
    Osci::Measurements::Settings::ModeViewSignals::E    meas_modeViewSignals;   ///< Сжимать ли сигналы при выводе измерений.
    Osci::Measurements::Measure::Type::E                meas_measures[15];      ///< Выбранные для индикации измерения.
    Osci::Measurements::Measure::Type::E                meas_marked;            ///< Измерение, на которое нужно выводить маркеры.

    // Меню СЕРВИС

    uint8               notUsing;
    Color::Scheme::E    serv_colorScheme;
    bool                serv_notUsing;
    CalibratorMode::E   serv_calibratorMode;            ///< Режим работы калибратора.
    uint8               serv_soundVolume;               ///< Громкость звука
    bool                serv_showInfoVoltage;           ///< Показывать напряжение батареи на экране

    // Настройки математики

    FuncModeDraw::E          math_modeDraw;             ///< Раздельный или общий дисплей в режиме математической функции.
    bool                     math_enableFFT;
    ScaleFFT::E              math_scaleFFT;
    SourceFFT::E             math_sourceFFT;
    WindowFFT::E             math_windowFFT;
    uint8                    math_currentCursor;        ///< Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
    uint8                    math_posCur[2];            ///< Позиция курсора спектра. Изменяется 0...256.
    FFTmaxDB::E              math_FFTmaxDB;
    MathFunction::E          math_function;
    ModeRegSet::E            math_modeRegSet;           ///< Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    Osci::Settings::Range::E math_range;
    int8                     math_divider;
    uint16                   math_rShift;

    // Настройки частотомера

    FrequencyCounter::Enabled::E        freq_enabled;
    FrequencyCounter::ModeView::E       freq_modeView;      
    FrequencyCounter::TimeCounting::E   freq_timeCounting;  ///< Время счёта частоты.
    FrequencyCounter::FreqClc::E        freq_freqClc;       ///< Частота заполнения.
    FrequencyCounter::NumberPeriods::E  freq_numberPeriods; ///< Количество периодов.

    // Меню ОТЛАДКА

    int8                         dbg_showConsole;               ///< Показывать ли консоль
    int16                        dbg_numStrings;                ///< Число строк в консоли.
    int8                         dbg_sizeFont;                  ///< Размер шрифта консоли - 0 - 5, 1 - 8.
    bool                         dbg_modePauseConsole;          ///< Если true, то вывод в консоль останавливается нажатием кнопки ПУСК/СТОП.
    bool                         dbg_showAll;
    bool                         dbg_showFlag;
    bool                         dbg_showRShift[2];
    bool                         dbg_showTrigLev;
    bool                         dbg_showRange[2];
    bool                         dbg_showChanParam[2];
    bool                         dbg_showTrigParam;
    bool                         dbg_showTShift;
    bool                         dbg_showTBase;
    int16                        dbg_numMeasuresForGates;       ///< Число измерений для ворот.
    bool                         dbg_showRandInfo;              ///< Выводить информацию по рандомизатору - ворота и считанное значение.
    bool                         dbg_showRandStat;              ///< Выводить график статистики.
    int16                        dbg_timeCompensation;          ///< Дополнительное смещение по времени для данной развёртки режима рандомизатора.
    int16                        dbg_pretriggered;              ///< Регулируемая величина предзапуска для исследования рандомизатора.
    FPGA::Settings::Bandwidth::E dbg_bandwidth[2];              ///< \brief Здесь задаётся полоса, которая будет включаться при выборе в "КАНАЛ-Полоса" 
                                                                ///< значения "Полная".
    bool                         dbg_ShowStats;                 ///< Показывать статистику на экране (fps, например).
    Display::Orientation::E      dbg_Orientation;

    // Настройки тестер-компонента

    Tester::Settings::Control::E    test_control;
    Tester::Settings::Polarity::E   test_polarity;
    Tester::Settings::StepU::E      test_stepU;
    Tester::Settings::StepI::E      test_stepI;
    Display::ENumAverage::E         test_ave;                   ///< Количество усреднений
    Tester::Settings::ViewMode::E   test_viewMode;

    // Настройки меню

    bool                menu_show;                                                  ///< Если true, то нужно показывать текущую страницу
    int8                menu_posActItem[Page::Name::Number];                        ///< \brief Позиция активного пункта. bit7 == 1 - item is opened, 0x7f - нет активного пункта.
    int8                menu_currentSubPage[Page::Name::Number];                    ///< Номер текущей подстраницы.

    // Несбрасываемые настройки калибровки

    int16               nrst_balanceADC[Chan::Size];                                ///< Значение дополнительного смещения АЦП для ручной балансировки.
    BalanceADC::E       nrst_balanceADCtype;                                        ///< Тип балансировки.
    StretchADC::E       nrst_stretchADCtype;                                        ///< Тип растяжки канала.
    int16               nrst_StretchADC[Chan::Size][3];                             ///< \brief Поправочный коэффициент для ручного, калибровочного и
                        ///< отключенного режимов. Здесь хранится в целом виде, чтобы получить реальный коэффициент, нужно разделить на 1000 и
                        ///< прибавить единицу.
    int16               nrst_rShiftAddStable[Chan::Size][3];                        ///< Добавочное смещение для трёх самых чувствительных диапазонов. Задаётся единожды при настройке
    int16               nrst_numAveForRand;                                         ///< По скольким измерениям усреднять сигнал в режиме рандомизатора.
    int16               nrst_numSmoothForRand;                                      ///< Число точек для скользящего фильта в рандомизаторе.
    int16               nrst_rShiftAdd[Chan::Size][Osci::Settings::Range::Size][2]; ///< Добавочное смещение, которое пишется сюда при калибровке и балансировке
    int16               nrst_correctionTime;                                        ///< Коэффициент коррекции времени.

    // Настройки мультиметра
    Multimeter::Settings::AVP::E              multi_avp;
    Multimeter::Measure::E                    multi_meas;
    Multimeter::Settings::RangeDC::E          multi_rangeDC;
    Multimeter::Settings::RangeAC::E          multi_rangeAC;
    Multimeter::Settings::RangeCurrent::E     multi_rangeCurrentAC;     ///< Предел измерения переменного тока
    Multimeter::Settings::RangeCurrent::E     multi_rangeCurrentDC;     ///< Предел измерения постоянного тока
    Multimeter::Settings::RangeResistance::E  multi_rangeResist;

    // Настройки регистратора
    Recorder::Settings::ViewAlways::E  rec_viewAlways;       ///< Показывать ли всегда данные регистратора
    bool                               rec_sourceChanA;      ///< Включение записи канала 1
    bool                               rec_sourceChanB;      ///< Включение записи канала 2
    bool                               rec_sourceSensor;     ///< Включение записи показаний датчика
    Recorder::Settings::TypeStorage::E rec_storageRecord;    ///< Куда сохранять данные
    Recorder::Settings::TypeStorage::E rec_storagePlay;      ///< Откуда брать сохранённые данные
    Recorder::Settings::Axis::E        rec_axisMove;         ///< Текущая ось перемещения
    Recorder::Settings::Axis::E        rec_axisZoom;         ///< Текущая ось масштабирования
    Recorder::Settings::ScaleX         rec_scaleX;           ///< Установленный масштаб по времени
    Osci::Settings::Range::E           unused[2];            ///< Масштаб по вертикали в режиме регистратора


    /*  !!!!!!!!!!!!!!!!!!!!!!                                    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!!!!!!!!!!!!!!!!!!!!! Эти настройки задаются при наладке !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
        !!!!!!!!!!!!!!!!!!!!!!                                    !!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    */

    int16   dbg_enum_gate_max;                              ///< Ограничение ворот в рандомизаторе сверху
    int16   dbg_enum_gate_min;                              ///< Ограничение ворот в рандомизаторе снизу

    int8    addRShift[2][Osci::Settings::Range::Size];      ///< Добавочные смещения для 

    /// Настройки по умолчанию
    static const Settings defaultSettings;
};

#pragma pack(pop)

extern Settings set;

extern int8 divider[2]; // Делитель канала
