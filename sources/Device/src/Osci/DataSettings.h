#pragma once
#include "Settings/SettingsOsci.h"


class Buffer;


struct PackedTime
{
    unsigned hours : 5;
    unsigned minutes : 6;
    unsigned seconds : 6;
    unsigned year : 7;
    unsigned month : 4;
    unsigned notUsed0 : 4;
    unsigned day : 5;
    unsigned notUsed1 : 27;
    PackedTime(uint h = 11, uint m = 11, uint s = 11, uint d = 11, uint mo = 11, uint y = 11) :
        hours(h), minutes(m), seconds(s), year(y), month(mo), notUsed0(0), day(d), notUsed1(0) {};
    /// Изменение значения поля на +/- 1
    void ChangeHours(int delta);
    void ChangeMinutes(int delta);
    void ChangeSeconds(int delta);
    void ChangeDay(int delta);
    void ChangeMonth(int delta);
    void ChangeYear(int delta);
};

struct DataSettings
{
friend class RAM;
friend class Reader;
friend struct PacketROM;
friend struct FrameP2P;
friend struct Sector;
friend struct FPGA;
friend struct Packet;
friend struct TestMemoryStruct;

    uint        id;                 ///< Сквозной id данных. 1-й номер получают данные, считанные первыми после включения прибора и каждые следующие увиличиваются на 1
    uint        timeMS;             ///< Время чтения данных
private:
    uint8       *dataA;             ///< По этому адресу хранятся данные 1-го канала
    uint8       *dataB;             ///< По этому адресу хранятся данные 2-го канала. При хранение в Buffer данные 2-го канала идут сразу после 1-го канала
public:
    int16       rShift[2];
    int16       trigLev[2];
    int         tShift;             ///< Смещение по времени
    Range::E    range[2];           ///< Масштаб по напряжению обоих каналов.
    uint        tBase       : 5;    ///< Масштаб по времени
    uint        enableA     : 1;    ///< Включён ли канал A
    uint        enableB     : 1;    ///< Включен ли канал B
    uint        coupleA     : 2;    ///< Режим канала по входу
    uint        coupleB     : 2;
    uint        peackDet    : 2;    ///< Включен ли пиковый детектор
    uint        inverseA    : 1;
    uint        inverseB    : 1;
    uint        multiplierA : 1;
    uint        multiplierB : 1;
    uint        enumPoints  : 3;
    uint        numInROM    : 5;    ///< Номер данных в памяти ROM
    uint        notUsed     : 7;
    PackedTime  time;
    /// Заполняет структуру в соответствии с текущими настройками
    void Fill();
    /// Возвращает размер занимаемый данными одного канала
    uint BytesInChannel() const;
    /// Количество отсчётов на канал
    uint PointsInChannel() const;
    /// Возвращает требуемое количество памяти для хранения данных каналов
    uint NeedMemoryForData() const;
    bool Equals(const DataSettings &ds) const;
    bool EqualsCurrentSettings() const;
    void Log() const;
    uint8 *Data(Chan::E ch) { return ch == Chan::A ? dataA : dataB; }
};


#define DATA(ds, ch)         (((ch) == Chan::A) ? (ds)->dataA : (ds)->dataB)

#define Lval_ENABLED_A(ds)      ((ds)->enableA)
#define Lval_ENABLED_B(ds)      ((ds)->enableB)
#define Lval_ENABLED(ds, ch)    (ch.IsA() ? Lval_ENABLED_A(ds) : Lval_ENABLED_B(ds))
#define ENABLED_A(ds)           ((bool)Lval_ENABLED_A(ds))
#define ENABLED_B(ds)           ((bool)Lval_ENABLED_B(ds))
#define ENABLED(ds, ch)         (((ch) == Chan::A) ? ENABLED_A(ds) : ENABLED_B(ds))

#define ENABLED_DS(ch)          (ENABLED(DS, ch))
#define ENABLED_DS_A            ENABLED_A(DS)
#define ENABLED_DS_B            ENABLED_B(DS)

#define RSHIFT(ds, ch)          ((ds)->rShift[static_cast<int>(ch)])
#define RSHIFT_A(ds)            (RSHIFT(ds, Chan::A))
#define RSHIFT_B(ds)            (RSHIFT(ds, Chan::B))

