#include "defines.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"


struct StructMeasure
{
    const char *name;
    const char UGO;
    uint8      notUsed0;
    uint8      notUsed1;
    uint8      notUsed2;
};

#define DEF_STRUCT_MEASURE(name, ugo) {name, ugo, 0, 0, 0}

static const StructMeasure sMeas[Measure::Type::Number] =
{
    DEF_STRUCT_MEASURE("",            '\x00'),
    DEF_STRUCT_MEASURE("U макс",      '\x20'),
    DEF_STRUCT_MEASURE("U мин",       '\x25'),
    DEF_STRUCT_MEASURE("U пик",       '\x2a'),
    DEF_STRUCT_MEASURE("U макс уст",  '\x40'),
    DEF_STRUCT_MEASURE("U мин уст",   '\x45'),
    DEF_STRUCT_MEASURE("U амп",       '\x4a'),
    DEF_STRUCT_MEASURE("U ср",        '\x60'),
    DEF_STRUCT_MEASURE("U скз",       '\x65'),
    DEF_STRUCT_MEASURE("Выброс+",     '\x6a'),
    DEF_STRUCT_MEASURE("Выброс-",     '\x80'),
    DEF_STRUCT_MEASURE("Период",      '\x85'),
    DEF_STRUCT_MEASURE("Частота",     '\x8a'),
    DEF_STRUCT_MEASURE("Вр нараст",   '\xa0'),
    DEF_STRUCT_MEASURE("Вр спада",    '\xa5'),
    DEF_STRUCT_MEASURE("Длит+",       '\xaa'),
    DEF_STRUCT_MEASURE("Длит-",       '\xc0'),
    DEF_STRUCT_MEASURE("Скважн+",     '\xc5'),
    DEF_STRUCT_MEASURE("Скважн-",     '\xca'),
    DEF_STRUCT_MEASURE("Задержка\xa7",'\xe0'),
    DEF_STRUCT_MEASURE("Задержка\xa6",'\xe5'),
    DEF_STRUCT_MEASURE("Фаза\xa7",    '\xe0'),
    DEF_STRUCT_MEASURE("Фаза\xa6",    '\xe5')
};



bool Measure::IsActive()
{
    if(MeasurementsOsci::posActive >= TableMeasures::NumCols() * TableMeasures::NumRows())
    {
        MeasurementsOsci::posActive = 0;
    }
    return (row * TableMeasures::NumCols() + col) == MeasurementsOsci::posActive;
}


void Measure::SetActive(int row, int col)
{
    MeasurementsOsci::posActive = (int8)(row * TableMeasures::NumCols() + col);
}


char Measure::GetChar(Measure::Type::E measure)
{
    return sMeas[measure].UGO;
}


void Measure::ChangeActive(int delta)
{
    Measure measure = MeasurementsOsci::GetActiveMeasure();

    int row = measure.row;
    int col = measure.col;

    col += Math::Sign(delta);

    if (col < 0)
    {
        col = TableMeasures::NumCols() - 1;
        row--;
        if (row < 0)
        {
            row = TableMeasures::NumRows() - 1;
        }
    }
    else if (col == TableMeasures::NumCols())
    {
        col = 0;
        row++;
        if (row >= TableMeasures::NumRows())
        {
            row = 0;
        }
    }
    else
    {
        // здесь ничего
    }

    SetActive(row, col);
}


String Measure::Name()
{
    Type::E type = GetType();

    return (type > Type::None && type < Type::Number) ? String(sMeas[type].name) : String("");
}


String Measure::GetName(Measure::Type::E type)
{
    return String(sMeas[type].name);
}


Measure::Type::E Measure::GetType()
{
    return set.meas.measures[row * TableMeasures::NumCols() + col];
}


void Measure::ShortPressOnSmallButonMarker()
{
    if(set.meas.measures[MeasurementsOsci::posActive] == set.meas.marked)
    {
        set.meas.marked = Measure::Type::None;
    }
    else
    {
        set.meas.marked = set.meas.measures[MeasurementsOsci::posActive];
    }
}


void Measure::SetMarkerVoltage(Chan::E ch, int num, float value)
{
    MeasurementsOsci::markerVoltage[ch][num] = (int)value;
}


void Measure::SetMarkerTime(Chan::E ch, int num, int value)
{
    MeasurementsOsci::markerTime[ch][num] = value;
}
