// 2023/11/22 13:47:03 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "FPGA/FPGA.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include "Utils/Smoother.h"
#include "Utils/Values.h"
#include <cmath>
#include <cstdlib>
#include <cstring>
#include <limits>


namespace AutoMeasurements
{
    // �������� �������� ������ ch �� ��������� � ��������
    static void CountedToCurrentRShift(Chan::E ch, uint numBytes, const uint8 *in, uint8 *out);

    // ������ �� IN_A, IN_B ����������� � ������� ���������� � �������� � OUT_A, OUT_B
    static void CountedToCurrentSettings();

    static void CountedToCurrentSettings(Chan::E ch, uint numBytes, const uint8 *in, uint8 *out);

    // ������� �������� ������ � ����������� ��������
    static void LimitationData(uint8 *inOut, uint numBytes);

    int markerTime[Chan::Count][2] = { {Integer::ERROR}, {Integer::ERROR} };
    int markerVoltage[Chan::Count][2] = { {Integer::ERROR}, {Integer::ERROR} };
    int8 posActive = 0;

    typedef float (*pFuncFCh)(Chan::E);
    typedef char *(*pFuncPCFBPC)(float, bool, char *);

    typedef struct
    {
        const char *name;
        pFuncFCh    FuncCalculate;
        pFuncPCFBPC FucnConvertate;
        bool        showSign;           // ���� true, ����� ���������� ����.
        uint8       notUsed0[3];
    } MeasureCalculate;

    static float CalculateVoltageMax(Chan::E);
    static float CalculateVoltageMin(Chan::E);
    static float CalculateVoltagePic(Chan::E);
    static float CalculateVoltageMaxSteady(Chan::E);
    static float CalculateVoltageMinSteady(Chan::E);
    static float CalculateVoltageAmpl(Chan::E);
    static float CalculateVoltageAverage(Chan::E);
    static float CalculateVoltageRMS(Chan::E);
    static float CalculateVoltageVybrosPlus(Chan::E);
    static float CalculateVoltageVybrosMinus(Chan::E);
    static float CalculatePeriod(Chan::E);
    static int   CalculatePeriodAccurately(Chan::E); // ����� ��������� ������ ��� ����� ����� �������� � ������ �������.
    static float CalculateFreq(Chan::E);
    static float CalculateTimeNarastaniya(Chan::E);
    static float CalculateTimeSpada(Chan::E);
    static float CalculateDurationPlus(Chan::E);
    static float CalculateDurationMinus(Chan::E);
    static float CalculateSkvaznostPlus(Chan::E);
    static float CalculateSkvaznostMinus(Chan::E);


    static float CalculateMinRel(Chan::E);           // ���������� ����������� �������� �������������� �������.
    static float CalculateMinSteadyRel(Chan::E);     // ���������� ����������� �������������� �������� �������������� �������.
    static float CalculateMaxRel(Chan::E);           // ���������� ������������ �������� �������������� �������.
    static float CalculateMaxSteadyRel(Chan::E);     // ���������� ������������ �������������� �������� �������������� �������.
    static float CalculateAverageRel(Chan::E);       // ���������� �������� �������� �������������� �������.
    static float CalculatePicRel(Chan::E);
    static float CalculateDelayPlus(Chan::E);
    static float CalculateDelayMinus(Chan::E);
    static float CalculatePhazaPlus(Chan::E);
    static float CalculatePhazaMinus(Chan::E);

    static char *Time2String(float, bool, char buffer[20]);
    static char *Voltage2String(float, bool, char buffer[20]);
    static char *Phase2String(float, bool, char buffer[20]);

    static const MeasureCalculate sMeas[TypeMeasure::Count] =
    {
        {"", 0, 0, false, {}},
        {"CalculateVoltageMax",         CalculateVoltageMax,         Voltage2String,                    true,  {}},
        {"CalculateVoltageMin",         CalculateVoltageMin,         Voltage2String,                    true,  {}},
        {"CalculateVoltagePic",         CalculateVoltagePic,         Voltage2String,                    false, {}},
        {"CalculateVoltageMaxSteady",   CalculateVoltageMaxSteady,   Voltage2String,                    true,  {}},
        {"CalculateVoltageMinSteady",   CalculateVoltageMinSteady,   Voltage2String,                    true,  {}},
        {"CalculateVoltageAmpl",        CalculateVoltageAmpl,        Voltage2String,                    false, {}},
        {"CalculateVoltageAverage",     CalculateVoltageAverage,     Voltage2String,                    true,  {}},
        {"CalculateVoltageRMS",         CalculateVoltageRMS,         Voltage2String,                    false, {}},
        {"CalculateVoltageVybrosPlus",  CalculateVoltageVybrosPlus,  Voltage2String,                    false, {}},
        {"CalculateVoltageVybrosMinus", CalculateVoltageVybrosMinus, Voltage2String,                    false, {}},
        {"CalculatePeriod",             CalculatePeriod,             Time2String,                       false, {}},
        {"CalculateFreq",               CalculateFreq,               AutoMeasurements::Freq2String,     false, {}},
        {"CalculateTimeNarastaniya",    CalculateTimeNarastaniya,    Time2String,                       false, {}},
        {"CalculateTimeSpada",          CalculateTimeSpada,          Time2String,                       false, {}},
        {"CalculateDurationPlus",       CalculateDurationPlus,       Time2String,                       false, {}},
        {"CalculateDurationPlus",       CalculateDurationMinus,      Time2String,                       false, {}},
        {"CalculateSkvaznostPlus",      CalculateSkvaznostPlus,      AutoMeasurements::Float2String,    false, {}},
        {"CalculateSkvaznostMinus",     CalculateSkvaznostMinus,     AutoMeasurements::Float2String,    false, {}},
        {"CalculateDelayPlus",          CalculateDelayPlus,          Time2String,                       false, {}},
        {"CalculateDelayMinus",         CalculateDelayMinus,         Time2String,                       false, {}},
        {"CalculatePhazaPlus",          CalculatePhazaPlus,          Phase2String,                      false, {}},
        {"CalculatePhazaMinus",         CalculatePhazaMinus,         Phase2String,                      false, {}}
    };
}


