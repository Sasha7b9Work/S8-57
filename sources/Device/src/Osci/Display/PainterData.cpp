#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Osci/Osci.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "SCPI/SCPI.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdlib>


bool DisplayOsci::PainterData::needSendToSCPI_FFT = false;


void DisplayOsci::PainterData::DrawData()
{
    if (SCPI::Sender::fft)
    {
        SCPI::Sender::fft = false;
        needSendToSCPI_FFT = true;
    }


    if (AutoMeasurements::DataIsSetting())
    {
        static const pFuncVV func[ModeWork::Count] =
        {
            DrawCurrent,
            DrawRAM,
            DrawROM
        };
    
        func[S_MEM_MODE_WORK]();
    }

    DrawSpectrum();

    Accumulator::NextFrame();

    needSendToSCPI_FFT = false;
}


void DisplayOsci::PainterData::DrawCurrent() //-V2506
{
    if (DS == nullptr)
    {
        return;
    }

    if(S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
    {
        DrawChannel(ChanB);
        DrawChannel(ChanA);
    }
    else
    {
        DrawChannel(ChanA);
        DrawChannel(ChanB);
    }

    DisplayOsci::MemoryWindow::Draw();
}


void DisplayOsci::PainterData::DrawRAM()
{
    DrawChannel(ChanA);
    DrawChannel(ChanB);
    DisplayOsci::MemoryWindow::Draw();
}


void DisplayOsci::PainterData::DrawSpectrumChannel(const float *spectrum, Color color)
{
    color.SetAsCurrent();
    int gridLeft = Grid::Left();
    int gridBottom = Grid::MathBottom();
    int gridHeight = Grid::MathHeight();
    for (int i = 0; i < 256; i++)
    {
        int height = static_cast<int>(gridHeight * spectrum[i]);

        VLine(height).Draw(gridLeft + i, gridBottom - height);
    }
}


void DisplayOsci::PainterData::WriteParametersFFT(Chan::E ch, float freq0, float density0, float freq1, float density1)
{
    int x = Grid::Left() + 259;
    int y = Grid::ChannelBottom() + 5;
    int dY = 10;

    char buffer[20];
    Color::FILL.SetAsCurrent();

    Text(AutoMeasurements::Freq2String(freq0, false, buffer)).Draw(x, y);

    y += dY;

    Text(AutoMeasurements::Freq2String(freq1, false, buffer)).Draw(x, y);

    if(ch == ChanA)
    {
        y += dY + 2;
    }
    else
    {
        y += dY * 3 + 4;
    }

    Color::CHAN[ch].SetAsCurrent();

    Text(S_FFT_SCALE_IS_LOG ? SU::Db2String(density0, 4, buffer) : AutoMeasurements::Float2String(density0, false, buffer)).Draw(x, y);

    y += dY;

    Text(S_FFT_SCALE_IS_LOG ? SU::Db2String(density1, 4, buffer) : AutoMeasurements::Float2String(density1, false, buffer)).Draw(x, y);
}


void DisplayOsci::PainterData::DrawSpectrum(const uint8 *dataIn, int numPoints, Chan::E ch) //-V2506
{
    if (!S_CHANNEL_ENABLED(ch))
    {
        return;
    }

#undef SIZE_BUFFER

    static const int SIZE_BUFFER = (1024 * 4);

    if (numPoints > SIZE_BUFFER)
    {
        numPoints = SIZE_BUFFER;
    }

    float dataR[SIZE_BUFFER];

    float freq0 = 0.0F;
    float freq1 = 0.0F;
    float density0 = 0.0F;
    float density1 = 0.0F;
    int y0 = 0;
    int y1 = 0;

    float *spectrum = static_cast<float *>(std::malloc(numPoints * sizeof(float)));

    if (spectrum)
    {
        int s = 2;

        VALUE::PointsToVoltage(dataIn, numPoints, RANGE_DS(ch), (int16)RSHIFT_DS(ch), dataR);

        MathFPGA::CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1, ch);

        DrawSpectrumChannel(spectrum, Color::CHAN[ch]);

        WriteParametersFFT(ch, freq0, density0, freq1, density1);

        VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Left() + S_FFT_POS_CUR_0, Grid::ChannelBottom(), Color::GRID);
        VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Left() + S_FFT_POS_CUR_1, Grid::ChannelBottom());
        Rectangle(s * 2, s * 2).Draw(S_FFT_POS_CUR_0 + Grid::Left() - s, y0 - s, Color::FILL);
        Rectangle(s * 2, s * 2).Draw(S_FFT_POS_CUR_1 + Grid::Left() - s, y1 - s);

        std::free(spectrum);
    }
}


void DisplayOsci::PainterData::DrawSpectrum() //-V2506
{
    if (!S_FFT_ENABLED || !DS)
    {
        return;
    }

    VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Right(), Grid::ChannelBottom(), Color::BACK);
    
    
    if (S_MEM_MODE_WORK_IS_DIR)
    {
        int numPoints = DS->BytesInChannel();
    
        if (numPoints > 1024)       // \todo Пока 8к и более не хочет считать
        {
            numPoints = 1024;
        }
    
        if (S_FFT_SOURCE_IS_A)
        {
            DrawSpectrum(OUT_A, numPoints, ChanA);
        }
        else if (S_FFT_SOURCE_IS_B)
        {
            DrawSpectrum(OUT_B, numPoints, ChanB);
        }
        else
        {
            if (S_DISP_LAST_AFFECTED_CHANNEL_IS_A)
            {
                DrawSpectrum(OUT_B, numPoints, ChanB);
                DrawSpectrum(OUT_A, numPoints, ChanA);
            }
            else
            {
                DrawSpectrum(OUT_A, numPoints, ChanA);
                DrawSpectrum(OUT_B, numPoints, ChanB);
            }
        }
    
    }
    
    VLine(Grid::MathHeight()).Draw(Grid::Left(), Grid::ChannelBottom(), Color::FILL);
    HLine(290).Draw(Grid::Left(), Grid::MathBottom());
}


