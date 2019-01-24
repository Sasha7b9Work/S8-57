#include "defines.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Hardware/Sound.h"
#include "Utils/Math.h"

#include "Osci/Measurements/Processing.h"
#include "Osci/Measurements/Measurements_Graphics.h"


using namespace Display::Primitives;
using namespace Osci::Processing;
using namespace Osci::Measurements;
using namespace Osci::Measurements::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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
    DEF_STRUCT_MEASURE("U ампл",      '\x4a'),
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

int8 Measure::posActive = 0;
bool Measure::pageChoiceIsActive = false;
int8 Measure::posOnPageChoice = 0;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool Measure::IsActive()
{
    if(posActive >= Graphics::NumCols() * Graphics::NumRows())
    {
        posActive = 0;
    }
    return (row * Graphics::NumCols() + col) == posActive;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Measure Measure::GetActive()
{
    int row = posActive / Graphics::NumCols();
    int col = posActive - row * Graphics::NumCols();

    return Measure(row, col);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetActive(int row, int col)
{
    posActive = (int8)(row * Graphics::NumCols() + col);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char Measure::GetChar(Measure::Type::E measure)
{
    return sMeas[measure].UGO;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::ChangeActive(int delta)
{
    Measure measure = GetActive();

    int row = measure.row;
    int col = measure.col;

    col += Math::Sign(delta);

    if (col < 0)
    {
        col = Graphics::NumCols() - 1;
        row--;
        if (row < 0)
        {
            row = Graphics::NumRows() - 1;
        }
    }
    else if (col == Graphics::NumCols())
    {
        col = 0;
        row++;
        if (row >= Graphics::NumRows())
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


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::DY()
{
    if(VIEW_MEASURES_BOTH)
    {
        return 30;
    }
    return 21;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::DX()
{
    return Grid::Width() / 5; 
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Measure::Name()
{
    return String(sMeas[GetType()].name);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Measure::Type::E Measure::GetType()
{
    return set.meas_measures[row * Graphics::NumCols() + col];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::NumCols()
{
    const int cols[] = {1, 2, 5, 5, 5, 1, 2};
    return cols[NUM_MEASURES];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::NumRows()
{
    int rows[] = {1, 1, 1, 2, 3, 6, 6};
    return rows[NUM_MEASURES];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Osci::Measurements::Graphics::GetDeltaGridLeft()
{
    if(SHOW_MEASURES && MODE_VIEW_SIGNALS_IS_COMPRESS)
    {
        if(NUM_MEASURES_IS_6_1)
        {
            return Graphics::DX();
        }
        else if(NUM_MEASURES_IS_6_2)
        {
            return DX() * 2;
        }
        else
        {
            // других вариантов нет
        }
    }
    return 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::ShortPressOnSmallButonMarker()
{
    if(set.meas_measures[posActive] == MEAS_MARKED)
    {
        MEAS_MARKED = Measure::Type::None;
    }
    else
    {
        MEAS_MARKED = set.meas_measures[posActive];
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerVoltage(Chan::E ch, int num, float value)
{
    Processing::markerVoltage[ch][num] = (int)value;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetMarkerTime(Chan::E ch, int num, int value)
{
    Processing::markerTime[ch][num] = value;
}