// \brief ����� ����� ����������� ������� � �������������� ������, ���������� �� ������ yLine. numItersection - ���������� ����� �����������, 
// ���������� � 1. downToTop - ���� true, ���� ����������� ������� �� ������� ������ ��� ����������� �� "-" � "+".
static float FindIntersectionWithHorLine(Chan::E ch, int numIntersection, bool downToUp, uint8 yLine);
// ������ val1 �� val2. ���������� nan, ���� ��������� ����������
static float Divide(float val1, float val2);

static bool isSet = false;          // ���� true, �� ������ ��������.

static int firstByte = 0;
static int lastByte = 0;
// ���������� ����, �� ������� ������������ �������
static int nBytes = 0;


typedef struct
{
    float value[2];
} MeasureValue;

static MeasureValue values[TypeMeasure::Count] = { {{0.0F, 0.0F}} };

static bool maxIsCalculating[2] = { false, false };
static bool minIsCalculating[2] = { false, false };
static bool maxSteadyIsCalculating[2] = { false, false };
static bool minSteadyIsCalculating[2] = { false, false };
static bool aveIsCalculating[2] = { false, false };
static bool periodIsCaclulating[2] = { false, false };
static bool periodAccurateIsCalculating[2];
static bool picIsCalculating[2] = { false, false };

#define EXIT_IF_ERROR_FLOAT(x)      if(isnan(x))                return Float::ERROR;
#define EXIT_IF_ERRORS_FLOAT(x, y)  if(isnan(x) || isnan(y))    return Float::ERROR;
#define EXIT_IF_ERROR_INT(x)        if((x) == Integer::ERROR)   return Float::ERROR;

// ������� ����� ������ ������ ch
#define CHOICE_BUFFER (OUT(ch))


void AutoMeasurements::CalculateMeasures()
{
    if (!S_MEAS_SHOW || !isSet)
    {
        return;
    }

    maxIsCalculating[0] = false;
    maxIsCalculating[1] = false;
    maxSteadyIsCalculating[0] = false;
    maxSteadyIsCalculating[1] = false;
    minIsCalculating[0] = false;
    minIsCalculating[1] = false;
    minSteadyIsCalculating[0] = false;
    minSteadyIsCalculating[1] = false;
    aveIsCalculating[0] = false;
    aveIsCalculating[1] = false;
    periodIsCaclulating[0] = false;
    periodIsCaclulating[1] = false;
    periodAccurateIsCalculating[0] = false;
    periodAccurateIsCalculating[1] = false;
    picIsCalculating[0] = false;
    picIsCalculating[1] = false;

    for (int str = 0; str < TableMeasures::NumRows(); str++)
    {
        for (int elem = 0; elem < TableMeasures::NumCols(); elem++)
        {
            Measure measure(str, elem);
            TypeMeasure::E type = measure.GetType();

            pFuncFCh func = sMeas[static_cast<int>(type)].FuncCalculate;
            if (func)
            {
                if (type == S_MEAS_MARKED || S_MEAS_MARKED == TypeMeasure::None)
                {
                    markerTime[ChanA][0] = Integer::ERROR;
                    markerTime[ChanA][1] = Integer::ERROR;
                    markerTime[ChanB][0] = Integer::ERROR;
                    markerTime[ChanB][1] = Integer::ERROR;
                    markerVoltage[ChanA][0] = Integer::ERROR;
                    markerVoltage[ChanA][1] = Integer::ERROR;
                    markerVoltage[ChanB][0] = Integer::ERROR;
                    markerVoltage[ChanB][1] = Integer::ERROR;
                }

                if (VIEW_MEASURES_A)
                {
                    values[static_cast<int>(type)].value[ChanA] = func(ChanA);
                }

                if (VIEW_MEASURES_B)
                {
                    values[static_cast<int>(type)].value[ChanB] = func(ChanB);
                }
            }
        }
    }


    // �������� ��������� � SCPI
    if (AutoMeasuresSender::sended != TypeMeasure::Count)
    {
        TypeMeasure::E type = AutoMeasuresSender::sended;

        pFuncFCh func = sMeas[type].FuncCalculate;

        char buffer[50];

        if (func)
        {
            Measure measure(type);

            if (VIEW_MEASURES_A)
            {
                values[type].value[ChanA] = func(ChanA);
                SCPI::SendMeasure(measure.GetStringMeasure(ChanA, buffer, 49));
            }
            if (VIEW_MEASURES_B)
            {
                values[type].value[ChanB] = func(ChanB);
                SCPI::SendMeasure(measure.GetStringMeasure(ChanB, buffer, 49));
            }
        }

        AutoMeasuresSender::sended = TypeMeasure::Count;
    }
}



