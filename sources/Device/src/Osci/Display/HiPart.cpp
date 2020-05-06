#include "defines.h"
#include "Display/Grid.h"
#include "Display/Primitives.h"
#include "FPGA/FPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Reader.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Osci/Measurements/CursorsMeasurements.h"
#include "Settings/Settings.h"
#include "Utils/Values.h"
#include <cmath>
#include <cstring>
#include <cstdio>


struct Separator
{
    static void Draw(int x, int y)
    {
        VLine(17).Draw(x, y, Color::FILL);
    }
};


// Написать параметры вертикального тракта заданного канала
static void WriteTextVoltage(Chan::E ch, int x, int y);

static void WriteStringAndNumber(const char *text, int x, int y, int number);

static void DrawTime(int x, int y);
// Нарисовать разделительные линии
static void DrawSeparators();
// Записывает главные параметры в указанную позицию. Возвращает х-координату правого верхнего угла выведенного изображения
static int DrawMainParameters(int x, int y);

static void WriteCursors();
// Нарисовать значок пикового детектора
static void DrawPeakDet(int x, int y);


void DisplayOsci::HiPart::Draw()
{
    const int y0 = 0;

    int x = -1;

    DrawSeparators();

    x = DrawMainParameters(x, y0 + 1); //-V2007

    x += 82;

    x += 42;

    DFont::Set(DTypeFont::_8);

    Separator::Draw(x + 1, y0);

    if (set.mem.modeWork == ModeWork::Dir)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FreqMeter::GetFreq();
        if (freq == -1.0F) //-V550 //-V2550
        {
            std::strcat(mesFreq, "******");
        }
        else
        {
            std::strcat(mesFreq, Frequency(freq).ToString().c_str());
        }

        String(mesFreq).Draw(x + 3, y0 + 1, Color::FILL); //-V2007
    }

    DrawTime(x + 3, y0 + 10); //-V2007

    DrawRightPart(271, y0);

    WriteCursors();

    DrawPeakDet(x + 37, y0 + 10); //-V2007
}


static void DrawSeparators()
{
    HLine line(Grid::Left() - TableMeasures::GetDeltaGridLeft() - 1);

    line.Draw(1, Grid::ChannelBottom(), Color::SEPARATOR);
    line.Draw(1, Grid::FullBottom());

    HLine line2(::Display::WIDTH - Grid::Right() - 4);

    line2.Draw(Grid::Right() + 2, Grid::Bottom(), Color::FILL);
    line2.Draw(Grid::Right() + 2, Grid::ChannelBottom());
}


static int DrawMainParameters(int _x, int _y)
{
    /*
        Если активны курсоры ручных измерений, то нужно корректировать положение вывода
    */

    if (CursorsMeasurements::NecessaryDraw())
    {
        _y = Grid::Bottom() + 3;
    }

    int x = _x;
    const int y0 = _y;
    int y1 = _y + 8;

    WriteTextVoltage(ChanA, x + 2, y0);
    WriteTextVoltage(ChanB, x + 2, y1);

    x += 98;

    Separator::Draw(x - 3, _y - 1);

    const int SIZE = 100;
    char buffer[SIZE] = { 0 };

    std::snprintf(buffer, SIZE, "р\xa5%s", TBase::ToString(set.time.base));

    String(buffer).Draw(x, y0, Color::FILL);

    buffer[0] = 'a';
    buffer[1] = 0;
    std::snprintf(buffer, SIZE, "\xa5%s", TShift::ToString().c_str());
    String(buffer).Draw(x + 35, y0);

    buffer[0] = 0;

    if (set.mem.modeWork == ModeWork::Dir)
    {
        pString source[3] = { "1", "2", "\x82" };
        std::snprintf(buffer, 100, "с\xa5\x10%s", source[S_TRIG_SOURCE]);
    }

    String(buffer).Draw(x, y1, Color::Trig());

    buffer[0] = 0;
    static pString couple[] =
    {
        "\x92",
        "\x91",
        "\x92",
        "\x92"
    };
    static pString polar[] =
    {
        "\xa7",
        "\xa6"
    };
    static pString filtr[] =
    {
        "\xb5\xb6",
        "\xb5\xb6",
        "\xb3\xb4",
        "\xb1\xb2"
    };
    if (set.mem.modeWork == ModeWork::Dir)
    {
        std::snprintf(buffer, SIZE, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[set.trig.input], polar[set.trig.polarity]);
        String(buffer).Draw(x + 18, y1);

        Char(filtr[set.trig.input][0]).Draw(x + 45, y1);
        Char(filtr[set.trig.input][1]).Draw(x + 53, y1);
    }

    buffer[0] = '\0';
    const char mode[] =
    {
        '\xb7',
        '\xa0',
        '\xb0'
    };
    if (set.mem.modeWork == ModeWork::Dir)
    {
        std::snprintf(buffer, 100, "\xa5\x10%c", mode[set.trig.startMode]);
        String(buffer).Draw(x + 63, y1);
    }

    y1 = y1 - 6;

    DFont::Set(DTypeFont::_5);

    x += 77;

    Separator::Draw(x - 2, y0 - 1);

    if (set.mem.modeWork == ModeWork::Dir)
    {
        WriteStringAndNumber("накопл", x, y0 - 4, ENumAccum().Number());
        WriteStringAndNumber("усредн", x, y1, S_DISP_NUM_AVERAGE);
        WriteStringAndNumber("сглаж", x, y1 + 6, ENumSmoothing().Number());
    }

    Separator::Draw(x + 43, y0 - 1);

    return _x + 93;
}


