#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


/// \brief Тип балансировки АЦП каналов.
/// Дело в том, что уровни АЦП не совпадают из-за отличия характеристик ( ? ), поэтому мы вводим дополнительное смещение для одного из АЦП канала.
struct BalanceADC
{
    enum E
    {
        Disable,     ///< Балансировка выключена.
        Settings,    ///< Используются значения балансировки, которые получены автоматически.
        Hand         ///< Используются значения балансировки, заданные вручную.
    } value;
    explicit BalanceADC(E v) : value(v) {};
};

/// Тип растяжки АЦП
struct StretchADC
{
    enum E
    {
        Disable,
        Real,
        Hand,
        Count
    } value;
    explicit StretchADC(E v) : value(v) {};
};


struct SettingsNRST
{
    int16           numAveForRand;                              ///< По скольким измерениям усреднять сигнал в режиме рандомизатора.
    int16           numSmoothForRand;                           ///< Число точек для скользящего фильта в рандомизаторе.
    int16           correctionTime;                             ///< Коэффициент коррекции времени.
    int16           enum_gate_max;                              ///< Ограничение ворот в рандомизаторе сверху
    int16           enum_gate_min;                              ///< Ограничение ворот в рандомизаторе снизу

    BalanceADC::E   balanceADCtype;                             ///< Тип балансировки.
    int8            rShiftAdd[Chan::Count][Range::Count];       ///< Добавочное смещение, которое пишется сюда при калибровке и балансировке
    StretchADC::E   stretchADCtype;                             ///< Тип растяжки канала.
    int16           stretchADC[Chan::Count][StretchADC::Count]; ///< \brief Поправочный коэффициент для ручного, калибровочного и
                    ///< отключенного режимов. Здесь хранится в целом виде, чтобы получить реальный коэффициент, нужно разделить на 1000 и
                    ///< прибавить единицу.

    int16           rShiftAddStable[Chan::Count][3];            ///< Добавочное смещение для трёх самых чувствительных диапазонов. Задаётся единожды при настройке
    int16           balanceADC[Chan::Count];                    ///< Значение дополнительного смещения АЦП для ручной балансировки.
};


struct SettingsDebug
{ //-V802
    int8         showConsole;                   ///< Показывать ли консоль
    int16        numStrings;                    ///< Число строк в консоли.
    int8         sizeFont;                      ///< Размер шрифта консоли - 0 - 5, 1 - 8.
    bool         modePauseConsole;              ///< Если true, то вывод в консоль останавливается нажатием кнопки ПУСК/СТОП.
    bool         showAll;
    bool         showFlag;
    bool         showRShift[2];
    bool         showTrigLev;
    bool         showRange[2];
    bool         showChanParam[2];
    bool         showTrigParam;
    bool         showTShift;
    bool         showTBase;
    int16        numMeasuresForGates;           ///< Число измерений для ворот.
    bool         showRandInfo;                  ///< Выводить информацию по рандомизатору - ворота и считанное значение.
    bool         showRandStat;                  ///< Выводить график статистики.
    int16        timeCompensation;              ///< Дополнительное смещение по времени для данной развёртки режима рандомизатора.
    int16        pretriggered;                  ///< Регулируемая величина предзапуска для исследования рандомизатора.
    bool         ShowStats;                     ///< Показывать статистику на экране (fps, например).
    bool         runTest;                       ///< Если true, то нужно выполнять тест при запуске
    SettingsNRST nrst;
};

float   GetStretchADC(Chan::E ch);

void    SetStretchADC(Chan::E ch, float kStretch);




struct PageDebug
{
    static const Page *const self;



    struct PageConsole
    {
        static const Page *const self;
    };


    struct PageRegisters
    {
        static const Page *const self;
    };


    struct PageRand
    {
        static const Page *const self;
    };


    struct PageSettings
    {
        static const Page *const self;
    };


    struct PageSerialNumber
    {
        static const Page *const self;
    };


    struct PageTests
    {
        static const Page *const self;

        static void OnPress_Run();
    };


    struct PageADC
    {
        static const Page *const self;


        struct PageBalance
        {
            static const Page *const self;
        };


        struct PageStretch
        {
            static const Page *const self;

            static void OnChanged_Mode(bool);
        };


        struct PageShift
        {
            static const Page *const self;
        };
    };
};