float AutoMeasurements::CalculateVoltageMax(Chan::E ch)
{
    float max = CalculateMaxRel(ch);
    EXIT_IF_ERROR_FLOAT(max); //-V2507
    if (S_MEAS_MARKED == TypeMeasure::VoltageMax)
    {
        Measure::SetMarkerVoltage(ch, 0, max);      // ����� �� ���������, ������ ��� max ����� ���� ������ �����
    }

    Range::E range = RANGE_DS(ch);

    int16 rShift = RSHIFT_DS(ch);

    uint8 value = ROUND(uint8, max);

    return VALUE::ToVoltage(value, range, rShift);
}



float AutoMeasurements::CalculateVoltageMin(Chan::E ch)
{
    float min = CalculateMinRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if (S_MEAS_MARKED == TypeMeasure::VoltageMin)
    {
        Measure::SetMarkerVoltage(ch, 0, min);             // ����� �� ���������, ������ ��� min ����� ���� ������ �����
    }

    return VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch), RSHIFT_DS(ch));
}



float AutoMeasurements::CalculateVoltagePic(Chan::E ch)
{
    float max = CalculateVoltageMax(ch);
    float min = CalculateVoltageMin(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if (S_MEAS_MARKED == TypeMeasure::VoltagePic)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinRel(ch));
    }
    return max - min;
}



float AutoMeasurements::CalculateVoltageMinSteady(Chan::E ch)
{
    float min = CalculateMinSteadyRel(ch);
    EXIT_IF_ERROR_FLOAT(min); //-V2507
    if (S_MEAS_MARKED == TypeMeasure::VoltageMinSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, ROUND(float, min));
    }

    return VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch), RSHIFT_DS(ch));
}



float AutoMeasurements::CalculateVoltageMaxSteady(Chan::E ch)
{
    float max = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERROR_FLOAT(max); //-V2507

    if (S_MEAS_MARKED == TypeMeasure::VoltageMaxSteady)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
    }

    return VALUE::ToVoltage(ROUND(uint8, max), RANGE_DS(ch), RSHIFT_DS(ch));
}



float AutoMeasurements::CalculateVoltageVybrosPlus(Chan::E ch)
{
    float max = CalculateMaxRel(ch);
    float maxSteady = CalculateMaxSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(max, maxSteady); //-V2507

    if (S_MEAS_MARKED == TypeMeasure::VoltageVybrosPlus)
    {
        Measure::SetMarkerVoltage(ch, 0, max);
        Measure::SetMarkerVoltage(ch, 1, maxSteady);
    }

    int16 rShift = RSHIFT_DS(ch);
    return std::fabsf(VALUE::ToVoltage(ROUND(uint8, maxSteady), RANGE_DS(ch), rShift) - VALUE::ToVoltage(ROUND(uint8, max), RANGE_DS(ch), rShift));
}



float AutoMeasurements::CalculateVoltageVybrosMinus(Chan::E ch)
{
    float min = CalculateMinRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);
    EXIT_IF_ERRORS_FLOAT(min, minSteady); //-V2507

    if (S_MEAS_MARKED == TypeMeasure::VoltageVybrosMinus)
    {
        Measure::SetMarkerVoltage(ch, 0, min);
        Measure::SetMarkerVoltage(ch, 1, minSteady);
    }

    int16 rShift = RSHIFT_DS(ch);
    return std::fabsf(VALUE::ToVoltage(ROUND(uint8, minSteady), RANGE_DS(ch), rShift) - VALUE::ToVoltage(ROUND(uint8, min), RANGE_DS(ch), rShift));
}



float AutoMeasurements::CalculateVoltageAmpl(Chan::E ch)
{
    float max = CalculateVoltageMaxSteady(ch);
    float min = CalculateVoltageMinSteady(ch);

    EXIT_IF_ERRORS_FLOAT(min, max); //-V2507

    if (S_MEAS_MARKED == TypeMeasure::VoltageAmpl)
    {
        Measure::SetMarkerVoltage(ch, 0, CalculateMaxSteadyRel(ch));
        Measure::SetMarkerVoltage(ch, 1, CalculateMinSteadyRel(ch));
    }
    return max - min;
}



float AutoMeasurements::CalculateVoltageAverage(Chan::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    int sum = 0;

    uint8 *data = &CHOICE_BUFFER[firstByte];

    for (int i = 0; i < period; i++)
    {
        sum += *data++;
    }

    uint8 aveRel = static_cast<uint8>(static_cast<float>(sum) / period);

    if (S_MEAS_MARKED == TypeMeasure::VoltageAverage)
    {
        Measure::SetMarkerVoltage(ch, 0, aveRel);
    }

    return VALUE::ToVoltage(aveRel, RANGE_DS(ch), RSHIFT_DS(ch));
}



float AutoMeasurements::CalculateVoltageRMS(Chan::E ch)
{
    int period = CalculatePeriodAccurately(ch);

    EXIT_IF_ERROR_INT(period); //-V2507

    float rms = 0.0F;

    Range::E range = RANGE_DS(ch);
    int16 rShift = RSHIFT_DS(ch);

    uint8 *dataIn = &CHOICE_BUFFER[firstByte];

    for (int i = firstByte; i < firstByte + period; i++)
    {
        float volts = VALUE::ToVoltage(dataIn[i], range, rShift);
        rms += volts * volts;
    }

    rms = std::sqrtf(rms / period);

    if (S_MEAS_MARKED == TypeMeasure::VoltageRMS)
    {
        Measure::SetMarkerVoltage(ch, 0, VALUE::FromVoltage(rms, range, rShift));
    }

    return rms;
}



