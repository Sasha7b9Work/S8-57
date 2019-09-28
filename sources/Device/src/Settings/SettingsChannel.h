#pragma once
#include "Utils/String.h"


 /// Делитель.
struct Divider
{
    enum E
    {
        _1,
        _10
    } value;
    explicit Divider(E v) : value(v) { };
    explicit Divider(uint v) : value(static_cast<E>(v)) { };
    int ToAbs() const { return (value == _1) ? 1 : 10; };
};

struct DataSettings;

struct Chan
{
    enum E
    {
        A,
        B,
        Math,
        Size
    } value;
    explicit Chan(E v) : value(v) { };
    bool IsA() { return value == A; };
    bool IsB() { return value == B; }
    int PointsInChannel() const;
    /// Возвращает количество памяти, требуемой для сохранения данных одного канала
    int RequestBytes(DataSettings *ds) const;
    pString Name() const;
};

/// Режим калибровки.
struct CalibrationMode
{
    enum E
    {
        x1,
        x10,
        Disable
    } value;
};

#define SET_INVERSE(c)          (set.ch.inverse[c])
#define SET_INVERSE_A           (SET_INVERSE(Chan::A))
#define SET_INVERSE_B           (SET_INVERSE(Chan::B))
#define SET_RSHIFT_MATH         (set.math_rShift)
#define SET_RANGE_MATH          (set.math_range)

#define SET_RANGE(c)            (set.ch.range[c])
#define SET_RANGE_A             (SET_RANGE(Chan::A))
#define SET_RANGE_B             (SET_RANGE(Chan::B))

#define SET_COUPLE(c)           (set.ch.couple[c])
#define SET_COUPLE_A            (SET_COUPLE(Chan::A))
#define SET_COUPLE_B            (SET_COUPLE(Chan::B))

#define SET_ENABLED(c)          (set.ch.enabled[c])
#define SET_ENABLED_A           (SET_ENABLED(Chan::A))
#define SET_ENABLED_B           (SET_ENABLED(Chan::B))
#define SET_ENABLED_BOTH        (SET_ENABLED_A && SET_ENABLED_B)
#define SET_DISABLED_BOTH       (!SET_ENABLED_A && !SET_ENABLED_B)

#define SET_BALANCE_ADC(c)      (set.ch.balanceShiftADC[c])
#define SET_BALANCE_ADC_A       (SET_BALANCE_ADC(Chan::A))
#define SET_BALANCE_ADC_B       (SET_BALANCE_ADC(Chan::B))

#define SET_BANDWIDTH(c)        (set.ch.bandwidth[c])
#define SET_BANDWIDTH_A         (SET_BANDWIDTH(Chan::A))
#define SET_BANDWIDTH_B         (SET_BANDWIDTH(Chan::B))

#define SET_RESISTANCE(c)       (set.ch.resistance[c])
#define SET_RESISTANCE_A        (SET_RESISTANCE(Chan::A))
#define SET_RESISTANCE_B        (SET_RESISTANCE(Chan::B))

#define SET_CALIBR_MODE(c)      (set.ch.calibrationMode[c])
#define SET_CALIBR_MODE_A       (SET_CALIBR_MODE(Chan::A))
#define CALIBR_MODE_B           (SET_CALIBR_MODE(Chan::B))
