#include "defines.h"
#include "HiPart.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Math.h"
#include "Osci/Measurements/Measurements_Table.h"
#include "Tables.h"
#include "Utils/Dictionary.h"
#include "Utils/Values.h"
#include <cmath>
#include <cstdio>
#include <cstring>

#include "Utils/Debug.h"
#include "Display/Font/Font.h"
#include "Hardware/Timer.h"


using namespace Display::Primitives;
using namespace FPGA;
using namespace FPGA::Math;
using namespace FPGA::Settings;
using namespace Hardware;
using namespace Osci::Measurements;
using namespace Osci::Measurements::Cursors;
using namespace Osci::Settings;

using Osci::Measurements::Measure;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Написать параметры вертикального тракта заданного канала
static void WriteTextVoltage(Chan::E ch, int x, int y);

static void WriteStringAndNumber(const char *text, int x, int y, int number);

static void DrawTime(int x, int y);
/// Нарисовать разделительные линии
static void DrawSeparators(int x, int y);
/// Записывает главные параметры в указанную позицию. Возвращает х-координату правого верхнего угла выведенного изображения
static int DrawMainParameters(int x, int y);
/// Нарисовать правую часть - синхронизация и режим работы
static void DrawRightPart(int x, int y);

static void WriteCursors();
/// Нарисовать значок пикового детектора
static void DrawPeakDet(int x, int y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Osci::Display::HiPart::Draw()
{
    const int y0 = 0;
    const int x0 = 0;

    int y1 = 9;
    int x = -1;

    DrawSeparators(x0, y0);

    x = DrawMainParameters(x, y0 + 1); //-V2007

    x += 82;
    y1 = y1 - 6;
    int y2 = y1 + 6;
    Font::SetCurrent(Font::Type::_5);

    if (MODE_WORK == ModeWork::Dir)
    {
        WriteStringAndNumber(LANG_RU ? "накопл" : "accum", x, y0 - 3, NUM_ACCUM);
        WriteStringAndNumber(LANG_RU ? "усредн" : "ave", x, y1, NUM_AVE);
        WriteStringAndNumber(LANG_RU ? "мн\x93мкс" : "mn\x93max", x, y2, NUM_MIN_MAX);
    }

    x += 42;

    Font::SetCurrent(Font::Type::_8);

    if (MODE_WORK == ModeWork::Dir)
    {
        char mesFreq[20] = "\x7c=";
        float freq = FrequencyCounter::GetFreq();
        if (freq == -1.0F) //-V550
        {
            std::strcat(mesFreq, "******");
        }
        else
        {
            std::strcat(mesFreq, Frequency(freq).ToString().CString());
        }

        String(mesFreq).Draw(x + 3, y0 + 1, Color::FILL); //-V2007
    }

    DrawTime(x + 3, y0 + 10); //-V2007

    DrawRightPart(271, y0);

    WriteCursors();

    DrawPeakDet(x + 37, y0 + 10);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawSeparators(int x, int y)
{
    HLine line(Grid::Left() - Osci::Measurements::Table::GetDeltaGridLeft() - 1);

    line.Draw(1, Grid::ChannelBottom(), Color::SEPARATOR);
    line.Draw(1, Grid::FullBottom());

    VLine separator(17);

    separator.Draw(x + 95, y, Color::FILL);
    separator.Draw(x + 172, y);
    separator.Draw(x + 215, y);
    separator.Draw(x + 270, y);

    HLine line2(::Display::WIDTH - Grid::Right() - 4);

    line2.Draw(Grid::Right() + 2, Grid::Bottom(), Color::FILL);
    line2.Draw(Grid::Right() + 2, Grid::ChannelBottom());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int DrawMainParameters(int _x, int _y)
{
    /*
        Если активны курсоры ручных измерений, то нужно корректировать положение вывода
    */

    if (Osci::Measurements::Cursors::Cursor::NecessaryDraw())
    {
        _y = Grid::Bottom() + 3;
    }

    int x = _x;
    int y0 = _y;
    int y1 = _y + 8;

    WriteTextVoltage(Chan::A, x + 2, y0);
    WriteTextVoltage(Chan::B, x + 2, y1);

    x += 98;
    const int SIZE = 100;
    char buffer[SIZE] = { 0 };

    std::snprintf(buffer, SIZE, "р\xa5%s", TBase(SET_TBASE).ToString());

    String(buffer).Draw(x, y0, Color::FILL);

    buffer[0] = 'a';
    buffer[1] = 0;
    std::snprintf(buffer, SIZE, "\xa5%s", SET_TSHIFT.ToString(SET_TBASE).CString());
    String(buffer).Draw(x + 35, y0);

    buffer[0] = 0;

    if (MODE_WORK == ModeWork::Dir)
    {
        pString source[3] = { "1", "2", "\x82" };
        std::snprintf(buffer, 100, "с\xa5\x10%s", source[(uint8)TRIG_SOURCE]);
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
    if (MODE_WORK == ModeWork::Dir)
    {
        std::snprintf(buffer, SIZE, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY]);
        String(buffer).Draw(x + 18, y1);

        Char(filtr[TRIG_INPUT][0]).Draw(x + 45, y1);
        Char(filtr[TRIG_INPUT][1]).Draw(x + 53, y1);
    }

    buffer[0] = '\0';
    const char mode[] =
    {
        '\xb7',
        '\xa0',
        '\xb0'
    };
    if (MODE_WORK == ModeWork::Dir)
    {
        std::snprintf(buffer, 100, "\xa5\x10%c", mode[START_MODE]);
        String(buffer).Draw(x + 63, y1);
    }

    return _x + 93;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteTextVoltage(Chan::E ch, int x, int y)
{
    if (!SET_ENABLED(ch))
    {
        return;
    }

    Color color = Color::Channel(ch);

    bool inverse = SET_INVERSE(ch);
    Divider::E divider = SET_DIVIDER(ch);
    Osci::Settings::Range::E range = SET_RANGE(ch);

    const int widthField = 91;
    const int heightField = 8;
    Color colorDraw = inverse ? Color::WHITE : color;
    if (inverse)
    {
        Region(widthField, heightField).Fill(x, y, color);
    }
    const int SIZE = 100;

    char buffer[SIZE];
    std::snprintf(buffer, SIZE, "%s\xa5%s\xa5%s", Chan(ch).IsA() ? DICT(D1ch) : DICT(D2ch), ModeCouple(SET_COUPLE(ch)).UGO(), Range(range).ToString(divider));
    String(buffer).Draw(x + 1, y, colorDraw);

    char bufferTemp[SIZE];
    std::snprintf(bufferTemp, SIZE, "\xa5%s", RShift::ToString((uint16)SET_RSHIFT(ch), range, divider).CString());
    String(bufferTemp).Draw(x + 46, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
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

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = Clock::GetTime();

    Color::SetCurrent(Color::FILL);

    if ((MODE_WORK == ModeWork::ROM) || (MODE_WORK == ModeWork::RAM))
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
            Integer((int)time.day).ToString(false, 2).Draw(x, y);
            String(':').Draw(x + dField, y);
            Integer((int)time.month).ToString(false, 2).Draw(x + dField + dSeparator, y);
            String(':').Draw(x + 2 * dField + dSeparator, y);
            Integer((int)time.year + 2000).ToString(false, 4).Draw(x + 2 * dField + 2 * dSeparator, y);

            y += 9;
        }
        else
        {
            return;
        }
    }

    Integer((int)time.hours).ToString(false, 2).Draw(x, y);
    String(':').Draw(x + dField, y);
    Integer((int)time.minutes).ToString(false, 2).Draw(x + dField + dSeparator, y);
    String(':').Draw(x + 2 * dField + dSeparator, y);
    Integer((int)time.seconds).ToString(false, 2).Draw(x + 2 * dField + 2 * dSeparator, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawRightPart(int x0, int y0)
{
    // Синхроимпульс

    static const int xses[3] = { 280, 271, 251 };
    int x = xses[MODE_WORK];

    if (MODE_WORK != ModeWork::RAM)
    {
        x += 2;

        if (Trig::SyncPulse())
        {
            Region(26, 16).Fill(x0 + 1, y0 + 1, Color::FILL);
            String(DICT(DTrig)).Draw(x0 + 9, y0 + 5, Color::BACK);
        }
    }

    // Режим работы
    static pString strs[][2] =
    {
        {"ИЗМ", "MEAS"},
        {"ПОСЛ", "LAST"},
        {"ВНТР", "INT"}
    };

    if (MODE_WORK != ModeWork::Dir)
    {
        x += 18;

        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 2;
        String(DICT(DMode)).Draw(LANG_RU ? x : x + 3, -1);
        Text(strs[MODE_WORK][LANG]).DrawInCenterRect(x + 1, 9, 25, 8);
    }
    else
    {
        x -= 9;
    }

    if (MODE_WORK != ModeWork::RAM)
    {
        x += 27;

        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 2;
        int y = y0 + 1;

        if (FPGA::IsRunning())       // Рабочий режим
        {
            Char(SYMBOL_PLAY).Draw4SymbolsInRect(x, 1);
        }
        else if (FPGA_IN_STATE_STOP)  // Режим остановки
        {
            Region(10, 10).Fill(x + 3, y + 3);
        }
        else if (FPGA_IN_STATE_WAIT)  // Режим ожидания сигнала
        {
            int w = 4;
            int h = 14;
            int delta = 4;
            x = x + 2;
            Region(w, h).Fill(x, y + 1);
            Region(w, h).Fill(x + w + delta, y + 1);
        }
        else
        {
            // больше ничего не обрабатываем
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void WriteCursors()
{
    const int x0 = 0;
    const int y0 = 0;

    int x = x0;
    int y1 = y0;
    int y2 = y0 + 9; //-V2007

    if (Cursor::NecessaryDraw())
    {
        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 3;
        Chan::E source = CURS_SOURCE;
        Color colorText = Color::Channel(source);
        if (CURsU_ENABLED)
        {
            String("1:").Draw(x, y1, colorText);
            String("2:").Draw(x, y2);
            x += 7;
            Cursor::Voltage(source, 0).Draw(x, y1);
            Cursor::Voltage(source, 1).Draw(x, y2);
            x = x0 + 49;
            float pos0 = FPGA::Math::VoltageCursor(Cursor::PosU(source, 0), SET_RANGE(source), SET_RSHIFT(source));
            float pos1 = FPGA::Math::VoltageCursor(Cursor::PosU(source, 1), SET_RANGE(source), SET_RSHIFT(source));
            float delta = std::fabsf(pos1 - pos0);
            if (SET_DIVIDER_10(source))
            {
                delta *= 10;
            }
            String(":dU=").Draw(x, y1);
            Voltage(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            Cursor::PercentsU(source).Draw(x + 10, y2);
        }

        x = x0 + 101;

        VLine(Grid::Top() - 3).Draw(x, 1, Color::FILL);

        x += 3;
        if (CURsT_ENABLED)
        {
            Color::SetCurrent(colorText);
            String("1:").Draw(x, y1);
            String("2:").Draw(x, y2);
            x += 7;
            Cursor::Time(source, 0).Draw(x, y1);
            Cursor::Time(source, 1).Draw(x, y2);
            x = x0 + 153;

            float pos0 = FPGA::Math::TimeCursor(CURsT_POS(source, 0), SET_TBASE);
            float pos1 = FPGA::Math::TimeCursor(CURsT_POS(source, 1), SET_TBASE);
            float delta = std::fabsf(pos1 - pos0);
            String(":dT=").Draw(x, y1);
            Time(delta).ToString(false).Draw(x + 17, y1);
            String(':').Draw(x, y2);
            Cursor::PercentsT(source).Draw(x + 8, y2);

            if (CURSORS_SHOW_FREQ)
            {
                int width = 65;
                x = Grid::Right() - width;
                Rectangle(width, 12).Draw(x, Grid::Top(), Color::FILL);
                Region(width - 2, 10).Fill(x + 1, Grid::Top() + 1, Color::BACK);
                String("1/dT=").Draw(x + 1, Grid::Top() + 2, colorText);
                if (delta != 0.0F) //-V550
                {
                    Frequency(1.0F / delta).ToString().Draw(x + 25, Grid::Top() + 2);
                }
            }
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawPeakDet(int x, int y)
{
    if (SET_PEAKDET_EN)
    {
        Font::SetCurrent(Font::Type::_UGO2);
        Char(SYMBOL_PEAK_DET_LEFT).Draw(x, y, Color::FILL);
        Char(SYMBOL_PEAK_DET_RIGHT).Draw(x + 8, y);
        Font::SetCurrent(Font::Type::_8);
    }
}