static void WriteTextVoltage(Chan::E ch, int x, int y)
{
    if (!set.ch[ch].enabled)
    {
        return;
    }

    Color color = Color::CHAN[ch];

    bool inverse = set.ch[ch].inverse;

    Range::E range = S_RANGE(ch);

    Color colorDraw = inverse ? Color::WHITE : color;
    if (inverse)
    {
        Region(91, 8).Fill(x, y, color);
    }
    const int SIZE = 100;

    char buffer[SIZE];
    std::snprintf(buffer, SIZE, "%s\xa5%s\xa5%s", (ch == ChanA) ? "1к" : "2к", ModeCouple::UGO(set.ch[ch].couple), Range::ToString(ch, S_DIVIDER(ch)));
    String(buffer).Draw(x + 1, y, colorDraw);

    char bufferTemp[SIZE];
    std::snprintf(bufferTemp, SIZE, "\xa5%s", RShift::ToString(S_RSHIFT(ch), range, S_DIVIDER(ch)).c_str());
    String(bufferTemp).Draw(x + 46, y);
}


static void WriteStringAndNumber(const char *text, int x, int y, int number)
{
    String(text).Draw(x, y, Color::FILL);

    const int SIZE = 100;
    char buffer[SIZE];

    if (number == 0)
    {
        std::snprintf(buffer, SIZE, "-");
    }
    else
    {
        std::snprintf(buffer, SIZE, "%d", number);
    }
    Text(buffer).DrawRelativelyRight(x + 41, y);
}


static void DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = HAL_RTC::GetPackedTime();

    Color::FILL.SetAsCurrent();

    if ((set.mem.modeWork == ModeWork::ROM) || (set.mem.modeWork == ModeWork::RAM))
    {
        if (DS)
        {
            y -= 9;
            time.day = TIME_DAY_DS;
            time.hours = TIME_HOURS_DS;
            time.minutes = TIME_MINUTES_DS;
            time.seconds = TIME_SECONDS_DS;
            time.month = TIME_MONTH_DS;
            time.year = TIME_YEAR_DS;
            Integer(static_cast<int>(time.day)).ToString(false, 2).Draw(x, y);
            String(':').Draw(x + dField, y);
            Integer(static_cast<int>(time.month)).ToString(false, 2).Draw(x + dField + dSeparator, y);
            String(':').Draw(x + 2 * dField + dSeparator, y);
            Integer(static_cast<int>(time.year) + 2000).ToString(false, 4).Draw(x + 2 * dField + 2 * dSeparator, y);

            y += 9;
        }
        else
        {
            return;
        }
    }

    Integer(static_cast<int>(time.hours)).ToString(false, 2).Draw(x, y);
    String(':').Draw(x + dField, y);
    Integer(static_cast<int>(time.minutes)).ToString(false, 2).Draw(x + dField + dSeparator, y);
    String(':').Draw(x + 2 * dField + dSeparator, y);
    Integer(static_cast<int>(time.seconds)).ToString(false, 2).Draw(x + 2 * dField + 2 * dSeparator, y);
}