#define Lval_RANGE(ds, ch)      ((ds)->range[static_cast<int>(ch)])
#define Lval_RANGE_A(ds)        (Lval_RANGE(ds, Chan::A))
#define Lval_RANGE_B(ds)        (Lval_RANGE(ds, Chan::B))
#define RANGE(ds, ch)           ((Range::E)Lval_RANGE(ds, ch))
#define RANGE_A(ds)             (RANGE(ds, Chan::A))
#define RANGE_B(ds)             (RANGE(ds, Chan::B))

#define Lval_TBASE(ds)          ((ds)->tBase)
#define TBASE(ds)               ((TBase::E)(Lval_TBASE(ds)))


#define TIME_TIME(ds)           ((ds)->time)
#define TIME_DAY(ds)            ((ds)->time.day)
#define TIME_HOURS(ds)          ((ds)->time.hours)
#define TIME_MINUTES(ds)        ((ds)->time.minutes)
#define TIME_SECONDS(ds)        ((ds)->time.seconds)
#define TIME_MONTH(ds)          ((ds)->time.month)
#define TIME_YEAR(ds)           ((ds)->time.year)

#define INVERSE_A(ds)           ((ds)->inverseA)
#define INVERSE_B(ds)           ((ds)->inverseB)
#define INVERSE(ds, ch)         (ch.IsA() ? INVERSE_A(ds) : INVERSE_B(ds))

#define TSHIFT(ds)              ((ds)->tShift)

#define Lval_COUPLE_A(ds)       ((ds)->coupleA)
#define Lval_COUPLE_B(ds)       ((ds)->coupleB)
#define COUPLE_A(ds)            ((ModeCouple)Lval_COUPLE_A(ds))
#define COUPLE_B(ds)            ((ModeCouple)Lval_COUPLE_B(ds))
#define COUPLE(ds, ch)          (ch.IsA() ? COUPLE_A(ds) : COUPLE_B(ds))

#define TRIGLEV(ds, ch)         ((ds)->trigLev[ch])
#define TRIGLEV_A(ds)           (TRIGLEV(ds, Chan::A))
#define TRIGLEV_B(ds)           (TRIGLEV(ds, Chan::B))

#define Lval_PEAKDET(ds)        ((ds)->peackDet)
#define PEAKDET(ds)             ((PeakDetMode::E)Lval_PEAKDET(ds))
#define PEAKDET_DISABLED(ds)     (PEAKDET(ds) == PeakDetMode::Disabled)

#define Lval_DIVIDER_A(ds)      ((ds)->multiplierA)
#define Lval_DIVIDER_B(ds)      ((ds)->multiplierB)

#define ENUM_POINTS(ds)         ((ds)->enumPoints)
#define BYTES_IN_CHANNEL(ds)    ((uint)(ds)->BytesInChannel())


struct FrameP2P
{
    static DataSettings *ds;
    /// Подготовить фрейм для нового цикла чтения
    static void Prepare();
    static void AddPoint(const BitSet16 &a, const BitSet16 &b);
    static void FillScreenBuffer(Buffer *buffer, Chan::E ch);
    /// true, если данные нужно выводить из поточечного фрейма
    static bool IsCorrect();
    /// Позиция раздела в поточечном выводе - в ней нужно нарисовать вертикальную линию
    static int posSeparate;
private:
    /// Количество всех считанных точек в поточечном режиме
    static uint numBytesP2P;
    /// Указатель на позицию записываемых данных. В байтах
    static uint pointerP2P;
    /// Добавить точки при выключенном пиковом детекторе
    static void AddNormalPoint(uint8 a, uint8 b);
    static void AddNormalPoint(Chan::E ch, uint8 point);
    /// Добавить точки при включённом пиковом детекторе
    static void AddPeakDetPoint(uint16 a, uint16 b);
    static void AddPeakDetPoint(Chan::E ch, uint16 point);
    /// Возвращает количество имеющихся байт канала
    static uint GetNumberStoredBytes();
    /// Возвращает значение байта в позиции position
    static uint8 GetByte(uint position, Chan::E ch);
};