float AutoMeasurements::CalculatePeriod(Chan::E ch)
{
    static float period[2] = { 0.0F, 0.0F };

    static float firstIntersection = 0.0F;
    static float secondIntersection = 0.0F;

    if (!periodIsCaclulating[static_cast<int>(ch)])
    {
        float aveValue = CalculateAverageRel(ch);
        if (aveValue == Uint8::ERROR)
        {
            period[static_cast<int>(ch)] = Float::ERROR;
        }
        else
        {
            float intersectionDownToTop = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));
            float intersectionTopToDown = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(intersectionDownToTop, intersectionTopToDown); //-V2507

            bool firstDownToTop = intersectionDownToTop < intersectionTopToDown;
            firstIntersection = firstDownToTop ? intersectionDownToTop : intersectionTopToDown;
            secondIntersection = FindIntersectionWithHorLine(ch, 2, firstDownToTop, static_cast<uint8>(aveValue));

            EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

            float per = TShift::ToAbs(ROUND(uint16, secondIntersection - firstIntersection), S_TIME_BASE);

            period[static_cast<int>(ch)] = per;

            periodIsCaclulating[static_cast<int>(ch)] = true;
        }
    }

    if ((S_MEAS_MARKED == TypeMeasure::Period || S_MEAS_MARKED == TypeMeasure::Freq) && periodIsCaclulating[static_cast<int>(ch)])
    {
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    return period[static_cast<int>(ch)];
}



#define EXIT_FROM_PERIOD_ACCURACY                               \
    period[static_cast<int>(ch)] = Integer::ERROR;              \
    periodAccurateIsCalculating[static_cast<int>(ch)] = true;   \
    std::free(sums);                                            \
    return period[static_cast<int>(ch)];



int AutoMeasurements::CalculatePeriodAccurately(Chan::E ch)
{
    static int period[2];

    int *sums = static_cast<int *>(std::malloc(static_cast<uint>(nBytes)));

    if (sums == 0)
    {
        return 0;
    }

    uint8 *dataIn = CHOICE_BUFFER;

    if (!periodAccurateIsCalculating[static_cast<int>(ch)])
    {
        period[static_cast<int>(ch)] = 0;

        float pic = CalculatePicRel(ch);

        if (pic == Float::ERROR)
        {
            EXIT_FROM_PERIOD_ACCURACY
        }
        int delta = static_cast<int>(pic * 5.0F);
        sums[firstByte] = dataIn[firstByte];

        int i = firstByte + 1;
        int *pSum = &sums[i];
        uint8 *data = &dataIn[i];
        uint8 *end = &dataIn[lastByte];

        while (data < end)
        {
            uint8 point = *data++;
            if (point < VALUE::MIN || point >= VALUE::MAX)
            {
                EXIT_FROM_PERIOD_ACCURACY
            }
            *pSum = *(pSum - 1) + point;
            pSum++;
        }

        int addShift = firstByte - 1;
        int maxPeriod = static_cast<int>(nBytes * 0.95F);

        for (int nextPeriod = 10; nextPeriod < maxPeriod; nextPeriod++)
        {
            int sum = sums[addShift + nextPeriod];

            int maxDelta = 0;
            int maxStart = nBytes - nextPeriod;

            int *pSums = &sums[firstByte + 1];

            for (int start = 1; start < maxStart; start++)
            {
                int nextSum = *(pSums + nextPeriod) - (*pSums);
                pSums++;

                int nextDelta = nextSum - sum;

                if (nextSum < sum)
                {
                    nextDelta = -nextDelta;
                }

                if (nextDelta > delta)
                {
                    maxDelta = delta + 1;
                    break;
                }
                else if (nextDelta > maxDelta) //-V2516
                {
                    maxDelta = nextDelta;
                }
            }

            if (maxDelta < delta)
            {
                delta = maxDelta;
                period[static_cast<int>(ch)] = nextPeriod;
            }
        }

        if (period[static_cast<int>(ch)] == 0)
        {
            period[static_cast<int>(ch)] = Integer::ERROR;
        }
        periodAccurateIsCalculating[static_cast<int>(ch)] = true;
    }

    std::free(sums);

    return period[static_cast<int>(ch)];
}



float AutoMeasurements::CalculateFreq(Chan::E ch)
{
    float period = CalculatePeriod(ch);

    return (period == Float::ERROR) ? Float::ERROR : 1.0F / period;
}



float FindIntersectionWithHorLine(Chan::E ch, int numIntersection, bool downToUp, uint8 yLine)
{
    int num = 0;
    int x = firstByte;
    int compValue = lastByte - 1;
    int step = PEAKDET_ENABLED(DS) ? 2 : 1;

    uint8 *data = &CHOICE_BUFFER[0];

    if (downToUp)
    {
        while ((num < numIntersection) && (x < compValue))
        {
            if (data[x] < yLine && data[x + step] >= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    else
    {
        while ((num < numIntersection) && (x < compValue))
        {
            if (data[x] > yLine && data[x + step] <= yLine)
            {
                num++;
            }
            x += step;
        }
    }
    x -= step;

    if (num < numIntersection)
    {
        return Float::ERROR;
    }

    return ::Math::GetIntersectionWithHorizontalLine(x, data[x], x + step, data[x + step], yLine);
}



float AutoMeasurements::CalculateDurationPlus(Chan::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, static_cast<uint8>(aveValue));
    }

    if (S_MEAS_MARKED == TypeMeasure::DurationPlus)
    {
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), (TBase::E)TBASE_DS);
}



