#include "defines.h"
#include "Data/Reader.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "FPGA/MathFPGA.h"
#include "Osci/Osci.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Buffer.h"
#include "Utils/Math.h"
#include "Utils/StringUtils.h"
#include <cstdlib>


void DisplayOsci::PainterData::DrawData()
{
    if (AutoMeasurements::DataIsSetting())
    {
        static const pFuncVV func[ModeWork::Count] =
        {
            DrawCurrent,
            DrawRAM,
            DrawROM
        };
    
        func[ModeWork()]();
    }
    
    DrawSpectrum();

    Accumulator::NextFrame();
}


void DisplayOsci::PainterData::DrawCurrent()
{
    if (DS == nullptr)
    {
        return;
    }

    if(set.disp.lastAffectedChannel == Chan::A)
    {
        DrawChannel(Chan::B);
        DrawChannel(Chan::A);
    }
    else
    {
        DrawChannel(Chan::A);
        DrawChannel(Chan::B);
    }
    DisplayOsci::MemoryWindow::Draw();
}


void DisplayOsci::PainterData::DrawRAM()
{
    DrawChannel(Chan::A);
    DrawChannel(Chan::B);
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

    if (Chan(ch).IsA())
    {
        y += dY + 2;
    }
    else
    {
        y += dY * 3 + 4;
    }

    Color::CHAN[ch].SetAsCurrent();

    Text((set.fft.scale == ScaleFFT::Log) ? SU::Db2String(density0, 4, buffer) : AutoMeasurements::Float2String(density0, false, buffer)).Draw(x, y);

    y += dY;

    Text((set.fft.scale == ScaleFFT::Log) ? SU::Db2String(density1, 4, buffer) : AutoMeasurements::Float2String(density1, false, buffer)).Draw(x, y);
}


void DisplayOsci::PainterData::DrawSpectrum(const uint8 *dataIn, uint numPoints, Chan::E ch)
{
    if (!set.ch[ch].enabled)
    {
        return;
    }

#undef SIZE_BUFFER
#define SIZE_BUFFER (1024 * 4)

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
    int s = 2;

    float *spectrum = static_cast<float *>(std::malloc(numPoints * sizeof(float)));

    if (spectrum)
    {
        VALUE::PointsToVoltage(dataIn, numPoints, RANGE_DS(ch), (int16)RSHIFT_DS(ch), dataR);

        MathFPGA::CalculateFFT(dataR, numPoints, spectrum, &freq0, &density0, &freq1, &density1, &y0, &y1);

        DrawSpectrumChannel(spectrum, Color::CHAN[ch]);

        WriteParametersFFT(ch, freq0, density0, freq1, density1);

        VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Left() + set.fft.posCur[0], Grid::ChannelBottom(), Color::GRID);
        VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Left() + set.fft.posCur[1], Grid::ChannelBottom());
        Rectangle(s * 2, s * 2).Draw(set.fft.posCur[0] + Grid::Left() - s, y0 - s, Color::FILL);
        Rectangle(s * 2, s * 2).Draw(set.fft.posCur[1] + Grid::Left() - s, y1 - s);

        std::free(spectrum);
    }
}


void DisplayOsci::PainterData::DrawSpectrum()
{
    if (!set.fft.enabled || !DS)
    {
        return;
    }

    VLine(Grid::MathBottom() - Grid::ChannelBottom()).Draw(Grid::Right(), Grid::ChannelBottom(), Color::BACK);
    
    
    if (ModeWork::IsDir())
    {
        uint numPoints = DS->SizeChannel();
    
        if (numPoints > 2048)       /// \todo ѕока 8к и более не хочет считать
        {
            numPoints = 2048;
        }
    
        if (set.fft.source == SourceFFT::A)
        {
            DrawSpectrum(OUT_A, numPoints, Chan::A);
        }
        else if (set.fft.source == SourceFFT::B)
        {
            DrawSpectrum(OUT_B, numPoints, Chan::B);
        }
        else
        {
            if (set.disp.lastAffectedChannel == Chan::A)
            {
                DrawSpectrum(OUT_B, numPoints, Chan::B);
                DrawSpectrum(OUT_A, numPoints, Chan::A);
            }
            else
            {
                DrawSpectrum(OUT_A, numPoints, Chan::A);
                DrawSpectrum(OUT_B, numPoints, Chan::B);
            }
        }
    
    }
    
    VLine(Grid::MathHeight()).Draw(Grid::Left(), Grid::ChannelBottom(), Color::FILL);
    HLine(290).Draw(Grid::Left(), Grid::MathBottom());
}


