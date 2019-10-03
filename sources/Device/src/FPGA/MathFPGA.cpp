#include "defines.h"
#include "Display/Grid.h"
#include "FPGA/FPGA.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cmath>


static const float absStepRShift[] =
{
    2e-3F   / 20 / STEP_RSHIFT,
    5e-3F   / 20 / STEP_RSHIFT,
    10e-3F  / 20 / STEP_RSHIFT,
    20e-3F  / 20 / STEP_RSHIFT,
    50e-3F  / 20 / STEP_RSHIFT,
    100e-3F / 20 / STEP_RSHIFT,
    200e-3F / 20 / STEP_RSHIFT,
    500e-3F / 20 / STEP_RSHIFT,
    1.0F    / 20 / STEP_RSHIFT,
    2.0F    / 20 / STEP_RSHIFT,
    5.0F    / 20 / STEP_RSHIFT,
    10.0F   / 20 / STEP_RSHIFT,
    20.0F   / 20 / STEP_RSHIFT
};

static const float absStepTShift[TBase::Size] =
{
    /* 2 нс    */ 2e-9F   / 20, 
    /* 5 нс    */ 5e-9F   / 20, 
    /* 10 нс   */ 10e-9F  / 20, 
    /* 20 нс   */ 20e-9F  / 20,
    /* 50 нс   */ 50e-9F  / 20,   // 1.0  Это коэффициенты для реализации алгоритма прореживания отсчётов
    /* 100 нс  */ 100e-9F / 20,   // 2.0
    /* 200 нс  */ 200e-9F / 20,   // 4.0
    /* 500 нс  */ 500e-9F / 20,   // 10.0
    /* 1 мкс   */ 1e-6F   / 20,   // 20.0
    /* 2 мкс   */ 2e-6F   / 20,   // 40.0
    /* 5 мкс   */ 5e-6F   / 20,   // 100.0
    /* 10 мкс  */ 10e-6F  / 20,   // 200.0
    /* 20 мкс  */ 20e-6F  / 20,   // 400.0
    /* 50 мкс  */ 50e-6F  / 20,   // 1e3
    /* 100 мкс */ 100e-6F / 20,   // 2e3
    /* 200 мкс */ 200e-6F / 20,   // 4e3
    /* 500 мкс */ 500e-6F / 20,   // 10e3
    /* 1 мс    */ 1e-3F   / 20,   // 20e3
    /* 2 мс    */ 2e-3F   / 20,   // 40e3
    /* 5 мс    */ 5e-3F   / 20,   // 100e3
    /* 10 мс   */ 10e-3F  / 20,   // 200e3
    /* 20 мс   */ 20e-3F  / 20,   // 400e3
    /* 50 мс   */ 50e-3F  / 20,   // 1e4
    /* 100 мс  */ 100e-3F / 20,   // 2e4
    /* 200 мс  */ 200e-3F / 20,   // 4e4
    /* 500 мс  */ 500e-3F / 20,   // 10e4
    /* 1 с     */ 1.0F    / 20,   // 20e4
    /* 2 с     */ 2.0F    / 20,   // 40e4
    /* 5 с     */ 5.0F    / 20,   // 100e4
    /* 10 с    */ 10.0F   / 20    // 200e4
};


/// Столько вольт содержится в одной точке сигнала по вертикали

//DEF__STRUCT(StructInPoints, float) voltsInPoint[Range::Size] =
static const float voltsInPoint[Range::Size] =
{
    2e-3F   / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 2mV
    5e-3F   / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 5mV
    10e-3F  / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 10mV
    20e-3F  / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 20mV
    50e-3F  / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 50mV
    100e-3F / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 100mV
    200e-3F / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 200mV
    500e-3F / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 500mV
    1.0F    / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 1V
    2.0F    / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 2V
    5.0F    / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 5V
    10.0F   / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN),    // 10V
    20.0F   / 20 * Grid::Height() / (_FPGA::VALUE::MAX - _FPGA::VALUE::MIN)     // 20V
};