float AutoMeasurements::CalculateDurationMinus(Chan::E ch)
{
    float aveValue = CalculateAverageRel(ch);
    EXIT_IF_ERROR_FLOAT(aveValue); //-V2507

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(aveValue));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(aveValue));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, static_cast<uint8>(aveValue));
    }

    if (S_MEAS_MARKED == TypeMeasure::DurationMinus)
    {
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float AutoMeasurements::CalculateTimeNarastaniya(Chan::E ch)   /** \todo �����, ��������, ����� ��������� �������� - ����� �� ����� ������� ���������� ����� 
                                              ��������� �� �������, � ������������ ����������� �����. */
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(min01));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, true, static_cast<uint8>(max09));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, true, static_cast<uint8>(max09));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);

    if (S_MEAS_MARKED == TypeMeasure::TimeNarastaniya)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - firstByte);
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - firstByte);
    }

    return retValue;
}



float AutoMeasurements::CalculateTimeSpada(Chan::E ch)        // \todo ���������� ������� ����������
{
    float maxSteady = CalculateMaxSteadyRel(ch);
    float minSteady = CalculateMinSteadyRel(ch);

    EXIT_IF_ERRORS_FLOAT(maxSteady, minSteady); //-V2507

    float value01 = (maxSteady - minSteady) * 0.1F;
    float max09 = maxSteady - value01;
    float min01 = minSteady + value01;

    float firstIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(max09));
    float secondIntersection = FindIntersectionWithHorLine(ch, 1, false, static_cast<uint8>(min01));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(ch, 2, false, static_cast<uint8>(min01));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    float retValue = TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);

    if (S_MEAS_MARKED == TypeMeasure::TimeSpada)
    {
        Measure::SetMarkerVoltage(ch, 0, max09);
        Measure::SetMarkerVoltage(ch, 1, min01);
        Measure::SetMarkerTime(ch, 0, static_cast<int16>(firstIntersection) - DisplayOsci::ShiftInMemory::Get());
        Measure::SetMarkerTime(ch, 1, static_cast<int16>(secondIntersection) - DisplayOsci::ShiftInMemory::Get());
    }

    return retValue;
}



float AutoMeasurements::CalculateSkvaznostPlus(Chan::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationPlus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}



float AutoMeasurements::CalculateSkvaznostMinus(Chan::E ch)
{
    float period = CalculatePeriod(ch);
    float duration = CalculateDurationMinus(ch);

    EXIT_IF_ERRORS_FLOAT(period, duration); //-V2507

    return Divide(period, duration);
}



float AutoMeasurements::CalculateMinSteadyRel(Chan::E ch)
{
    static float min[2] = { 255.0F, 255.0F };

    uint8 *dataIn = CHOICE_BUFFER;

    if (!minSteadyIsCalculating[static_cast<int>(ch)])
    {
        float aveValue = CalculateAverageRel(ch);
        if (aveValue == Float::ERROR)
        {
            min[static_cast<int>(ch)] = Float::ERROR;
        }
        else
        {
            int sum = 0;
            int numSums = 0;

            uint8 *data = &dataIn[firstByte];
            const uint8 *const end = &dataIn[lastByte];
            while (data <= end)
            {
                uint8 d = *data++;
                if (d < aveValue)
                {
                    sum += d; //-V127
                    numSums++; //-V127
                }
            }
            min[static_cast<int>(ch)] = static_cast<float>(sum) / numSums;

            float pic = CalculatePicRel(ch);
            if (pic == Float::ERROR)
            {
                min[static_cast<int>(ch)] = Float::ERROR;
            }
            else
            {
                int numDeleted = 0;

                int numMin = numSums;

                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                float _min = min[static_cast<int>(ch)];
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
                                ++numDeleted; //-V127
                            }
                        }
                        else if (d - _min > value) //-V2516
                        {
                            sum -= d;
                            --numSums;
                            ++numDeleted; //-V127
                        }
                    }
                }
                min[static_cast<int>(ch)] = (numDeleted > numMin / 2.0F) ? CalculateMinRel(ch) : static_cast<float>(sum) / numSums;
            }
        }
        minSteadyIsCalculating[static_cast<int>(ch)] = true;
    }

    return min[static_cast<int>(ch)];
}



float AutoMeasurements::CalculateMaxSteadyRel(Chan::E ch)
{
    static float max[2] = { 255.0F, 255.0F };

    if (!maxSteadyIsCalculating[static_cast<int>(ch)])
    {
        uint8 *dataIn = CHOICE_BUFFER;

        float aveValue = CalculateAverageRel(ch);

        if (aveValue == Float::ERROR)
        {
            max[static_cast<int>(ch)] = Float::ERROR;
        }
        else
        {
            int sum = 0;
            int numSums = 0;
            uint8 *data = &dataIn[firstByte];
            const uint8 *const end = &dataIn[lastByte];
            while (data <= end)
            {
                uint8 d = *data++;
                if (d > aveValue)
                {
                    sum += d; //-V127
                    numSums++; //-V127
                }
            }
            max[static_cast<int>(ch)] = static_cast<float>(sum) / numSums;

            float pic = CalculatePicRel(ch);

            if (pic == Float::ERROR)
            {
                max[static_cast<int>(ch)] = Float::ERROR;
            }
            else
            {
                int numDeleted = 0;

                int numMax = numSums;

                float value = pic / 9.0F;

                data = &dataIn[firstByte];
                uint8 _max = static_cast<uint8>(max[static_cast<int>(ch)]);

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
                                numDeleted++; //-V127
                            }
                        }
                        else if (_max - d > value) //-V2516
                        {
                            sum -= d;
                            numSums--;
                            numDeleted++; //-V127
                        }
                    }
                }
                max[static_cast<int>(ch)] = (numDeleted > numMax / 2) ? CalculateMaxRel(ch) : static_cast<float>(sum) / numSums;
            }
        }
        maxSteadyIsCalculating[static_cast<int>(ch)] = true;
    }

    return max[static_cast<int>(ch)];
}



