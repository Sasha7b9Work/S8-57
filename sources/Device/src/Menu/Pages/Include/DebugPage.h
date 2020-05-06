#pragma once
#include "Menu/MenuItems.h"


#define S_DBG_SHOW_CONSOLE              (set.dbg._showConsole)
#define S_DBG_NUM_STRINGS_IN_CONSOLE    (set.dbg._numStrings)
#define S_DBG_SHOW_ALL_REGISTERS        (set.dbg._showAll)
#define S_DBG_SHOW_FLAG                 (set.dbg._showFlag)
#define S_DBG_SHOW_RSHIFT(chan)         (set.dbg._showRShift[chan])
#define S_DBG_SHOW_TRIG_LEV             (set.dbg._showTrigLev)
#define S_DBG_SHOW_RANGE(chan)          (set.dbg._showRange[chan])
#define S_DBG_SHOW_CHAN_PARAM(chan)     (set.dbg._showChanParam[chan])


struct SettingsDebug
{ //-V802
    int8   _showConsole;         // Показывать ли консоль
    int16  _numStrings;          // Число строк в консоли.
    bool   _showAll;
    bool   _showFlag;
    bool   _showRShift[2];
    bool   _showTrigLev;
    bool   _showRange[2];
    bool   _showChanParam[2];
    bool   showTrigParam;
    bool   showTShift;
    bool   showTBase;
    int16  numMeasuresForGates; // Число измерений для ворот.
    bool   showRandInfo;        // Выводить информацию по рандомизатору - ворота и считанное значение.
    bool   showRandStat;        // Выводить график статистики.
    int16  timeCompensation;    // Дополнительное смещение по времени для данной развёртки режима рандомизатора.
    int16  pretriggered;        // Регулируемая величина предзапуска для исследования рандомизатора.
    bool   ShowStats;           // Показывать статистику на экране (fps, например).
    bool   runTest;             // Если true, то нужно выполнять тест при запуске
    bool   showBattery;         // Показывать или нет состояние батареи
    bool   showRandGate;        // Выводить ли значения ворот в режиме рандомизатора
    bool   showRandPredPost;    // Выводить ли значения пред- и после- запуска в режиме рандомизатора
};


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
