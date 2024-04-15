// 2023/11/21 13:09:35 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/DeviceSettings.h"
#include "Hardware/Memory/RAM.h"
#include "Menu/Pages/Include/PageMemory.h"


#define DS   (Reader::pDS)             // Указатель на настройки текущего рисуемого сигнала.

#define ENABLE_CH_DS(ch)  (((ch) == Chan::A) ? DS->enableA : DS->enableB)

#define _IN(ch)  (Reader::dataIN[static_cast<int>(ch)])
#define _IN_A    _IN(ChanA)
#define IN_B    _IN(ChanB)
#define _OUT(ch) (Reader::dataOUT[static_cast<int>(ch)])
#define _OUT_A   _OUT(ChanA)
#define OUT_B   _OUT(ChanB)

#define AVE_DATA(ch) (Reader::ave[static_cast<int>(ch)])
#define AVE_1        AVE_DATA(ChanA)
#define AVE_2        AVE_DATA(ChanB)


#define RANGE_DS(ch)    (RANGE(DS, ch))
#define RSHIFT_DS(ch)   (RSHIFT(DS, ch))
#define RSHIFT_DS_A     (RSHIFT(DS, ChanA))
#define RSHIFT_DS_B     (RSHIFT(DS, ChanB))
#define TBASE_DS        (TBASE(DS))


#define TIME_TIME_DS    (TIME_TIME(DS))
#define TIME_DAY_DS     (TIME_DAY(DS))
#define TIME_HOURS_DS   (TIME_HOURS(DS))
#define TIME_MINUTES_DS (TIME_MINUTES(DS))
#define TIME_SECONDS_DS (TIME_SECONDS(DS))
#define TIME_MONTH_DS   (TIME_MONTH(DS))
#define TIME_YEAR_DS    (TIME_YEAR(DS))

#define BYTES_IN_CHANNEL_DS    (BYTES_IN_CHANNEL(DS))


// В этой структуре будут храниться точки, подготовленные для вывода на экран
struct StructDataDrawing
{
    uint8       data[Chan::Count][281 * 2];    // Данные обоих каналов. Точек в два раза больше, чем на экране, для пикового детектора
    bool        needDraw[Chan::Count];         // Если true, то канал 1 надо рисовать
    uint8       notUsed0[2];
    int         posBreak;                      // Позиция в координатах сетки, на которой нужно рисовать линию разрыва поточечного вывода
    ModeWork::E forMode;                       // Для какого режима считываются данные
    uint8       notUsed1[3];
};



namespace Reader
{
    // Читает данные из хранилища
    void ReadDataFromRAM();

    void ReadDataFromROM();

    // Указатель на настройки текущего рисуемого сигнала. Обращаться к нему следует через макрос DS.
    // Указатель на настройки считанных данных
    extern const DataSettings *pDS;

    extern uint8 *dataIN[2];

    extern uint8 *dataOUT[2];           // Считанные данные второго канала

    extern uint16 *ave[2];
};
