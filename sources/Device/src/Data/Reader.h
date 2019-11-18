#pragma once
#include "Data/DataSettings.h"
#include "Hardware/Memory/RAM.h"
#include "Menu/Pages/Include/PageMemory.h"


struct FrameP2P;

extern FrameP2P *frameP2P;
extern const DataSettings *pDS;           ///< Указатель на настройки текущего рисуемого сигнала. Обращаться к нему следует через макрос DS.
extern uint8 *dataOUT[2];           ///< Считанные данные второго канала
extern const uint8 *dataIN[2];
extern uint16 *ave[2];

#define DS          pDS             ///< Указатель на настройки текущего рисуемого сигнала.
#define FRAME_P2P   frameP2P

#define IN(ch)  (dataIN[static_cast<int>(ch)])
#define IN_A    IN(Chan::A)
#define IN_B    IN(Chan::B)
#define OUT(ch) (dataOUT[static_cast<int>(ch)])
#define OUT_A   OUT(Chan::A)
#define OUT_B   OUT(Chan::B)

#define AVE_DATA(ch) ave[static_cast<int>(ch)]
#define AVE_1        AVE_DATA(Chan::A)
#define AVE_2        AVE_DATA(Chan::B)


#define RANGE_DS(ch)    (RANGE(DS, ch))
#define RSHIFT_DS(ch)   (RSHIFT(DS, ch))
#define RSHIFT_DS_A     (RSHIFT(DS, Chan::A))
#define RSHIFT_DS_B     (RSHIFT(DS, Chan::B))
#define TBASE_DS        (TBASE(DS))


#define TIME_TIME_DS    (TIME_TIME(DS))
#define TIME_DAY_DS     (TIME_DAY(DS))
#define TIME_HOURS_DS   (TIME_HOURS(DS))
#define TIME_MINUTES_DS (TIME_MINUTES(DS))
#define TIME_SECONDS_DS (TIME_SECONDS(DS))
#define TIME_MONTH_DS   (TIME_MONTH(DS))
#define TIME_YEAR_DS    (TIME_YEAR(DS))

#define BYTES_IN_CHANNEL_DS    (BYTES_IN_CHANNEL(DS))


/// В этой структуре будут храниться точки, подготовленные для вывода на экран
struct StructDataDrawing
{
    uint8       data[Chan::Count][281 * 2];    ///< Данные обоих каналов. Точек в два раза больше, чем на экране, для пикового детектора
    bool        needDraw[Chan::Count];         ///< Если true, то канал 1 надо рисовать
    uint8       notUsed0[2];
    int         posBreak;                      ///< Позиция в координатах сетки, на которой нужно рисовать линию разрыва поточечного вывода
    ModeWork::E forMode;                       ///< Для какого режима считываются данные
    uint8       notUsed1[3];
};



class Reader
{
public:
    /// Читает данные из хранилища
    static void ReadDataFromRAM();

    static void ReadDataFromROM();
private:
    /// Читает фрейм поточечного вывода
    static void ReadDataP2P();

};
