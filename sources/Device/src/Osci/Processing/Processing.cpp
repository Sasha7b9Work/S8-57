#include "defines.h"
#include "Data/DataStorage.h"
#include "Data/Reader.h"
#include "FPGA/FPGA_Math.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"
#include "Utils/Math.h"
#include <cmath>
#include <climits>
#include <cstdlib>
#include <cstring>
#include <limits>

#include "Osci/Processing/Processing.h"


using namespace Osci;
using namespace Osci::Settings;
using namespace Osci::Processing::Measurements::Settings;

using FPGA::VALUE::MIN;
using FPGA::VALUE::MAX;
using FPGA::VALUE::AVE;
using FPGA::VALUE::NONE;

using Osci::Processing::Measure;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static float CalculateVoltageMax(Chan::E ch);
static float CalculateVoltageMin(Chan::E ch);
static float CalculateVoltagePic(Chan::E ch);
static float CalculateVoltageMaxSteady(Chan::E ch);
static float CalculateVoltageMinSteady(Chan::E ch);
static float CalculateVoltageAmpl(Chan::E ch);
static float CalculateVoltageAverage(Chan::E ch);
static float CalculateVoltageRMS(Chan::E ch);
static float CalculateVoltageVybrosPlus(Chan::E ch);
static float CalculateVoltageVybrosMinus(Chan::E ch);
static float CalculatePeriod(Chan::E ch);
static int   CalculatePeriodAccurately(Chan::E ch); ///< Точно вычисляет период или целое число периодов в точках сигнала.
static float CalculateFreq(Chan::E ch);
static float CalculateTimeNarastaniya(Chan::E ch);
static float CalculateTimeSpada(Chan::E ch);
static float CalculateDurationPlus(Chan::E ch);
static float CalculateDurationMinus(Chan::E ch);
static float CalculateSkvaznostPlus(Chan::E ch);
static float CalculateSkvaznostMinus(Chan::E ch);


static float CalculateMinRel(Chan::E ch);           ///< Возвращает минимальное значение относительного сигнала.
static float CalculateMinSteadyRel(Chan::E ch);     ///< Возвращает минимальное установившееся значение относительного сигнала.
static float CalculateMaxRel(Chan::E ch);           ///< Возвращает максимальное значение относительного сигнала.
static float CalculateMaxSteadyRel(Chan::E ch);     ///< Возвращает максимальное установившееся значение относительного сигнала.
static float CalculateAverageRel(Chan::E ch);       ///< Возвращает среденее значение относительного сигнала.
static float CalculatePicRel(Chan::E ch);
static float CalculateDelayPlus(Chan::E ch);
static float CalculateDelayMinus(Chan::E ch);
static float CalculatePhazaPlus(Chan::E ch);
static float CalculatePhazaMinus(Chan::E ch);
/// \brief Найти точку пересечения сигнала с горизонтальной линией, проведённой на уровне yLine. numItersection - порядковый номер пересечения, 
/// начинается с 1. downToTop - если true, ищем пересечение сигнала со средней линией при прохождении из "-" в "+".
static float FindIntersectionWithHorLine(Chan::E ch, int numIntersection, bool downToUp, uint8 yLine);
/// Делить val1 на val2. Возвращает nan, если результат неопределён
static float Divide(float val1, float val2);

static bool isSet = false;          ///< Если true, то сигнал назначен.

static int firstByte = 0;
static int lastByte = 0;
/// Количество байт, по которым производятся расчёты
static int nBytes = 0;


typedef float (*pFuncFCh)(Chan::E);
typedef char *(*pFuncPCFBPC)(float, bool, char*);

typedef struct
{
    const char *name;
    pFuncFCh    FuncCalculate;
    pFuncPCFBPC FucnConvertate;
    bool        showSign;           ///< Если true, нужно показывать знак.
    uint8       notUsed0[3];
} MeasureCalculate;

static char* Freq2String(float, bool, char buffer[20]);
static char* Time2String(float, bool, char buffer[20]);
static char* Voltage2String(float, bool, char buffer[20]);
static char* Phase2String(float, bool, char buffer[20]);
static char* Float2String(float, bool, char buffer[20]);

