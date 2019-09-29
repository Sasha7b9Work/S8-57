#pragma once
#include "Menu/MenuItems.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define CONSOLE_NUM_STRINGS     (set.dbg.numStrings)
#define CONSOLE_SIZE_FONT       (set.dbg.sizeFont ? 8 : 5)
/// Если truе, то включён режим остновки консоли, в котором нажатие ПУСК/СТОП приостанавливает вывод в консоль.
#define MODE_PAUSE_CONSOLE      (set.dbg.modePauseConsole)
#define NUM_MEASURES_FOR_GATES  (set.dbg.numMeasuresForGates)
#define TIME_COMPENSATION       (set.dbg.timeCompensation)
#define SHOW_RAND_INFO          (set.dbg.showRandInfo)
#define SHOW_RAND_STAT          (set.dbg.showRandStat)
#define SHOW_STAT               (set.dbg.ShowStats)
#define PRETRIGGERED            (set.dbg.pretriggered)
#define BANDWIDTH_DEBUG(ch)     (set.dbg.bandwidth[ch])

#define DBG_SHOW_ALL            (set.dbg.showAll)
#define DBG_SHOW_FLAG           (set.dbg.showFlag)

struct SettingsDebug
{
    int8                         showConsole;               ///< Показывать ли консоль
    int16                        numStrings;                ///< Число строк в консоли.
    int8                         sizeFont;                  ///< Размер шрифта консоли - 0 - 5, 1 - 8.
    bool                         modePauseConsole;          ///< Если true, то вывод в консоль останавливается нажатием кнопки ПУСК/СТОП.
    bool                         showAll;
    bool                         showFlag;
    bool                         showRShift[2];
    bool                         showTrigLev;
    bool                         showRange[2];
    bool                         showChanParam[2];
    bool                         showTrigParam;
    bool                         showTShift;
    bool                         showTBase;
    int16                        numMeasuresForGates;       ///< Число измерений для ворот.
    bool                         showRandInfo;              ///< Выводить информацию по рандомизатору - ворота и считанное значение.
    bool                         showRandStat;              ///< Выводить график статистики.
    int16                        timeCompensation;          ///< Дополнительное смещение по времени для данной развёртки режима рандомизатора.
    int16                        pretriggered;              ///< Регулируемая величина предзапуска для исследования рандомизатора.
    FPGA::Settings::Bandwidth::E bandwidth[2];              ///< \brief Здесь задаётся полоса, которая будет включаться при выборе в "КАНАЛ-Полоса" 
                                                            ///< значения "Полная".
    bool                         ShowStats;                 ///< Показывать статистику на экране (fps, например).
    int16                        enum_gate_max;             ///< Ограничение ворот в рандомизаторе сверху
    int16                        enum_gate_min;             ///< Ограничение ворот в рандомизаторе снизу
    int8                         addRShift[2][Range::Size]; ///< Добавочные смещения для 
};

float   GetStretchADC(Chan::E ch);

void    SetStretchADC(Chan::E ch, float kStretch);



/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageDebug
{
    static const Page *const self;


    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageConsole
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageRegisters
    {
        static const Page *const self;
    };


    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageRand
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSettings
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSerialNumber
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageADC
    {
        static const Page *const self;

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageBalance
        {
            static const Page *const self;
        };

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageStretch
        {
            static const Page *const self;

            static void OnChanged_Mode(bool);
        };

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageShift
        {
            static const Page *const self;
        };
    };
};