void DisplayOsci::PainterData::DrawROM()
{
    if(set.mem.typeSignalROM == TypeSignalROM::Current || set.mem.typeSignalROM == TypeSignalROM::Both)
    {
        DrawCurrent();
    }

    if (set.mem.typeSignalROM == TypeSignalROM::Recorded || set.mem.typeSignalROM == TypeSignalROM::Both)
    {
        Reader::ReadDataFromROM();
        AutoMeasurements::SetData();
        DrawCurrent();
    }
}


void DisplayOsci::PainterData::DrawChannel(Chan::E ch)
{
    if (!set.ch[ch].enabled)
    {
        return;
    }

    uint8 *data = OUT(ch);


    data += SHIFT_IN_MEMORY;

    if (PeakDetMode().IsEnabled())
    {
        data += SHIFT_IN_MEMORY;
    }

    /// ƒл€ отрисовки поточечного вывода
    Buffer buffer;

    if (Osci::InModeP2P())
    {
        if (FRAME_P2P)
        {
            FRAME_P2P->FillBufferForDraw(ch, &buffer);
            data = buffer.data;
        }
        else if(DS == nullptr)
        {
            return;
        }
        else
        {
            // Ѕудем выводит целый фрейм, несмотр€ на то, что поточечный вывод
        }
    }

    int center = (Grid::Bottom() - Grid::Top()) / 2 + Grid::Top();

    int left = Grid::Left();

    float scale = static_cast<float>(Grid::Height()) / (VALUE::MAX - VALUE::MIN);

    if (set.fft.enabled)
    {
        center -= (Grid::Bottom() - Grid::Top()) / 4;
        scale /= 2.0F;
    }


    if (DisplayMapping() == DisplayMapping::Lines)
    {
        DrawModeLines(ch, left, center, data, scale);
    }
    else
    {
        DrawModePoints(ch, left, center, data, scale);
    }
}


void DisplayOsci::PainterData::DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    int x = left;

    if (PeakDetMode().IsEnabled())
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
    int coordVert = -1;  // Ќа этой координате нужно нарисовать вертикальную линию, чтобы скрыть дефект поточечного вывода, когда считана только часть точек

    for (int i = 1; i < 281; i++)
    {
        int value = static_cast<int>(center - (data[i] - VALUE::AVE) * scale + 0.5F);
        int valuePrev = static_cast<int>(center - (data[i - 1] - VALUE::AVE) * scale + 0.5F);

        if (value == valuePrev)
        {
            Pixel().Draw(x++, valuePrev);
        }
        else
        {
            int val = valuePrev > value ? (value + 1) : (value - 1);
            VLine(val - valuePrev).Draw(x++, valuePrev);
        }

        if (coordVert == -1 && data[i] == VALUE::NONE)
        {
            coordVert = i;
            VLine(Grid::Height()).Draw(x - 1, Grid::Top(), Color::GRID);
        }
    }
}


void DisplayOsci::PainterData::DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale)
{
    Color::CHAN[ch].SetAsCurrent();

    if (PeakDetMode().IsEnabled())
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
        float value = center - (data[i] - VALUE::AVE) * scale;
        Pixel().Draw(x + i, ROUND(uint8, value));
    }
}


int DisplayOsci::PainterData::FirstPointOnScreen()
{
    return set.disp.shiftInMemory;
}


BitSet64 DisplayOsci::PainterData::PointsOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = SHIFT_IN_MEMORY;
    retValue.sword1 = retValue.sword0 + 281;

    return retValue;
}


BitSet64 DisplayOsci::PainterData::BytesOnDisplay()
{
    BitSet64 retValue;

    retValue.sword0 = SHIFT_IN_MEMORY;
    retValue.sword1 = retValue.sword0 + 281;

    if (PeakDetMode().IsEnabled())
    {
        retValue.sword1 += 281;
    }

    return retValue;
}


void DisplayOsci::PainterData::ChangeTPos()
{
    int width = Grid::Width();

    if (TPos() == TPos::Left)
    {
        set.disp.shiftInMemory = 0;
    }
    else if (TPos() == TPos::Center)
    {
        set.disp.shiftInMemory = static_cast<int16>(ENumPointsFPGA().PointsInChannel() / 2 - width / 2);
    }
    else // TPOS_IS_RIGHT
    {
        set.disp.shiftInMemory = static_cast<int16>(ENumPointsFPGA().PointsInChannel() - width - 2);
    }
}