void DisplayOsci::PainterData::DrawROM()
{
    if(S_MEM_TYPE_SIGNAL_ROM_IS_CURRENT || S_MEM_TYPE_SIGNAL_ROM_IS_BOTH)
    {
        DrawCurrent();
    }

    if (S_MEM_TYPE_SIGNAL_ROM_IS_RECORDED || S_MEM_TYPE_SIGNAL_ROM_IS_BOTH)
    {
        Reader::ReadDataFromROM();
        AutoMeasurements::SetData();
        DrawCurrent();
    }
}


void DisplayOsci::PainterData::DrawChannel(Chan::E ch) //-V2506
{
    if (!S_CHANNEL_ENABLED(ch))
    {
        return;
    }

    uint8 *data = OUT(ch);

    data += DisplayOsci::ShiftInMemory::Get();

    if (PEAKDET_ENABLED(DS))
    {
        data += DisplayOsci::ShiftInMemory::Get();
    }

    int center = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top();

    int left = Grid::Left();

    float scale = static_cast<float>(Grid::Height()) / (VALUE::MAX - VALUE::MIN);

    if (S_FFT_ENABLED)
    {
        center -= (Grid::Bottom() - Grid::Top()) / 4;
        scale /= 2.0F;
    }

    Buffer bufferP2P;
    int posSeparator = 0;

    if(Roller::NeedDraw())
    {
        posSeparator = Roller::FillScreenBuffer(ch, bufferP2P, Grid::Width());
        data = bufferP2P.data;
    }

    if (S_DISP_MAPPING_IS_LINES)
    {
        DrawModeLines(ch, left, center, data, scale);
    }
    else
    {
        DrawModePoints(ch, left, center, data, scale);
    }

    if(posSeparator > 0)
    {
        VLine(Grid::Height()).Draw(Grid::Left() + posSeparator, Grid::Top(), Color::GRID);
    }
}


void DisplayOsci::PainterData::DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    int x = left;

    if (PEAKDET_ENABLED(DS))
    {
        DrawModeLinesPeakDetOn(center, data, scale, x);
    }
    else
    {
        DrawModeLinesPeakDetOff(center, data, scale, x);
    }
}


void DisplayOsci::PainterData::DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 281 * 2; i += 2)
    {
        int min = static_cast<int>(center - (data[i] - VALUE::AVE) * scale + 0.5F);
        int max = static_cast<int>(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F);

        VLine(min - max).Draw(x++, max);
    }
}


void DisplayOsci::PainterData::DrawModeLinesPeakDetOff(int center, const uint8 *data, float scale, int x)
{
    for (int i = 1; i < 281; i++)
    {
        int value = static_cast<int>(center - (data[i] - VALUE::AVE) * scale + 0.5F);
        int valuePrev = static_cast<int>(center - (data[i - 1] - VALUE::AVE) * scale + 0.5F);

        if(data[i] != VALUE::NONE && data[i - 1] != VALUE::NONE)
        {
            if(value == valuePrev)
            {
                Pixel().Draw(x, valuePrev);
            }
            else
            {
                int val = valuePrev > value ? (value + 1) : (value - 1);
                VLine(val - valuePrev).Draw(x, valuePrev);
            }
        }
        x++;
    }
}


void DisplayOsci::PainterData::DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    if (PEAKDET_ENABLED(DS))
    {
        DrawModePointsPeakDetOn(center, data, scale, left);
    }
    else
    {
        DrawModePointsPeakDetOff(center, data, scale, left);
    }
}


void DisplayOsci::PainterData::DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 281 * 2; i += 2)
    {
        Pixel().Draw(x, static_cast<int>(center - (data[i] - VALUE::AVE) * scale + 0.5F));
        Pixel().Draw(x, static_cast<int>(center - (data[i + 1] - VALUE::AVE) * scale + 0.5F));
        x++;
    }
}


void DisplayOsci::PainterData::DrawModePointsPeakDetOff(int center, const uint8 *data, float scale, int x)
{
    for (int i = 0; i < 280; i++)
    {
        uint8 v8 = data[i];
        if(v8 != VALUE::NONE)
        {
            float value = center - (v8 - VALUE::AVE) * scale;
            Pixel().Draw(x + i, ROUND(uint8, value));
        }
    }
}


BitSet64 DisplayOsci::PainterData::PointsOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = DisplayOsci::ShiftInMemory::Get();
    retValue.sword1 = retValue.sword0 + 281;

    return retValue;
}


BitSet64 DisplayOsci::PainterData::BytesOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = DisplayOsci::ShiftInMemory::Get();
    retValue.sword1 = retValue.sword0 + 281;

    if (PEAKDET_ENABLED(DS))
    {
        retValue.sword1 += 281;
    }

    return retValue;
}
