#include "defines.h"
#include "Display/Display.h"
#include "Display/Grid.h"
#include "FPGA/FPGA.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Math.h"
#include <cmath>
#include <cstdio>
#include <cstring>


// Столько вольт содержится в одной точке сигнала по вертикали

// Столько вольт в одной точке экрана
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


static void MultiplyToWindow(float *data, int numPoints);

static void Normalize(float *data, int numPoints);


float MathFPGA::VoltageCursor(float shiftCurU, Range::E range, int16 rShift)
{
    return Range::MaxVoltageOnScreen(range) - shiftCurU * voltsInPixel[range] - RShift::ToAbs(rShift, range);
}


float MathFPGA::TimeCursor(float shiftCurT, TBase::E tBase)
{
    return TShift::ToAbs(static_cast<int>(shiftCurT), tBase);
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


void MathFPGA::CalculateFFT(float *dataR, int numPoints, float *result, float *freq0, float *density0, float *freq1, float *density1, int *y0, int *y1, Chan::E ch)
{
    float scale = 1.0F / TShift::ToAbs(1, S_TIME_BASE) / 1024.0F;

    float koeff = 1024.0F / numPoints;

    *freq0 = scale * S_FFT_POS_CUR_0 * koeff;
    *freq1 = scale * S_FFT_POS_CUR_1 * koeff;

    if (PEAKDET_ENABLED(DS))
    {
        *freq0 *= 2;
        *freq1 *= 2;
    }

    for (int i = 0; i < numPoints; i++)
    {
        result[i] = 0.0F; //-V2563
    }

    MultiplyToWindow(dataR, numPoints);

    int logN = 8;

    if (numPoints == 512)
    {
        logN = 9;
    }
    else if (numPoints == 1024) //-V2516
    {
        logN = 10;
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

    int nn = static_cast<int>(numPoints >> 1);
    int ie = static_cast<int>(numPoints);

    for (int n = 1; n <= logN; n++)
    {
        float rw = Rcoef[logN - n];
        float iw = Icoef[logN - n];
        int in = ie >> 1;
        float ru = 1.0F;
        float iu = 0.0F;
        for (int j = 0; j < in; j++)
        {
            for (int i = j; i < static_cast<int>(numPoints); i += ie)
            {
                int io = i + in;
                float dRi = dataR[i]; //-V2563
                float dRio = dataR[io]; //-V2563
                float ri = result[i]; //-V2563
                float rio = result[io]; //-V2563
                dataR[i] = dRi + dRio; //-V2563
                result[i] = ri + rio; //-V2563
                float rtq = dRi - dRio;
                float itq = ri - rio;
                dataR[io] = rtq * ru - itq * iu; //-V2563
                result[io] = itq * ru + rtq * iu; //-V2563
            }
            float sr = ru;
            ru = ru * rw - iu * iw;
            iu = iu * rw + sr * iw;
        }
        ie >>= 1;
    }

    for (int j = 1, i = 1; i < static_cast<int>(numPoints); i++)
    {
        if (i < j)
        {
            int io = i - 1;
            int in = j - 1;
            float rtp = dataR[in]; //-V2563
            float itp = result[in]; //-V2563
            dataR[in] = dataR[io]; //-V2563
            result[in] = result[io]; //-V2563
            dataR[io] = rtp; //-V2563
            result[io] = itp; //-V2563
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
        result[i] = std::sqrtf(dataR[i] * dataR[i] + result[i] * result[i]); //-V2563
    }

    result[0] = 0.0F;       // \todo нулевая составляющая мешает постоянно. надо её убрать //-V2563

    Normalize(result, 256);

    if (DisplayOsci::PainterData::needSendToSCPI_FFT)
    {
        SCPI::SendData(ch == ChanA ? "1 : " : "2 : ");

        char buffer[50];
        for(int i = 0; i < 255; i++)
        {
            std::sprintf(buffer, "%e ", result[i]); //-V2563
            SCPI::SendData(buffer);
        }
        std::sprintf(buffer, "%e", result[255]); //-V2563
        SCPI::SendAnswer(buffer);
    }

    if (S_FFT_SCALE_IS_LOG)
    {
        float minDB = MaxDBFFT::MaxDBforFFT(S_FFT_MAX_DB);

        for (int i = 0; i < 256; i++)
        {
#ifdef DEBUG
            result[i] = 20 * std::log10f(result[i]); //-V2563
#else
            result[i] = Log10[static_cast<int>(result[i] * 10000)];
#endif

            if (i == S_FFT_POS_CUR_0)
            {
                *density0 = result[i]; //-V2563
            }
            else if (i == S_FFT_POS_CUR_1) //-V2516
            {
                *density1 = result[i]; //-V2563
            }

            if (result[i] < minDB) //-V2563
            {
                result[i] = minDB; //-V2563
            }

            result[i] = 1.0F - result[i] / minDB; //-V2563
        }
    }
    else
    {
        *density0 = result[S_FFT_POS_CUR_0]; //-V2563
        *density1 = result[S_FFT_POS_CUR_1]; //-V2563
    }
    *y0 = static_cast<int>(Grid::MathBottom() - result[S_FFT_POS_CUR_0] * Grid::MathHeight()); //-V2563
    *y1 = static_cast<int>(Grid::MathBottom() - result[S_FFT_POS_CUR_1] * Grid::MathHeight()); //-V2563
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

    if (S_FFT_WINDOW_IS_HAMMING)
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.53836F - 0.46164F * std::cosf(2 * Math::PI_F * i / (numPoints - 1)); //-V2563
        }
    }
    else if (S_FFT_WINDOW_IS_BLACKMAN)
    {
        float alpha = 0.16F;
        float a0 = (1.0F - alpha) / 2.0F;
        float a1 = 0.5F;
        float a2 = alpha / 2.0F;
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= a0 - a1 * std::cosf(2 * Math::PI_F * i / (numPoints - 1)) + a2 * std::cosf(4 * Math::PI_F * i / (numPoints - 1)); //-V2563
        }
    }
    else if (S_FFT_WINDOW_IS_HANN) //-V2516
    {
        for (int i = 0; i < numPoints; i++)
        {
            data[i] *= 0.5F * (1.0F - std::cosf(2.0F * Math::PI_F * i / (numPoints - 1.0F))); //-V2563
        }
    }

#endif
}


static void Normalize(float *data, int)
{
    float max = 0.0;
    for (int i = 0; i < 256; i++)
    {
        if (data[i] > max) //-V2563
        {
            max = data[i]; //-V2563
        }
    }

    for (int i = 0; i < 256; i++)
    {
        data[i] /= max; //-V2563
    }
}