static const MeasureCalculate sMeas[Osci::Processing::Measure::Type::Number] =
{
    {"", 0, 0, false, {}},
    {"CalculateVoltageMax",         CalculateVoltageMax,         Voltage2String, true,  {}},
    {"CalculateVoltageMin",         CalculateVoltageMin,         Voltage2String, true,  {}},
    {"CalculateVoltagePic",         CalculateVoltagePic,         Voltage2String, false, {}},
    {"CalculateVoltageMaxSteady",   CalculateVoltageMaxSteady,   Voltage2String, true,  {}},
    {"CalculateVoltageMinSteady",   CalculateVoltageMinSteady,   Voltage2String, true,  {}},
    {"CalculateVoltageAmpl",        CalculateVoltageAmpl,        Voltage2String, false, {}},
    {"CalculateVoltageAverage",     CalculateVoltageAverage,     Voltage2String, true,  {}},
    {"CalculateVoltageRMS",         CalculateVoltageRMS,         Voltage2String, false, {}},
    {"CalculateVoltageVybrosPlus",  CalculateVoltageVybrosPlus,  Voltage2String, false, {}},
    {"CalculateVoltageVybrosMinus", CalculateVoltageVybrosMinus, Voltage2String, false, {}},
    {"CalculatePeriod",             CalculatePeriod,             Time2String,    false, {}},
    {"CalculateFreq",               CalculateFreq,               Freq2String,    false, {}},
    {"CalculateTimeNarastaniya",    CalculateTimeNarastaniya,    Time2String,    false, {}},
    {"CalculateTimeSpada",          CalculateTimeSpada,          Time2String,    false, {}},
    {"CalculateDurationPlus",       CalculateDurationPlus,       Time2String,    false, {}},
    {"CalculateDurationPlus",       CalculateDurationMinus,      Time2String,    false, {}},
    {"CalculateSkvaznostPlus",      CalculateSkvaznostPlus,      Float2String,   false, {}},
    {"CalculateSkvaznostMinus",     CalculateSkvaznostMinus,     Float2String,   false, {}},
    {"CalculateDelayPlus",          CalculateDelayPlus,          Time2String,    false, {}},
    {"CalculateDelayMinus",         CalculateDelayMinus,         Time2String,    false, {}},
    {"CalculatePhazaPlus",          CalculatePhazaPlus,          Phase2String,   false, {}},
    {"CalculatePhazaMinus",         CalculatePhazaMinus,         Phase2String,   false, {}}
};


int Osci::Processing::markerTime[Chan::Size][2] = {{ERROR_VALUE_INT}, {ERROR_VALUE_INT}};

int Osci::Processing::markerVoltage[Chan::Size][2] = {{ERROR_VALUE_INT}, {ERROR_VALUE_INT}};

typedef struct
{
    float value[2];
} MeasureValue;

static MeasureValue values[Measure::Type::Number] = {{{0.0F, 0.0F}}};

static bool maxIsCalculating[2] = {false, false};
static bool minIsCalculating[2] = {false, false};
static bool maxSteadyIsCalculating[2] = {false, false};
static bool minSteadyIsCalculating[2] = {false, false};
static bool aveIsCalculating[2] = {false, false};
static bool periodIsCaclulating[2] = {false, false};
static bool periodAccurateIsCalculating[2];
static bool picIsCalculating[2] = {false, false};

