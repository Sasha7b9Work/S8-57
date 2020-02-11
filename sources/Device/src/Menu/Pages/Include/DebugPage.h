#pragma once
#include "Menu/MenuItems.h"
#include "Settings/SettingsOsci.h"


struct SettingsDebug
{ //-V802
    int8         showConsole;           ///< Показывать ли консоль
    int16        numStrings;            ///< Число строк в консоли.
    int8         sizeFont;              ///< Размер шрифта консоли - 0 - 5, 1 - 8.
    bool         modePauseConsole;      ///< Если true, то вывод в консоль останавливается нажатием кнопки ПУСК/СТОП.
    bool         showAll;
    bool         showFlag;
    bool         showRShift[2];
    bool         showTrigLev;
    bool         showRange[2];
    bool         showChanParam[2];
    bool         showTrigParam;
    bool         showTShift;
    bool         showTBase;
    int16        numMeasuresForGates;   ///< Число измерений для ворот.
    bool         showRandInfo;          ///< Выводить информацию по рандомизатору - ворота и считанное значение.
    bool         showRandStat;          ///< Выводить график статистики.
    int16        timeCompensation;      ///< Дополнительное смещение по времени для данной развёртки режима рандомизатора.
    int16        pretriggered;          ///< Регулируемая величина предзапуска для исследования рандомизатора.
    bool         ShowStats;             ///< Показывать статистику на экране (fps, например).
    bool         runTest;               ///< Если true, то нужно выполнять тест при запуске
    bool         showBattery;           ///< Показывать или нет состояние батареи
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