float AutoMeasurements::CalculateMaxRel(Chan::E ch)
{
    static float max[2] = { 0.0F, 0.0F };

    if (!maxIsCalculating[static_cast<int>(ch)])
    {
        uint8 val = Math::MaxFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        max[static_cast<int>(ch)] = (val == Uint8::ERROR) ? Float::ERROR : val;
        maxIsCalculating[static_cast<int>(ch)] = true;
    }

    return max[static_cast<int>(ch)];
}



float AutoMeasurements::CalculateMinRel(Chan::E ch)
{
    static float min[2] = { 255.0F, 255.0F };

    if (!minIsCalculating[static_cast<int>(ch)])
    {
        uint8 val = Math::MinFromArrayWithErrorCode(CHOICE_BUFFER, firstByte, lastByte);
        min[static_cast<int>(ch)] = (val == Uint8::ERROR) ? Float::ERROR : val;
        minIsCalculating[static_cast<int>(ch)] = true;
    }

    return min[static_cast<int>(ch)];
}



float AutoMeasurements::CalculateAverageRel(Chan::E ch)
{
    static float _ave[2] = { 0.0F, 0.0F };

    if (!aveIsCalculating[static_cast<int>(ch)])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        _ave[static_cast<int>(ch)] = (min == Float::ERROR || max == Float::ERROR) ? Float::ERROR : (min + max) / 2.0F;
        aveIsCalculating[static_cast<int>(ch)] = true;
    }
    return _ave[static_cast<int>(ch)];
}



float AutoMeasurements::CalculatePicRel(Chan::E ch)
{
    static float pic[2] = { 0.0F, 0.0F };

    if (!picIsCalculating[static_cast<int>(ch)])
    {
        float min = CalculateMinRel(ch);
        float max = CalculateMaxRel(ch);
        pic[static_cast<int>(ch)] = (min == Float::ERROR || max == Float::ERROR) ? Float::ERROR : max - min;
        picIsCalculating[static_cast<int>(ch)] = true;
    }

    return pic[static_cast<int>(ch)];
}