#define EXIT_IF_ERROR_FLOAT(x)      if(isnan(x))                return ERROR_VALUE_FLOAT;
#define EXIT_IF_ERRORS_FLOAT(x, y)  if(isnan(x) || isnan(y))    return ERROR_VALUE_FLOAT;
#define EXIT_IF_ERROR_INT(x)        if((x) == ERROR_VALUE_INT)  return ERROR_VALUE_FLOAT;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Processing::CalculateMeasures()
{
    if(!SHOW_MEASURES || !isSet)
    {
        return;
    }
    
    maxIsCalculating[0] = maxIsCalculating[1] = maxSteadyIsCalculating[0] = maxSteadyIsCalculating[1] = false;
    minIsCalculating[0] = minIsCalculating[1] = minSteadyIsCalculating[0] = minSteadyIsCalculating[1] = false;
    aveIsCalculating[0] = aveIsCalculating[1] = false;
    periodIsCaclulating[0] = periodIsCaclulating[1] = false;
    periodAccurateIsCalculating[0] = periodAccurateIsCalculating[1] = false;
    picIsCalculating[0] = picIsCalculating[1] = false;

    for(int str = 0; str < Measure::NumRows(); str++)
    {
        for(int elem = 0; elem < Measure::NumCols(); elem++)
        {
            Measure measure = Measure::Get(str, elem);
            Measure::Type::E type = measure.GetType();

            pFuncFCh func = sMeas[type].FuncCalculate;
            if(func)
            {
                if(type == MEAS_MARKED || MEAS_MARKED == Measure::Type::None)
                {
                    markerTime[Chan::A][0] = markerTime[Chan::A][1] = markerTime[Chan::B][0] = markerTime[Chan::B][1] = ERROR_VALUE_INT;
                    markerVoltage[Chan::A][0] = markerVoltage[Chan::A][1] = markerVoltage[Chan::B][0] = markerVoltage[Chan::B][1] = ERROR_VALUE_INT;
                }
                
                if(VIEW_MEASURES_A)
                {
                    values[type].value[Chan::A] = func(Chan::A);
                }
                
                if(VIEW_MEASURES_B)
                {
                    values[type].value[Chan::B] = func(Chan::B);
                }
            }
        }
    }
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMax(Chan::E ch)
{
    float max = CalculateMaxRel(ch);
    EXIT_IF_ERROR_FLOAT(max); //-V2507
    if(MEAS_MARKED == Measure::Type::VoltageMax)
    {
        Measure::SetMarkerVoltage(ch, 0, max);      // Здесь не округляем, потому что max может быть только целым
    }
    
    Range::E range = RANGE_DS(ch);

    uint16 rShift = RSHIFT_DS(ch);

    uint8 value = ROUND(uint8, max);

    return FPGA::Math::Point2Voltage(value, range, rShift);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMin(Chan::E ch)
{
    float min = CalculateMinRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if(MEAS_MARKED == Measure::Type::VoltageMin)
    {
        Measure::SetMarkerVoltage(ch, 0, min);             // Здесь не округляем, потому что min может быть только целым
    }
    
    return FPGA::Math::Point2Voltage(ROUND(uint8, min), RANGE_DS(ch),RSHIFT_DS(ch));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltagePic(Chan::E ch)
{
    float max = CalculateVoltageMax(ch);
    float min = CalculateVoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if(MEAS_MARKED == Measure::Type::VoltagePic)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinRel(ch));
    }
    return max - min;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMinSteady(Chan::E ch)
{
    float min = CalculateMinSteadyRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if(MEAS_MARKED == Measure::Type::VoltageMinSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, ROUND(float, min));
    }

    return FPGA::Math::Point2Voltage(ROUND(uint8, min), RANGE_DS(ch), RSHIFT_DS(ch));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageMaxSteady(Chan::E ch)
{
    float max = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(max); //-V2507

    if(MEAS_MARKED == Measure::Type::VoltageMaxSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
    }

    return FPGA::Math::Point2Voltage(ROUND(uint8, max), RANGE_DS(ch), RSHIFT_DS(ch));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageVybrosPlus(Chan::E ch)
{
    float max = CalculateMaxRel(ch);
    float maxSteady = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady); //-V2507

    if (MEAS_MARKED == Measure::Type::VoltageVybrosPlus)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
        Measure::SetMarkerVoltage(ch, 1, maxSteady);
    }

    uint16 rShift = RSHIFT_DS(ch);
    return std::fabsf(FPGA::Math::Point2Voltage(ROUND(uint8, maxSteady), RANGE_DS(ch), rShift) - FPGA::Math::Point2Voltage(ROUND(uint8, max), RANGE_DS(ch), rShift));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageVybrosMinus(Chan::E ch)
{
    float min = CalculateMinRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);
    EXIT_IF_ERRORS_FLOAT(min, minSteady); //-V2507

    if (MEAS_MARKED == Measure::Type::VoltageVybrosMinus)
    {
        Measure::SetMarkerVoltage(ch, 0, min);
        Measure::SetMarkerVoltage(ch, 1, minSteady);
    }

    uint16 rShift = RSHIFT_DS(ch);
    return std::fabsf(FPGA::Math::Point2Voltage(ROUND(uint8, minSteady), RANGE_DS(ch), rShift) - FPGA::Math::Point2Voltage(ROUND(uint8, min), RANGE_DS(ch), rShift));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageAmpl(Chan::E ch)
{
    float max = CalculateVoltageMaxSteady(ch);
    float min = CalculateVoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if(MEAS_MARKED == Measure::Type::VoltageAmpl)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxSteadyRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinSteadyRel(ch));
    }
    return max - min;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
/// Входной буфер данных канала ch
#define CHOICE_BUFFER (OUT(ch))


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageAverage(Chan::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    int sum = 0;

    uint8 *data = &CHOICE_BUFFER[firstByte];

    for(int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = (uint8)((float)sum / period);

    if(MEAS_MARKED == Measure::Type::VoltageAverage)
    {
        Measure::SetMarkerVoltage(ch, 0, aveRel);
    }

    return FPGA::Math::Point2Voltage(aveRel, RANGE_DS(ch), RSHIFT_DS(ch));
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateVoltageRMS(Chan::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    float rms = 0.0F;

    Range::E range = RANGE_DS(ch);
    uint16 rShift = RSHIFT_DS(ch);

    uint8 *dataIn = &CHOICE_BUFFER[firstByte];

    for(int i = firstByte; i < firstByte + period; i++)
    {
        float volts = FPGA::Math::Point2Voltage(dataIn[i], range, rShift);
        rms +=  volts * volts;
    }

    rms = std::sqrtf(rms / period);

    if(MEAS_MARKED == Measure::Type::VoltageRMS)
    {
        Measure::SetMarkerVoltage(ch, 0, FPGA::Math::Voltage2Point(rms, range, rShift));
    }

    return rms;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePeriod(Chan::E ch)
{
    static float period[2] = {0.0F, 0.0F};

    static float firstIntersection = 0.0F;
    static float secondIntersection = 0.0F;

    if(!periodIsCaclulating[ch])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == ERROR_VALUE_UINT8) //-V550
        {
            period[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            float intersectionDownToTop = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);
            float intersectionTopToDown = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown); //-V2507

            bool firstDownToTop = intersectionDownToTop < intersectionTopToDown;
            firstIntersection = firstDownToTop ? intersectionDownToTop : intersectionTopToDown;
            secondIntersection = FindIntersectionWithHorLine(ch, 2, firstDownToTop, (uint8)aveValue);

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

            float per = FPGA::Math::TShift2Abs(ROUND(uint16, secondIntersection - firstIntersection), SET_TBASE);

            period[ch] = per;

            periodIsCaclulating[ch] = true;
        }
    }

    if ((MEAS_MARKED == Measure::Type::Period || MEAS_MARKED == Measure::Type::Freq) && periodIsCaclulating[ch])
    {
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    return period[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
#define EXIT_FROM_PERIOD_ACCURACY           \
    period[ch] = ERROR_VALUE_INT;           \
    periodAccurateIsCalculating[ch] = true; \
    std::free(sums);                        \
    return period[ch];


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int CalculatePeriodAccurately(Chan::E ch)
{
    static int period[2];

    int *sums = (int *)std::malloc((uint)nBytes);

    if (sums == 0)
    {
        LOG_ERROR("Нет памяти");
        return 0;
    }

    uint8 *dataIn = CHOICE_BUFFER;

    if(!periodAccurateIsCalculating[ch])
    {
        period[ch] = 0;
        float pic = CalculatePicRel(ch);

        if(pic == ERROR_VALUE_FLOAT)
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = (int)(pic * 5.0F);
        sums[firstByte] = dataIn[firstByte];

        int i = firstByte + 1;
        int *pSum = &sums[i];
        uint8 *data = &dataIn[i];
        uint8 *end = &dataIn[lastByte];

        while (data < end)
        {
            uint8 point = *data++;
            if(point < MIN || point >= MAX)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *pSum = *(pSum - 1) + point;
            pSum++;
        }

        int addShift = firstByte - 1;
        int maxPeriod = (int)(nBytes * 0.95F);

        for(int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int sum = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = nBytes - nextPeriod;

            int *pSums = &sums[firstByte + 1];

            for(int start = 1; start < maxStart; start++)
            {
                int nextSum = *(pSums + nextPeriod) - (*pSums);
                pSums++;

                int nextDelta = nextSum - sum;

                if (nextSum < sum)
                {
                    nextDelta = -nextDelta;
                }

                if(nextDelta > delta)
                {
                    maxDelta = delta + 1;
                    break;
                }
                else if(nextDelta > maxDelta)
                {
                    maxDelta = nextDelta;
                }
                else
                {
                    // здесь ничего не делаем
                }
            }

            if(maxDelta < delta)
            {
                delta = maxDelta;
                period[ch] = nextPeriod;
            }
        }

        if(period[ch] == 0)
        {
            period[ch] = ERROR_VALUE_INT;
        }
        periodAccurateIsCalculating[ch] = true;
    }

    std::free(sums);

    return period[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateFreq(Chan::E ch)
{
    float period = CalculatePeriod(ch);

    return (period == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : 1.0F / period;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float FindIntersectionWithHorLine(Chan::E ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = firstByte;
    int compValue = lastByte - 1;
    int step = SET_PEAKDET_EN ? 2 : 1;

    uint8 *data = &CHOICE_BUFFER[0];

    if(downToUp)
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] < yLine && data[x + step] >= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    else
    {
        while((num < numIntersection) && (x < compValue))
        {
            if(data[x] > yLine && data[x + step] <= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    x -= step;

    if (num < numIntersection)
    {
        return ERROR_VALUE_FLOAT;
    }
    
    return ::Math::GetIntersectionWithHorizontalLine(x, data[x], x + step, data[x + step], yLine);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDurationPlus(Chan::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)aveValue);
    }

    if (MEAS_MARKED == Measure::Type::DurationPlus)
    {
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return FPGA::Math::TShift2Abs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F), (TBase::E)TBASE_DS);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDurationMinus(Chan::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)aveValue);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)aveValue);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)aveValue);
    }

    if (MEAS_MARKED == Measure::Type::DurationMinus)
    {
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return FPGA::Math::TShift2Abs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F), TBASE_DS);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateTimeNarastaniya(Chan::E ch)   /** \todo Здесь, возможно, нужно увеличить точность - брать не целые значени расстояний между 
                                              отсчётами по времени, а рассчитывать пересечения линий. */
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)min01);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, (uint8)max09);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507
    
    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, (uint8)max09);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = FPGA::Math::TShift2Abs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F), TBASE_DS);

    if (MEAS_MARKED == Measure::Type::TimeNarastaniya)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - firstByte);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - firstByte);
    }

    return retValue;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateTimeSpada(Chan::E ch)        /// \todo Аналогично времени нарастания
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)max09);
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, (uint8)min01);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, (uint8)min01);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = FPGA::Math::TShift2Abs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F), TBASE_DS);

    if (MEAS_MARKED == Measure::Type::TimeSpada)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, (int16)firstIntersection - SHIFT_IN_MEMORY);
        Measure::SetMarkerTime(ch, 1, (int16)secondIntersection - SHIFT_IN_MEMORY);
    }

    return retValue;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateSkvaznostPlus(Chan::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationPlus(ch);
    
    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateSkvaznostMinus(Chan::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMinSteadyRel(Chan::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    uint8 *dataIn = CHOICE_BUFFER;

    if(!minSteadyIsCalculating[ch])
    {
        float aveValue = CalculateAverageRel(ch);
        if(aveValue == ERROR_VALUE_FLOAT)
        {
            min[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;

            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while(data <= end)
            {
                uint8 d = *data++;
                if(d < aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            min[ch] = (float)sum / numSums;
            int numMin = numSums;

            int numDeleted = 0;

            float pic = CalculatePicRel(ch);
            if (pic == ERROR_VALUE_FLOAT)
            {
                min[ch] = ERROR_VALUE_FLOAT;
            }
            else
            {
                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                float _min = min[ch];
                while (data <= end)
                {
                    uint8 d = *data++;

                    if (d < aveValue)
                    {
                        if (d < _min)
                        {
                            if (_min - d > value)
                            {
                                sum -= d;
                                --numSums;
                                ++numDeleted;
                            }
                        }
                        else if (d - _min > value)
                        {
                            sum -= d;
                            --numSums;
                            ++numDeleted;
                        }
                        else
                        {
                            // здесь ничего менять не нужно
                        }
                    }
                }
                min[ch] = (numDeleted > numMin / 2.0F) ? CalculateMinRel(ch) : (float)sum / numSums;
            }
        }
        minSteadyIsCalculating[ch] = true;
    }

    return min[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMaxSteadyRel(Chan::E ch)
{
    static float max[2] = {255.0F, 255.0F};

    if(!maxSteadyIsCalculating[ch])
    {
        uint8 *dataIn = CHOICE_BUFFER;

        float aveValue = CalculateAverageRel(ch);
        
        if(aveValue == ERROR_VALUE_FLOAT)
        {
            max[ch] = ERROR_VALUE_FLOAT;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[firstByte];
            const uint8 * const end = &dataIn[lastByte];
            while (data <= end)
            {
                uint8 d = *data++;
                if(d > aveValue)
                {
                    sum += d;
                    numSums++;
                }
            }
            max[ch] = (float)sum / numSums;
            int numMax = numSums;

            int numDeleted = 0;

            float pic = CalculatePicRel(ch);

            if (pic == ERROR_VALUE_FLOAT)
            {
                max[ch] = ERROR_VALUE_FLOAT;
            }
            else
            {
                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                uint8 _max = (uint8)max[ch];

                while (data <= end)
                {
                    uint8 d = *data++;
                    if (d > aveValue)
                    {
                        if (d > _max)
                        {
                            if (d - _max > value)
                            {
                                sum -= d;
                                numSums--;
                                numDeleted++;
                            }
                        }
                        else if (_max - d > value)
                        {
                            sum -= d;
                            numSums--;
                            numDeleted++;
                        }
                        else
                        {
                            // здесь ничего делать не нужно
                        }
                    }
                }
                max[ch] = (numDeleted > numMax / 2) ? CalculateMaxRel(ch) : (float)sum / numSums;
            }
        }
        maxSteadyIsCalculating[ch] = true;
    }

    return max[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMaxRel(Chan::E ch)
{
    static float max[2] = {0.0F, 0.0F};

    if(!maxIsCalculating[ch])
    {
        uint8 val = Math::MaxFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        max[ch] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        maxIsCalculating[ch] = true;
    }

    return max[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateMinRel(Chan::E ch)
{
    static float min[2] = {255.0F, 255.0F};

    if (!minIsCalculating[ch])
    {
        uint8 val = Math::MinFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        min[ch] = val == ERROR_VALUE_UINT8 ? ERROR_VALUE_FLOAT : val;
        minIsCalculating[ch] = true;
    }

    return min[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateAverageRel(Chan::E ch)
{
    static float ave[2] = {0.0F, 0.0F};

    if(!aveIsCalculating[ch])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        ave[ch] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : (min + max) / 2.0F;
        aveIsCalculating[ch] = true;
    }
    return ave[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePicRel(Chan::E ch)
{
    static float pic[2] = {0.0F, 0.0F};

    if(!picIsCalculating[ch])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        pic[ch] = (min == ERROR_VALUE_FLOAT || max == ERROR_VALUE_FLOAT) ? ERROR_VALUE_FLOAT : max - min;
        picIsCalculating[ch] = true;
    }
    return pic[ch];
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDelayPlus(Chan::E ch)
{
    float periodA = CalculatePeriod(Chan::A);
    float periodB = CalculatePeriod(Chan::B);

    EXIT_IF_ERRORS_FLOAT(periodA, periodB); //-V2507
    if(!Math::FloatsIsEquals(periodA, periodB, 1.05F))
    {
        return ERROR_VALUE_FLOAT;
    }

    float averageA = CalculateAverageRel(Chan::A);
    float averageB = CalculateAverageRel(Chan::B);

    EXIT_IF_ERRORS_FLOAT(averageA, averageB); //-V2507

    float averageFirst = Chan(ch).IsA() ? averageA : averageB;
    float averageSecond = Chan(ch).IsA() ? averageB : averageA;
    Chan::E firstChannel = ch;
    Chan::E secondChannel = Chan(ch).IsA() ? Chan::B : Chan::A;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, (uint8)averageFirst);
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, (uint8)averageSecond);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return FPGA::Math::TShift2Abs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F), TBASE_DS);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculateDelayMinus(Chan::E ch)
{
    float period0 = CalculatePeriod(Chan::A);
    float period1 = CalculatePeriod(Chan::B);

    EXIT_IF_ERRORS_FLOAT(period0, period1); //-V2507

    if(!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return ERROR_VALUE_FLOAT;
    }

    float average0 = CalculateAverageRel(Chan::A);
    float average1 = CalculateAverageRel(Chan::B);

    EXIT_IF_ERRORS_FLOAT(average0, average1); //-V2507

    float averageFirst = Chan(ch).IsA() ? average0 : average1;
    float averageSecond = Chan(ch).IsA() ? average1 : average0;
    Chan::E firstChannel = ch;
    Chan::E secondChannel = Chan(ch).IsA() ? Chan::B : Chan::A;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, (uint8)averageFirst);
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, (uint8)averageSecond);

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if(secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, (uint8)averageSecond);
    }

    /*
    if (MEAS_MARKED == Measure_DelayMinus)
    {
        markerTime[ch][0] = (int)((int16)firstIntersection - firstByte);
        markerTime[ch][1] = (int)((int16)secondIntersection - firstByte);
    }
    */

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return FPGA::Math::TShift2Abs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F), TBASE_DS);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePhazaPlus(Chan::E ch)
{
    float delay = CalculateDelayPlus(ch);
    float period = CalculatePeriod(ch);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT)
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0F;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float CalculatePhazaMinus(Chan::E ch)
{
    float delay = CalculateDelayMinus(ch);
    float period = CalculatePeriod(ch);
    if(delay == ERROR_VALUE_FLOAT || period == ERROR_VALUE_FLOAT)
    {
        return ERROR_VALUE_FLOAT;
    }
    return delay / period * 360.0F; 
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Measure::SetData(bool /*needSmoothing*/)
{
    isSet = true;

    BitSet64 points = Display::BytesOnDisplay();
    firstByte = points.sword0;
    lastByte = points.sword1;

    nBytes = lastByte - firstByte;
    
    if(TBASE_DS >= TBase::MIN_P2P)           // Если находимся в поточечном режме, то нужно брать последние считанные точки для проведения измерений
    {
        for (int i = NUM_BYTES_DS - 1; i >= 0; --i)
        {
            if (IN_A[i] != NONE)      // Если это значение считано
            {
                lastByte = i;
                firstByte = lastByte - nBytes;
                if (firstByte < 0)
                {
                    firstByte = 0;
                    lastByte = nBytes;
                }
                break;
            }
        }
    }
    
//    int length = NUM_BYTES_DS;

    if (ENABLED_DS_A)
    {
//        Math::CalculateFiltrArray(IN_A, OUT_A, length, needSmoothing ? NUM_SMOOTHING : 1);
//        std::memcpy(IN_A, OUT_A, (uint)length);
    };
    if (ENABLED_DS_B)
    {
//        Math::CalculateFiltrArray(IN_B, OUT_B, length, needSmoothing ? NUM_SMOOTHING : 1);
//        std::memcpy(IN_B, OUT_B, (uint)length);
    };
  
    //Processing::CountedToCurrentSettings();
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Measure::CalculateCursorU(Chan::E ch, float posCurT)
{
    if(!CHOICE_BUFFER)
    {
        return 0;
    }
    
    BitSet64 points = Display::PointsOnDisplay();

    int rel = (int)(CHOICE_BUFFER)[(int)points.word0 + ROUND(int, posCurT)] - MIN;

#define SCALE (200.0F / (MAX - MIN))

    float value = 200.0F - rel * SCALE;
    LIMITATION(value, 0.0F, 200.0F); //-V2516
    return value;
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float Measure::CalculateCursorT(Chan::E ch, float posCurU, int numCur)
{
    uint8 *dataIn = CHOICE_BUFFER;

    if(!dataIn)
    {
        return 0;
    }

#define FIRST_POINT (points.sword0)
#define LAST_POINT  (points.sword1)
    
    BitSet64 points = Display::PointsOnDisplay();

    int prevData = 200 - dataIn[FIRST_POINT] + MIN;

    int numIntersections = 0;

    for(int i = FIRST_POINT + 1; i < LAST_POINT; i++)
    {
        int curData = 200 - (dataIn)[i] + MIN;

        if(curData <= posCurU && prevData > posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }

        if(curData >= posCurU && prevData < posCurU)
        {
            if(numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if(numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }
        prevData = curData;
    }
    return 0;

#undef LAST_POINT
#undef FIRST_POINT
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Processing::InterpolationSinX_X(uint8 *data, int numPoints, TBase::E tBase)
{
/*
     Последовательности x в sin(x)   // Это, наверное, неправильно
2    1. 20нс : pi/2, -pi/2 ...
8    2. 10нс : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
18   3. 5нс  : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
38   4. 2нс  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
98   5. 1нс  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
*/

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    static const int deltas[5] = {100, 50, 20, 10, 5};
    int delta = deltas[tBase];

    uint8 *signedData = (uint8 *)std::malloc((uint)numPoints / 2U);
    if (signedData == 0)
    {
        LOG_ERROR("Нет памяти");
        return;
    }
    int numSignedPoints = 0;
    
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            signedData[numSignedPoints++] = data[pos];
        }
    }

    // Найдём смещение первой значащей точки

    int shift = 0;
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            shift = pos;
            break;
        }
    }

    float deltaX = PI;
    float stepX0 = PI / (float)delta;
    float x0 = PI - stepX0;
    int num = 0;
    
    for(int i = 0; i < numPoints; i++)
    {
        x0 += stepX0;
        if((i % delta) == 0)
        {
            data[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ? std::sinf(PI / delta * (part + 1)) : std::sinf(PI / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)                 // Здесь используем более быструю, но более неправильную арифметику целвых чисел
            {
                int sinXint = (int)(sinX * MUL_SIN);
                int value = 0;
                int x = (int)((x0 - deltaX) * MUL);
                int deltaXint = (int)(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint);
                    sinXint = -sinXint;
                }
                data[i] = (uint8)(value * KOEFF);
            }
            else                                    // На этих развёртках арифметика с плавающей запятой даёт приемлемое быстродействие
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }
                data[i] = (uint8)value;
            }
        }
    }
    
    int pos = numPoints - 1;
    while (pos > shift)
    {
        data[pos] = data[pos - shift];
        pos--;
    }

    std::free(signedData);
}


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
String Processing::GetStringMeasure(Measure::Type::E measure, Chan::E ch, char* buffer, int lenBuf)
{
    if (!SET_ENABLED(ch))
    {
        return String("");
    }
    buffer[0] = '\0';
    std::strcpy(buffer, Chan(ch).IsA() ? "1: " : "2: ");
    if(!isSet || values[measure].value[ch] == ERROR_VALUE_FLOAT)
    {
        std::strcat(buffer, "-.-");
    }
    else if(sMeas[measure].FuncCalculate)
    {
        char bufferForFunc[20];
        pFuncPCFBPC func = sMeas[measure].FucnConvertate;
        float value = values[measure].value[ch];
       
        if (SET_DIVIDER_10(ch) && func == Voltage2String)
        {
            value *= 10.0F;                         // Домножаем, если включён делитель
        }
               
        char *text = func(value, sMeas[measure].showSign, bufferForFunc);
        int len = (int)std::strlen(text) + (int)std::strlen(buffer) + 1;
        if (len + 1 <= lenBuf)
        {
            std::strcat(buffer, text);
        }
        else
        {
//            LOG_ERROR_TRACE("Буфер слишком мал");
        }
    }
    else
    {
        return String(buffer);
    }
    return String(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Freq2String(float freq, bool, char buffer[20])
{
    std::strcpy(buffer, Frequency(freq).ToString().CString());
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Time2String(float time, bool always, char buffer[20])
{
    std::strcpy(buffer, Time(time).ToString(always).CString());
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Voltage2String(float voltage, bool always, char buffer[20])
{
    std::strcpy(buffer, Voltage(voltage).ToString(always).CString());
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Phase2String(float phase, bool, char buffer[20])
{
    return Phase(phase).ToString(buffer);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
char* Float2String(float value, bool always, char buffer[20])
{
    std::strcpy(buffer, Float(value).ToString(always, 4).CString());
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static float Divide(float val1, float val2)
{
    float result = val1 / val2;

    if(result == std::numeric_limits<float>::infinity())
    {
        return ERROR_VALUE_FLOAT;
    }

    if(isnan(result))
    {
        return ERROR_VALUE_FLOAT;
    }

    return result;
}
