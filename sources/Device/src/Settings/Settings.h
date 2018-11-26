#pragma once
#include "SettingsMath.h"
#include "SettingsMemory.h"
#include "SettingsDebug.h"
#include "SettingsNRST.h"
#include "SettingsService.h"
#include "Tester/Tester.h"
#include "Osci/Cursors.h"
#include "Osci/FrequencyCounter.h"
#include "Menu/Menu.h"
#include "Osci/Grid.h"
#include "Display/Colors.h"
#include "Multimeter/Multimeter.h"
#include "Utils/Measure.h"
#include "Recorder/Recorder.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define SET_SIZE                    (set.size)
#define SET_CRC32                   (set.crc32)


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define IN_RANDOMIZE_MODE           (SET_TBASE < TBase::_100ns)
#define CURRENT_PAGE                (set.menu_currentPage)

#define TRIG_MODE                   (set.trig_mode)

/// Возвращает позицию активного пункта на странице Page::Name.
#define MENU_POS_ACT_ITEM(name)     (set.menu_posActItem[name])

#pragma pack(push, 1)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Settings
{
public:
    static void Load(bool _default = false);
    static void Save();
    static void Reset();
    /// Сбросить цвета на значения по умолчанию
    static void ResetColors();
    /// Возвращает указатель на настройки по умолчанию
    static Settings &GetDefault();
    /// Преобразовать настройки в массив для сохранения в энергонезависимую память
    void TransformForSave(uint8 data[1024]);
    /// Преобразовать загруженные из энергонезависимой памяти настройки в нормальный вид
    static void TransformFromLoad(uint8 data[1024]);

    void SetData(uint8 data[1024]);
    /// Сохраняет значение перечислимого типа. Возвращает адрес первой свободной ячейки (туда надо записывать следующее значение)
    void SaveEnum16(uint8 name, uint8 value);

    uint                size;                               ///< Размер данной структуры в байтах
    uint                crc32;                              ///< \brief Контрольная сумма данной структуры с хранящимися в ней настройками

    // Меню ДИСПЛЕЙ
                                                                ///< вычисляется от первого байта, следующего за этим полем
    Display::ModeDrawSignal::E      disp_modeDrawSignal;
    Display::ThicknessSignal::E     disp_thickness;             ///< Толщина линии, которой рисуется сигнал
    Display::Background::E          disp_background;            ///< Цвет фона.
    Display::ShowStrNavi::E         disp_showStringNavigation;  ///< Режим показа строки навигации меню.
    Display::ENumMinMax::E          disp_ENumMinMax;            ///< Перечисление количества измерений для определения минимумов и масимумов.
    Display::ModeAveraging::E       disp_modeAveraging;         ///< Тип усреднений по измерениям.
    Display::ENumAverage::E         disp_ENumAverage;           ///< Число усреднений сигнала.
    Display::ENumAccum::E           disp_ENumAccum;             ///< Число накоплений сигнала на экране.
    Display::ModeAccumulation::E    disp_modeAccumulation;      ///< Режим накопления сигналов.
    Display::ENumSmoothing::E       disp_ENumSmoothing;         ///< Перечисление количества точек для скользящего фильтра.
    Display::ENumSignalsInSec::E    disp_ENumSignalsInSec;      ///< Перечисление числа считываний сигнала в секунда.
    Grid::Type::E                   disp_typeGrid;              ///< Тип сетки
    int16                           disp_brightnessGrid;        ///< Яркость сетки от 0 до 100.
    Display::LinkingRShift::E       disp_linkingRShift;         ///< Тип привязки к смещению по вертикали.
    int16                           disp_brightness;            ///< Яркость дисплея.
    int16                           disp_timeShowLevels;        ///< Время, в течение которого нужно показывать уровин смещения.
    int16                           disp_timeMessages;          ///< Время в секундах, на которое сообщение остаётся на экрне.
    Display::AltMarkers::E          disp_altMarkers;            ///< Режим отображения дополнительных боковых маркеров смещений.
    Menu::AutoHide::E               disp_menuAutoHide;          ///< Через сколько времени после последнего нажатия клавиши прятать меню.
    int16                           disp_shiftInMemory;         ///< \brief Показывает смещение левого края стеки относительно нулевого байта памяти в 
                                                                ///< байтах. Т.е. для пикового детектора будет в два раза больше количества точек на экране.
    Chan                            disp_lastAffectedChannel;   ///< \brief Последний управляемый канал. Используется для правильного вывода сигналов
                                                                ///< на экран с наложением один поверх другого

    // Меню КАНАЛ 1, КАНАЛ 2

    uint16              chan_shift[Chan::Number];           ///< Сдвиг канала по вертикали
    Range               chan_range[Chan::Number];           ///< Масштаб канала по вертикали
    ModeCouple          chan_couple[Chan::Number];          ///< Связь по входу
    bool                chan_enabled[Chan::Number];         ///< Включен/выключен канал
    int8                chan_balanceShiftADC[2];            ///< Добавочное смещение для балансировки АЦП.
    Bandwidth           chan_bandwidth[2];                  ///< Ограничение полосы.
    Resistance          chan_resistance[2];                 ///< Сопротивление входа.
    bool                chan_inverse[2];
    Divider             chan_divider[2];                    ///< Множитель.
    CalibrationMode     chan_calibrationMode[2];            ///< Режим калибровки.

    // Меню СИНХРОНИЗАЦИЯ

    Trig::Source        trig_source;
    Trig::Input         trig_input;
    Trig::Polarity      trig_polarity;
    uint16              trig_lev[Chan::Number];
    Trig::StartMode     trig_startMode;                     ///< Режим запуска.
    Trig::ModeFind      trig_modeFind;                      ///< Поиск синхронизации - вручную или автоматически.

    // Меню РАЗВЁРТКА

    TShift              time_shift;
    TBase               time_base;
    PeakDetMode         time_peakDet;
    TPos                time_TPos;
    SampleType          time_sampleType;
    FunctionTime        time_timeDivXPos;
    LinkingTShift       time_linkingTShift;                 ///< Тип привязки смещения по горизонтали
    SampleType          time_sampleTypeOld;

    // Меню КУРСОРЫ

    bool                curs_showCursors;                   ///< Показывать ли курсоры.
    Cursors::LookMode   curs_lookMode[2];                   ///< Режимы слежения за курсорами для двух пар курсоров.
    bool                curs_showFreq;                      ///< Установленное в true значение, что нужно показывать на экране 1/dT между курсорами.
    Cursors::Active     curs_active;                        ///< Какие курсоры сейас активны.
    Chan                curs_source;                        ///< Источник - к какому каналу относятся курсоры.
    Cursors::Control    curs_cntrlU[Chan::Number];          ///< Активные курсоры напряжения.
    Cursors::Control    curs_cntrlT[Chan::Number];          ///< Активные курсоры времени.
    Cursors::Movement   curs_movement;                      ///< Как перемещаться курсорам - по точкам или процентам.
    float               curs_deltaU100percents[2];          ///< Расстояние между курсорами напряжения для 100%, для обоих каналов.
    float               curs_deltaT100percents[2];          ///< Расстояние между курсорами времени для 100%, для обоих каналов.
    float               curs_posCurU[Chan::Number][2];      ///< Текущие позиции курсоров напряжения обоих каналов.
    float               curs_posCurT[Chan::Number][2];      ///< Текущие позиции курсоров времени обоих каналов.

    // Меню ПАМЯТЬ

#define MAX_SYMBOLS_IN_FILE_NAME 35
    ENumPointsFPGA      mem_enumPoints;                             ///< Число точек
    ModeBtnMemory       mem_modeBtnMemory;
    ModeWork            mem_modeWork;                               ///< Режим работы.
    bool                mem_flashAutoConnect;                       ///< Если true, при подключении флешки автоматически выводится Файл-Менеджер.
    int8                mem_indexCurSymbolNameMask;                 ///< Индекс текущего символа в режиме задания маски или выбора имени.
    ModeSaveSignal      mem_modeSaveSignal;                         ///< В каком виде сохранять сигнал.
    char                mem_fileName[MAX_SYMBOLS_IN_FILE_NAME];     ///< Имя файла для режима ручного задания.
    ModeShowIntMem      mem_modeShowIntMem;                         ///< Что показывать в режиме ВНУТР ЗУ - считанный или записанный сигнал.
    FileNamingMode      mem_fileNamingMode;                         ///< Режим именования файлов.
    char                mem_fileNameMask[MAX_SYMBOLS_IN_FILE_NAME]; ///< \brief Здесь маска для автоматического именования файлов.
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

    bool                     meas_show;                 ///< Показывать ли измерения.
    Measure::OnDisplay       meas_number;               ///< Сколько измерений выводить.
    Measure::Source          meas_source;               ///< Для каких каналов выводить измерения.
    Measure::ModeViewSignals meas_modeViewSignals;      ///< Сжимать ли сигналы при выводе измерений.
    Measure::Type            meas_measures[15];         ///< Выбранные для индикации измерения.
    Measure::Type            meas_marked;               ///< Измерение, на которое нужно выводить маркеры.

    // Меню СЕРВИС

    Language            serv_lang;
    Color::Scheme       serv_colorScheme;
    bool                serv_recorder;                  ///< Включён ли режим регистратора.
    CalibratorMode      serv_calibratorMode;            ///< Режим работы калибратора.
    bool                serv_soundEnable;               ///< Включены ли звуки.
    int16               serv_soundVolume;               ///< Громкость звука [0...100].

    // Настройки математики

    FuncModeDraw        math_modeDraw;                  ///< Раздельный или общий дисплей в режиме математической функции.
    bool                math_enableFFT;
    ScaleFFT            math_scaleFFT;
    SourceFFT           math_sourceFFT;
    WindowFFT           math_windowFFT;
    uint8               math_currentCursor;             ///< Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
    uint8               math_posCur[2];                 ///< Позиция курсора спектра. Изменяется 0...256.
    FFTmaxDB            math_FFTmaxDB;
    Function            math_function;
    ModeRegSet          math_modeRegSet;                ///< Функция ручки УСТАНОВКА - масштаб по времени или смещение по вертикали.
    Range               math_range;
    Divider             math_divider;
    uint16              math_rShift;

    // Настройки частотомера

    FrequencyCounter::Enabled       freq_enabled;
    FrequencyCounter::ModeView      freq_modeView;      
    FrequencyCounter::TimeCounting  freq_timeCounting;  ///< Время счёта частоты.
    FrequencyCounter::FreqClc       freq_freqClc;       ///< Частота заполнения.
    FrequencyCounter::NumberPeriods freq_numberPeriods; ///< Количество периодов.

    // Меню ОТЛАДКА

    int16               dbg_numStrings;                 ///< Число строк в консоли.
    int8                dbg_sizeFont;                   ///< Размер шрифта консоли - 0 - 5, 1 - 8.
    bool                dbg_modePauseConsole;           ///< Если true, то вывод в консоль останавливается нажатием кнопки ПУСК/СТОП.
    bool                dbg_showAll;
    bool                dbg_showFlag;
    bool                dbg_showRShift[2];
    bool                dbg_showTrigLev;
    bool                dbg_showRange[2];
    bool                dbg_showChanParam[2];
    bool                dbg_showTrigParam;
    bool                dbg_showTShift;
    bool                dbg_showTBase;
    int16               dbg_numMeasuresForGates;        ///< Число измерений для ворот.
    bool                dbg_showRandInfo;               ///< Выводить информацию по рандомизатору - ворота и считанное значение.
    bool                dbg_showRandStat;               ///< Выводить график статистики.
    int16               dbg_timeCompensation;           ///< Дополнительное смещение по времени для данной развёртки режима рандомизатора.
    int16               dbg_pretriggered;               ///< Регулируемая величина предзапуска для исследования рандомизатора.
    Bandwidth           dbg_bandwidth[2];               ///< \brief Здесь задаётся полоса, которая будет включаться при выборе в "КАНАЛ-Полоса" 
                                                        ///< значения "Полная".
    bool                  dbg_ShowStats;                ///< Показывать статистику на экране (fps, например).
    Display::Orientation  dbg_Orientation;

    // Настройки тестер-компонента

    Tester::Control     test_control;
    Tester::Polarity    test_polarity;
    Tester::StepU       test_stepU;
    Tester::StepI       test_stepI;
    Tester::Smoothing   test_smooth;                    ///< Количество сглаживаний
    Tester::ViewMode    test_viewMode;

    // Настройки меню

    const Page*         menu_currentPage;                               ///< Указатель на открытую страницу меню
    bool                menu_show;                                      ///< Если true, то нужно показывать текущую страницу
    int8                menu_posActItem[Page::Name::Number];            ///< \brief Позиция активного пункта. bit7 == 1 - item is opened, 0x7f - нет 
                                                                        ///< активного пункта.
    int8                menu_currentSubPage[Page::Name::Number];        ///< Номер текущей подстраницы.

    // Несбрасываемые настройки калибровки

    int16               nrst_balanceADC[Chan::Number];                  ///< Значение дополнительного смещения АЦП для ручной балансировки.
    BalanceADC          nrst_balanceADCtype;                            ///< Тип балансировки.
    StretchADC          nrst_stretchADCtype;                            ///< Тип растяжки канала.
    int16               nrst_StretchADC[Chan::Number][3];               ///< \brief Поправочный коэффициент для ручного, калибровочного и
                        ///< отключенного режимов. Здесь хранится в целом виде, чтобы получить реальный коэффициент, нужно разделить на 1000 и
                        ///< прибавить единицу.
    int16               nrst_rShiftAddStable[Chan::Number][3];          ///< Добавочное смещение для трёх самых чувствительных диапазонов. Задаётся единожды при настройке
    int16               nrst_numAveForRand;                             ///< По скольким измерениям усреднять сигнал в режиме рандомизатора.
    int16               nrst_numSmoothForRand;                          ///< Число точек для скользящего фильта в рандомизаторе.
    int16               nrst_rShiftAdd[Chan::Number][Range::Number][2]; ///< Добавочное смещение, которое пишется сюда при калибровке и балансировке
    int16               nrst_correctionTime;                            ///< Коэффициент коррекции времени.

    // Настройки мультиметра
    Multimeter::AVP             multi_avp;
    Multimeter::Measure         multi_meas;
    Multimeter::RangeDC         multi_rangeDC;
    Multimeter::RangeAC         multi_rangeAC;
    Multimeter::RangeResistance multi_rangeResist;

    // Настройки регистратора
    Recorder::ViewAlways        rec_viewAlways;         ///< Показывать ли всегда данные регистратора
    bool                        rec_sourceChanA;        ///< Включение записи канала 1
    bool                        rec_sourceChanB;        ///< Включение записи канала 2
    bool                        rec_sourceSensor;       ///< Включение записи показаний датчика
    Recorder::DataStorage       rec_storageRecord;      ///< Куда сохранять данные
    Recorder::DataStorage       rec_storagePlay;        ///< Откуда брать сохранённые данные
};

#pragma pack(pop)

extern Settings set;