/// Столько вольт в одной точке экрана
static const float voltsInPixel[] =
{
    2e-3F   / GRID_DELTA,   // 2mV
    5e-3F   / GRID_DELTA,   // 5mV
    10e-3F  / GRID_DELTA,   // 10mV
    20e-3F  / GRID_DELTA,   // 20mV
    50e-3F  / GRID_DELTA,   // 50mV
    100e-3F / GRID_DELTA,   // 100mV
    200e-3F / GRID_DELTA,   // 200mV
    500e-3F / GRID_DELTA,   // 500mV
    1.0F    / GRID_DELTA,   // 1V
    2.0F    / GRID_DELTA,   // 2V
    5.0F    / GRID_DELTA,   // 5V
    10.0F   / GRID_DELTA,   // 10V
    20.0F   / GRID_DELTA    // 20V
};

static const int voltsInPixelInt[] =   // Коэффициент 20000
{
    2,      // 2
    5,      // 5
    10,     // 10
    20,     // 20
    50,     // 50
    100,    // 100
    200,    // 200
    500,    // 500
    1000,   // 1
    2000,   // 2
    5000,   // 5
    1000,   // 10
    20000   // 20
};



static void MultiplyToWindow(float *data, int numPoints);

static void Normalize(float *data, int numPoints);
/// Возвращает напряжение, соответствующее верхней границе сетки
static float MaxVoltageOnScreen(Range::E range);



float MathFPGA::VoltageCursor(float shiftCurU, Range::E range, uint16 rShift)
{
    return MaxVoltageOnScreen(range) - shiftCurU * voltsInPixel[range] - RShift2Abs(rShift, range);
}


int MathFPGA::RShift2Rel(float rShiftAbs, Range::E range)
{
    int retValue = RShift::ZERO + (int)(rShiftAbs / absStepRShift[range]);

    if (retValue < RShift::MIN)
    {
        retValue = RShift::MIN;
    }
    else if (retValue > RShift::MAX)
    {
        retValue = RShift::MAX;
    }
    else
    {
        // здесь ничего делать не нужно
    }

    return retValue;
}


float MathFPGA::TimeCursor(float shiftCurT, TBase::E tBase)
{
    return shiftCurT * absStepTShift[tBase];
}


void MathFPGA::PointsRel2Voltage(const uint8 *points, int numPoints, Range::E range, int16 rShift, float *voltage)
{
    int voltInPixel = voltsInPixelInt[range];
    float maxVoltsOnScreen = MaxVoltageOnScreen(range);
    float rShiftAbs = RShift2Abs(rShift, range);
    int diff = (int)((_FPGA::VALUE::MIN * voltInPixel) + (maxVoltsOnScreen + rShiftAbs) * 20e3F);
    float koeff = 1.0F / 20e3F;
    for (int i = 0; i < numPoints; i++)
    {
        voltage[i] = (points[i] * voltInPixel - diff) * koeff; //-V636
    }
}


uint8 MathFPGA::Voltage2Point(float voltage, Range::E range, uint16 rShift)
{
    int relValue = (int)((voltage + MaxVoltageOnScreen(range) + RShift2Abs(rShift, range)) / voltsInPoint[range] + _FPGA::VALUE::MIN);
    ::Math::Limitation<int>(&relValue, 0, 255);
    return (uint8)relValue;
}


float MathFPGA::Point2Voltage(uint8 value, Range::E range, uint16 rShift)
{
    if(range == Range::_1V)
    {
        range = range;
    }
    
    uint8 delta = (uint8)(value - _FPGA::VALUE::MIN);

    float rShiftAbs = RShift2Abs(rShift, range);

    float maxVoltage = MaxVoltageOnScreen(range);

    return delta * voltsInPoint[range] - maxVoltage - rShiftAbs;
}