float AutoMeasurements::CalculateDelayPlus(Chan::E ch)
{
    float periodA = CalculatePeriod(ChanA);
    float periodB = CalculatePeriod(ChanB);

    EXIT_IF_ERRORS_FLOAT(periodA, periodB); //-V2507
    if (!Math::FloatsIsEquals(periodA, periodB, 1.05F))
    {
        return Float::ERROR;
    }

    float averageA = CalculateAverageRel(ChanA);
    float averageB = CalculateAverageRel(ChanB);

    EXIT_IF_ERRORS_FLOAT(averageA, averageB); //-V2507

    float averageFirst = (ch == ChanA) ? averageA : averageB;
    float averageSecond = (ch == ChanA) ? averageB : averageA;
    Chan::E firstChannel = ch;
    Chan::E secondChannel = (ch == ChanA) ? ChanB : ChanA;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, true, static_cast<uint8>(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, true, static_cast<uint8>(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, true, static_cast<uint8>(averageSecond));
    }

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float AutoMeasurements::CalculateDelayMinus(Chan::E ch)
{
    float period0 = CalculatePeriod(ChanA);
    float period1 = CalculatePeriod(ChanB);

    EXIT_IF_ERRORS_FLOAT(period0, period1); //-V2507

    if (!Math::FloatsIsEquals(period0, period1, 1.05F))
    {
        return Float::ERROR;
    }

    float average0 = CalculateAverageRel(ChanA);
    float average1 = CalculateAverageRel(ChanB);

    EXIT_IF_ERRORS_FLOAT(average0, average1); //-V2507

    float averageFirst = (ch == ChanA) ? average0 : average1;
    float averageSecond = (ch == ChanA) ? average1 : average0;
    Chan::E firstChannel = ch;
    Chan::E secondChannel = (ch == ChanA) ? ChanB : ChanA;

    float firstIntersection = FindIntersectionWithHorLine(firstChannel, 1, false, static_cast<uint8>(averageFirst));
    float secondIntersection = FindIntersectionWithHorLine(secondChannel, 1, false, static_cast<uint8>(averageSecond));

    EXIT_IF_ERRORS_FLOAT(firstIntersection, secondIntersection); //-V2507

    if (secondIntersection < firstIntersection)
    {
        secondIntersection = FindIntersectionWithHorLine(secondChannel, 2, false, static_cast<uint8>(averageSecond));
    }

    /*
    if (MEAS_MARKED == Measure_DelayMinus)
    {
        markerTime[ch][0] = static_cast<int>((int16)firstIntersection - firstByte);
        markerTime[ch][1] = static_cast<int>((int16)secondIntersection - firstByte);
    }
    */

    EXIT_IF_ERROR_FLOAT(secondIntersection); //-V2507

    return TShift::ToAbs(ROUND(uint16, (secondIntersection - firstIntersection) / 2.0F * 2.0F), TBASE_DS);
}



float AutoMeasurements::CalculatePhazaPlus(Chan::E ch)
{
    float delay = CalculateDelayPlus(ch);
    float period = CalculatePeriod(ch);
    if (delay == Float::ERROR || period == Float::ERROR)
    {
        return Float::ERROR;
    }
    return delay / period * 360.0F;
}



float AutoMeasurements::CalculatePhazaMinus(Chan::E ch)
{
    float delay = CalculateDelayMinus(ch);
    float period = CalculatePeriod(ch);

    float result = Float::ERROR;

    if (delay != Float::ERROR && period != Float::ERROR)
    {
        result = delay / period * 360.0F;
    }

    return result;
}


float Measure::CalculateCursorU(Chan::E ch, float posCurT)
{
    if (!CHOICE_BUFFER)
    {
        return 0;
    }

    BitSet64 points = DisplayOsci::PainterData::PointsOnDisplay();

    int rel = static_cast<int>((CHOICE_BUFFER)[static_cast<int>(points.word0) + ROUND(int, posCurT)]) - VALUE::MIN;

#define SCALE (200.0F / (VALUE::MAX - VALUE::MIN))

    float value = 200.0F - rel * SCALE;
    LIMITATION(value, 0.0F, 200.0F);
    return value;
}



float Measure::CalculateCursorT(Chan::E ch, float posCurU, int numCur)
{
    uint8 *dataIn = CHOICE_BUFFER;

    if (!dataIn)
    {
        return 0;
    }

#define FIRST_POINT (points.sword0)
#define LAST_POINT  (points.sword1)

    BitSet64 points = DisplayOsci::PainterData::PointsOnDisplay();

    int prevData = 200 - dataIn[FIRST_POINT] + VALUE::MIN;

    int numIntersections = 0;

    for (int i = FIRST_POINT + 1; i < LAST_POINT; i++)
    {
        int curData = 200 - (dataIn)[i] + VALUE::MIN;

        if (curData <= posCurU && prevData > posCurU)
        {
            if (numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if (numIntersections == 0)
                {
                    numIntersections++;
                }
                else
                {
                    return (float)(i - FIRST_POINT);
                }
            }
        }

        if (curData >= posCurU && prevData < posCurU)
        {
            if (numCur == 0)
            {
                return (float)(i - FIRST_POINT);
            }
            else
            {
                if (numIntersections == 0)
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



void InterpolationSinX_X(uint8 *data, int numPoints, TBase::E tBase)
{
    /*
         ������������������ x � sin(x)   // ���, ��������, �����������
    2    1. 20�� : pi/2, -pi/2 ...
    8    2. 10�� : pi/5, pi/5 * 2, pi/5 * 3, pi/5 * 4, -pi/5 * 4, -pi/5 * 3, -pi/5 * 2, -pi/5 ...
    18   3. 5��  : pi/10, pi/10 * 2 ... pi/10 * 9, -pi/10 * 9 .... -pi/10 * 2, -pi/10 ...
    38   4. 2��  : pi/20, pi/20 * 2 ... pi/20 * 19, -pi/20 * 19 ... -pi/20 * 2, -pi/20 ...
    98   5. 1��  : pi/50, pi/50 * 2 ... pi/50 * 49, -pi/50 * 49 ... -pi/50 * 2, -pi/50 ...
    */

#define MUL_SIN 1e7F
#define MUL     1e6F
#define KOEFF   (MUL / MUL_SIN)

    static const int deltas[5] = { 100, 50, 20, 10, 5 };
    int delta = deltas[static_cast<int>(tBase)];

    uint8 *signedData = static_cast<uint8 *>(std::malloc(static_cast<uint>(numPoints) / 2U));
    if (signedData == 0)
    {
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

    // ����� �������� ������ �������� �����

    int shift = 0;
    for (int pos = 0; pos < numPoints; pos++)
    {
        if (data[pos] > 0)
        {
            shift = pos;
            break;
        }
    }

    float deltaX = Math::PI_F;
    float stepX0 = Math::PI_F / static_cast<float>(delta);
    float x0 = Math::PI_F - stepX0;
    int num = 0;

    for (int i = 0; i < numPoints; i++)
    {
        x0 += stepX0;
        if ((i % delta) == 0)
        {
            data[i] = signedData[i / delta];
        }
        else
        {
            int part = num % ((delta - 1) * 2);
            num++;
            float sinX = (part < delta - 1) ? std::sinf(Math::PI_F / delta * (part + 1)) : std::sinf(Math::PI_F / delta * (part - (delta - 1) * 2));

            if (tBase > TBase::_5ns)                 // ����� ���������� ����� �������, �� ����� ������������ ���������� ������ �����
            {
                int sinXint = static_cast<int>(sinX * MUL_SIN);
                int value = 0;
                int x = static_cast<int>((x0 - deltaX) * MUL);
                int deltaXint = static_cast<int>(deltaX * MUL);

                for (int n = 0; n < numSignedPoints; n++)
                {
                    value += signedData[n] * sinXint / (x - n * deltaXint);
                    sinXint = -sinXint;
                }
                data[i] = (uint8)(value * KOEFF);
            }
            else                                    // �� ���� ��������� ���������� � ��������� ������� ��� ���������� ��������������
            {
                float value = 0.0F;
                float x = x0;

                for (int n = 0; n < numSignedPoints; n++)
                {
                    x -= deltaX;
                    value += signedData[n] * sinX / x;
                    sinX = -sinX;
                }
                data[i] = static_cast<uint8>(value);
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



String Measure::GetStringMeasure(Chan::E ch, char *buffer, int lenBuf)
{
    TypeMeasure::E type = GetType();

    if (!S_CHANNEL_ENABLED(ch))
    {
        return String("");
    }

    buffer[0] = '\0';
    std::strcpy(buffer, (ch == ChanA) ? "1: " : "2: "); //-V2513

    if (!isSet || values[static_cast<int>(type)].value[static_cast<int>(ch)] == Float::ERROR)
    {
        std::strcat(buffer, "-.-"); //-V2513
    }
    else if (AutoMeasurements::sMeas[static_cast<int>(type)].FuncCalculate)
    {
        char bufferForFunc[20];
        AutoMeasurements::pFuncPCFBPC func = AutoMeasurements::sMeas[static_cast<int>(type)].FucnConvertate;
        float value = values[static_cast<int>(type)].value[static_cast<int>(ch)];

        if ((S_DIVIDER(ch) == Divider::_10) && (func == AutoMeasurements::Voltage2String))
        {
            value *= 10.0F;                         // ���������, ���� ������� ��������
        }

        char *text = func(value, AutoMeasurements::sMeas[static_cast<int>(type)].showSign, bufferForFunc);
        int len = static_cast<int>(std::strlen(text)) + static_cast<int>(std::strlen(buffer)) + 1; //-V2513
        if (len + 1 <= lenBuf)
        {
            std::strcat(buffer, text); //-V2513
        }
    }
    else
    {
        return String(buffer);
    }

    return String(buffer);
}


char *AutoMeasurements::Freq2String(float freq, bool, char buffer[20])
{
    std::strcpy(buffer, Frequency(freq).ToString().c_str()); //-V2513
    return buffer;
}


char *AutoMeasurements::Time2String(float time, bool always, char buffer[20])
{
    std::strcpy(buffer, Time(time).ToString(always).c_str()); //-V2513
    return buffer;
}


char *AutoMeasurements::Voltage2String(float voltage, bool always, char buffer[20])
{
    std::strcpy(buffer, Voltage(voltage).ToString(always).c_str()); //-V2513
    return buffer;
}


char *AutoMeasurements::Phase2String(float phase, bool, char buffer[20])
{
    return Phase(phase).ToString(buffer);
}


char *AutoMeasurements::Float2String(float value, bool always, char buffer[20])
{
    std::strcpy(buffer, Float(value).ToString(always, 4).c_str()); //-V2513
    return buffer;
}


static float Divide(float val1, float val2)
{
    float result = val1 / val2;

    if (result == std::numeric_limits<float>::infinity())
    {
        result = Float::ERROR;
    }
    else if (isnan(result)) //-V2516
    {
        result = Float::ERROR;
    }

    return result;
}


bool AutoMeasurements::DataIsSetting()
{
    return isSet;
}


void AutoMeasurements::SetData()
{
    isSet = (DS != nullptr);

    if (isSet)
    {
        CountedToCurrentSettings();

        BitSet64 points = DisplayOsci::PainterData::BytesOnDisplay();
        firstByte = points.sword0;
        lastByte = points.sword1;
        nBytes = lastByte - firstByte;

        if (TBASE_DS >= TBase::MIN_P2P)             // ���� ��������� � ���������� �����, �� ����� ����� ��������� ��������� ����� ��� ���������� ���������
        {
            for (int i = static_cast<int>(BYTES_IN_CHANNEL_DS - 1); i >= 0; --i)
            {
                if (IN_A[i] != VALUE::NONE)                // ���� ��� �������� �������
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
    }
}


Measure AutoMeasurements::GetActiveMeasure()
{
    int row = posActive / TableMeasures::NumCols();
    int col = posActive - row * TableMeasures::NumCols();

    return Measure(row, col);
}


void AutoMeasurements::CountedToCurrentSettings()
{
    const uint NUM_BYTES = BYTES_IN_CHANNEL_DS;

    if (ENABLED_DS_A)
    {
        Smoother::Run(IN_A, OUT_A, NUM_BYTES, S_DISP_NUM_SMOOTH);
        std::memcpy(IN_A, OUT_A, NUM_BYTES);
        CountedToCurrentSettings(ChanA, NUM_BYTES, IN_A, OUT_A);
        LimitationData(OUT_A, NUM_BYTES);
    }

    if (ENABLED_DS_B)
    {
        Smoother::Run(IN_B, OUT_B, NUM_BYTES, S_DISP_NUM_SMOOTH);
        std::memcpy(IN_B, OUT_B, NUM_BYTES);
        CountedToCurrentSettings(ChanB, NUM_BYTES, IN_B, OUT_B);
        LimitationData(OUT_B, NUM_BYTES);
    }
}


void AutoMeasurements::CountedToCurrentSettings(Chan::E ch, uint numBytes, const uint8 *in, uint8 *out)
{
    CountedToCurrentRShift(ch, numBytes, in, out);
}


void AutoMeasurements::LimitationData(uint8 *inOut, uint numBytes)
{
    uint8 *data = inOut;

    for (uint i = 0; i < numBytes; i++)
    {
        if (*data != VALUE::NONE)
        {
            Math::Limitation<uint8>(data, VALUE::MIN, VALUE::MAX);
        }
        data++;
    }
}


void AutoMeasurements::CountedToCurrentRShift(Chan::E ch, uint numBytes, const uint8 *in, uint8 *out)
{
    int16 shiftDS = RSHIFT_DS(ch);
    Range::E rangeDS = RANGE_DS(ch);

    int16 shiftSET = S_RSHIFT(ch);
    Range::E rangeSET = S_RANGE(ch);

    if ((shiftDS == shiftSET) && (rangeDS == rangeSET))
    {
        std::memcpy(out, in, numBytes);
    }
    else
    {
        for (uint i = 0; i < numBytes; i++)
        {
            float voltage = VALUE::ToVoltage(in[i], rangeDS, shiftDS);
            out[i] = VALUE::FromVoltage(voltage, rangeSET, shiftSET);
        }
    }
}


TypeMeasure::E AutoMeasuresSender::sended = TypeMeasure::Count;

void AutoMeasuresSender::DesignateForSending(TypeMeasure::E type)
{
    sended = type;
}