void DisplayOsci::HiPart::DrawRightPart(int x0, int y0)
{
    // Синхроимпульс

    Separator::Draw(x0 - 1, y0);

    static const int xses[3] = { 280, 271, 251 };
    int x = xses[set.mem.modeWork];

    if (set.mem.modeWork != ModeWork::RAM)
    {
        x += 2;

        if (Trig::SyncPulse())
        {
            Region(26, 16).Fill(x0 + 1, y0 + 1, Color::FILL);
            String("СИ").Draw(x0 + 9, y0 + 5, Color::BACK);
        }
    }

    // Режим работы
    static const pString strs[] =
    {
        "ИЗМ",
        "ПОСЛ",
        "ВНТР"
    };

    if (set.mem.modeWork != ModeWork::Dir)
    {
        x += 18;

//        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 2;
        String("режим").Draw(x, -1);
        Text(strs[set.mem.modeWork]).DrawInCenterRect(x + 1, 9, 25, 8);
    }
    else
    {
        x -= 9;
    }

    if (set.mem.modeWork != ModeWork::RAM)
    {
        x += 27;

        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 2;
        //int y = y0 + 1;

        if (FPGA::IsRunning())       // Рабочий режим
        {
            Char(Symbol8::PLAY).Draw4SymbolsInRect(x, 1);
        }
        //else if (FPGA::InStateStop())  // Режим остановки
        //{
        //    Region(10, 10).Fill(x + 3, y + 3);
        //}
        //else if (FPGA::InStateWait())  // Режим ожидания сигнала
        //{
        //    int w = 4;
        //    int h = 14;
        //    int delta = 4;
        //    x = x + 2;
        //    Region(w, h).Fill(x, y + 1);
        //    Region(w, h).Fill(x + w + delta, y + 1);
        //}
        else
        {
            // больше ничего не обрабатываем
        }
    }
}


static void WriteCursors()
{
    if (CursorsMeasurements::NecessaryDraw())
    {
        int x = 0;
        int y1 = 0;
        int y2 = 0 + 9;

        VLine separator(Grid::Top() - 3);

        separator.Draw(x, 1, Color::FILL);
        separator.Draw(102, 1);
        separator.Draw(203, 1);

        x += 3;
        Chan::E source = CursorsSource();
        Color colorText = Color::CHAN[source];
        if (!CursorsControl::IsDisabledU())
        {
            String("1:").Draw(x, y1, colorText);
            String("2:").Draw(x, y2);
            x += 7;
            CursorsMeasurements::Voltage(source, 0).Draw(x, y1);
            CursorsMeasurements::Voltage(source, 1).Draw(x, y2);
            x = 49;
            float pos0 = MathFPGA::VoltageCursor(CursorsMeasurements::PosU(source, 0), S_RANGE(source), S_RSHIFT(source));
            float pos1 = MathFPGA::VoltageCursor(CursorsMeasurements::PosU(source, 1), S_RANGE(source), S_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0) * Divider::ToAbs(S_DIVIDER(source));
            String(":dU=").Draw(x, y1);
            Voltage(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            CursorsMeasurements::PercentsU(source).Draw(x + 10, y2);
        }

        x = 101;
        x += 3;
        if (!CursorsControl::IsDisabledT())
        {
            colorText.SetAsCurrent();
            String("1:").Draw(x, y1);
            String("2:").Draw(x, y2);
            x += 7;
            CursorsMeasurements::Time(source, 0).Draw(x, y1);
            CursorsMeasurements::Time(source, 1).Draw(x, y2);
            x = 153;

            float pos0 = MathFPGA::TimeCursor(CursorsMeasurements::PosT(source, 0), set.time.base);
            float pos1 = MathFPGA::TimeCursor(CursorsMeasurements::PosT(source, 1), set.time.base);
            float delta = std::fabsf(pos1 - pos0);
            String(":dT=").Draw(x, y1);
            Time(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            CursorsMeasurements::PercentsT(source).Draw(x + 8, y2);

            if (set.curs.showFreq)
            {
                int width = 65;
                x = Grid::Right() - width;
                Rectangle(width, 12).Draw(x, Grid::Top(), Color::FILL);
                Region(width - 2, 10).Fill(x + 1, Grid::Top() + 1, Color::BACK);
                String("1/dT=").Draw(x + 1, Grid::Top() + 2, colorText);
                if (delta != 0.0F) //-V550 //-V2550
                {
                    Frequency(1.0F / delta).ToString().Draw(x + 25, Grid::Top() + 2);
                }
            }
        }
    }
}


static void DrawPeakDet(int x, int y)
{
    if (PeakDetMode().IsEnabled())
    {
        Char(SymbolUGO2::PEAK_DET_LEFT).Draw(x, y, Color::FILL);
        Char(SymbolUGO2::PEAK_DET_RIGHT).Draw(x + 8, y);
    }
}