void MathFPGA::PointsVoltage2Rel(const float *voltage, int numPoints, Range::E range, int16 rShift, uint8 *points)
{
    float maxVoltOnScreen = MaxVoltageOnScreen(range);
    float rShiftAbs = RShift2Abs(rShift, range);
    float voltInPixel = 1.0F / (voltsInPoint[range] / ((_FPGA::VALUE::MAX - _FPGA::VALUE::MIN) / 200.0F));

    float add = maxVoltOnScreen + rShiftAbs;

    float delta = add * voltInPixel + _FPGA::VALUE::MIN;

    for (int i = 0; i < numPoints; i++)
    {
        int value = (int)(voltage[i] * voltInPixel + delta);

        if (value < 0)
        {
            points[i] = 0;
            continue;
        }
        else if (value > 255)
        {
            points[i] = 255;
            continue;
        }
        else
        {
            // здесь ничего не делаем
        }

        points[i] = (uint8)value;
    }
}


float MathFPGA::TShift2Abs(int tShift, TBase::E tBase)
{
    return absStepTShift[tBase] * tShift;
}


/*
    Быстрое преобразование Фурье. Вычисляет модуль спектра для дейсвтительного сигнала.
    Количество отсчётов должно быть 2**N
*/

#ifndef DEBUG
#include "TablesWindow.h"
#include "TablesLog.h"

static float const *Koeff(int numPoints)
{
    float const *tables[3][4] = {
        {koeffsNorm256, koeffsHamming256, koeffsBlack256, koeffsHann256},
        {koeffsNorm512, koeffsHamming512, koeffsBlack512, koeffsHann512},
        {koeffsNorm1024, koeffsHamming1024, koeffsBlack1024, koeffsHann1024},
    };

    int row = 0;
    if (numPoints == 512)
    {
        row = 1;
    }
    else if (numPoints == 1024)
    {
        row = 2;
    }

    return tables[row][PageServiceMath_GetWindowFFT()];
}

#endif


void MathFPGA::CalculateFFT(float *dataR, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1)
{
    float scale = 1.0F / absStepTShift[set.time.base] / 1024.0F;

    float koeff = 1024.0F / numPoints;

    *freq0 = scale * set.fft.posCur[0] * koeff;
    *freq1 = scale * set.fft.posCur[1] * koeff;

    if (set.time.peakDet)
    {
        *freq0 *= 2;
        *freq1 *= 2;
    }

    for (int i = 0; i < numPoints; i++)
    {
        result[i] = 0.0F;
    }

    MultiplyToWindow(dataR, numPoints);

    int logN = 8;

    if (numPoints == 512)
    {
        logN = 9;
    }
    else if (numPoints == 1024)
    {
        logN = 10;
    }
    else
    {
        // другое количество точек не обрабатываем
    }

    static const float Rcoef[14] =
    {
        -1.0000000000000000F, 0.0000000000000000F, 0.7071067811865475F,
        0.9238795325112867F, 0.9807852804032304F, 0.9951847266721969F,
        0.9987954562051724F, 0.9996988186962042F, 0.9999247018391445F,
        0.9999811752826011F, 0.9999952938095761F, 0.9999988234517018F,
        0.9999997058628822F, 0.9999999264657178F
    };

    static const float Icoef[14] =
    {
        0.0000000000000000F, -1.0000000000000000F, -0.7071067811865474F,
        -0.3826834323650897F, -0.1950903220161282F, -0.0980171403295606F,
        -0.0490676743274180F, -0.0245412285229122F, -0.0122715382857199F,
        -0.0061358846491544F, -0.0030679567629659F, -0.0015339801862847F,
        -0.0007669903187427F, -0.0003834951875714F
    };

    int nn = numPoints >> 1;
    int ie = numPoints;

    for (int n = 1; n <= logN; n++)
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0F;
        float iu = 0.0F;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < numPoints; i += ie)
            {
                int io = i + in;
                float dRi = dataR[i];
                float dRio = dataR[io];
                float ri = result[i];
                float rio = result[io];
                dataR[i] = dRi + dRio;
                result[i] = ri + rio;
                float rtq = dRi - dRio;
                float itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu;
                result[io] = itq * ru + rtq * iu;
            }
            float sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
        ie >>= 1;
    }

    for (int j = 1, i = 1; i < numPoints; i++)
    {
        if (i < j)
        {
            int io = i - 1;
            int in = j - 1;
            float rtp = dataR[in];
            float itp = result[in];
            dataR[in] = dataR[io];
            result[in] = result[io];
            dataR[io] = rtp;
            result[io] = itp;
        }

        int k = nn;

        while (k < j)
        {
            j = j - k;
            k >>= 1;
        }

        j = j + k;
    }

    for (int i = 0; i < 256; i++)
    {
        result[i] = std::sqrtf(dataR[i] * dataR[i] + result[i] * result[i]);
    }

    result[0] = 0.0F;       /// \todo нулевая составляющая мешает постоянно. надо её убрать

    Normalize(result, 256);

    if (set.fft.scale == ScaleFFT::Log)
    {
        float minDB = MaxDBFFT::MaxDBforFFT(set.fft.maxDB);

        for (int i = 0; i < 256; i++)
        {
#ifdef DEBUG
            result[i] = 20 * std::log10f(result[i]);
#else
            result[i] = Log10[(int)(result[i] * 10000)];
#endif

            if (i == set.fft.posCur[0])
            {
                *density0 = result[i];
            }
            else if (i == set.fft.posCur[1])
            {
                *density1 = result[i];
            }
            else
            {
                // остальные варианты не обрабатываем
            }

            if (result[i] < minDB)
            {
                result[i] = minDB;
            }

            result[i] = 1.0F - result[i] / minDB;
        }
    }
    else
    {
        *density0 = result[set.fft.posCur[0]];
        *density1 = result[set.fft.posCur[1]];
    }
    *y0 = (int)(Grid::MathBottom() - result[set.fft.posCur[0]] * Grid::MathHeight());
    *y1 = (int)(Grid::MathBottom() - result[set.fft.posCur[1]] * Grid::MathHeight());
}


static void MultiplyToWindow(float *data, int numPoints)
{
#ifndef DEBUG
    float const *koeff = Koeff(numPoints);

    for (int i = 0; i < numPoints; i++)
    {
        data[i] *= koeff[i];
    }
#else

    if (set.fft.window == WindowFFT::Hamming)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.53836F - 0.46164F * std::cosf(2 * PI * i / (numPoints - 1));
        }
    }
    else if (set.fft.window == WindowFFT::Blackman)
    {
        float alpha = 0.16F;
        float a0 = (1.0F - alpha) / 2.0F;
        float a1 = 0.5F;
        float a2 = alpha / 2.0F;
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= a0 - a1 * std::cosf(2 * PI * i / (numPoints - 1)) + a2 * std::cosf(4 * PI * i / (numPoints - 1));
        }
    }
    else if (set.fft.window == WindowFFT::Hann)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.5F * (1.0F - std::cosf(2.0F * PI * i / (numPoints - 1.0F)));
        }
    }
    else
    {
        // а других-то окон и нету
    }

#endif
}


static void Normalize(float *data, int)
{
    float max = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (data[i] > max)
        {
            max = data[i];
        }
    }

    for (int i = 0; i < 256; i++)
    {
        data[i] /= max;
    }
}


static float MaxVoltageOnScreen(Range::E range)
{
    //DEF__STRUCT(StructRange, float) table[Range::Size] =
    static const float table[Range::Size] =
    {
        2e-3F, 5e-3F, 10e-3F, 20e-3F, 50e-3F, 100e-3F, 200e-3F, 500e-3F, 1.0F, 2.0F, 5.0F, 10.0F, 20.0F
    };

    return table[range] * 5.0F;
}


float MathFPGA::RShift2Abs(int rShift, Range::E range)
{
    return -(RShift::ZERO - (int)rShift) * absStepRShift[range];
}
