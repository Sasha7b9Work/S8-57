#include "stdafx.h"
#ifndef WIN32
#include "defines.h"
#include "BottomPart.h"
#include "Data/Reader.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/Symbols.h"
#include "FlashDrive/FlashDrive.h"
#include "FPGA/FPGA_Math.h"
#include "Hardware/CPU.h"
#include "Hardware/VCP.h"
#include "Settings/SettingsTime.h"
#include "Tables.h"
#include "Utils/StringUtils.h"
#include "Utils/Dictionary.h"
#include "Utils/Values.h"
#endif


using namespace Display::Primitives;
using namespace FPGA::Math;
using namespace FPGA::Settings;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Написать параметры вертикального тракта заданного канала
static void WriteTextVoltage(Chan::E ch, int x, int y);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void BottomPart::Draw()
{
    int y0 = Display::HEIGHT - 19;
    int y1 = Display::HEIGHT - 10;
    int x = -1;

	HLine line(Grid::Left() - Measure::GetDeltaGridLeft() - 1);

	line.Draw(1, Grid::ChannelBottom(), Color::SEPARATOR);
	line.Draw(1, Grid::FullBottom());

    WriteTextVoltage(Chan::A, x + 2, y0);
    WriteTextVoltage(Chan::B, x + 2, y1);

    VLine(Display::HEIGHT - Grid::Bottom() - 4).Draw(x + 95, Grid::Bottom() + 2, Color::SEPARATOR);

    x += 98;
    const int SIZE = 100;
    char buffer[SIZE] = {0};

    snprintf(buffer, SIZE, "р\xa5%s", TBase(SET_TBASE).ToString());

    String(buffer).Draw(x, y0, Color::FILL);

    buffer[0] = 'a';
    buffer[1] = 0;
    snprintf(buffer, SIZE, "\xa5%s", SET_TSHIFT.ToString(SET_TBASE).CString());
    String(buffer).Draw(x + 35, y0);

    buffer[0] = 0;

    if (MODE_WORK == ModeWork::Dir)
    {
        pString source[3] = { "1", "2", "\x82" };
        snprintf(buffer, 100, "с\xa5\x10%s", source[(uint8)TRIG_SOURCE]);
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
        snprintf(buffer, SIZE, "\xa5\x10%s\x10\xa5\x10%s\x10\xa5\x10", couple[TRIG_INPUT], polar[TRIG_POLARITY]);
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
        snprintf(buffer, 100, "\xa5\x10%c", mode[START_MODE]);
        String(buffer).Draw(x + 63, y1);
    }

    VLine(Display::HEIGHT - Grid::Bottom() - 4).Draw(x + 79, Grid::Bottom() + 2, Color::SEPARATOR);

	HLine line2(Display::WIDTH - Grid::Right() - 4);

    line2.Draw(Grid::Right() + 2, Grid::Bottom(), Color::FILL);
    line2.Draw(Grid::Right() + 2, Grid::ChannelBottom());

    x += 82;
    y0 = y0 - 3;
    y1 = y1 - 6;
    int y2 = y1 + 6;
    Font::SetCurrent(Font::Type::_5);

    if (MODE_WORK == ModeWork::Dir)
    {
        WriteStringAndNumber(LANG_RU ? "накопл" : "accum", (int16)x, (int16)y0, NUM_ACCUM);
        WriteStringAndNumber(LANG_RU ? "усредн" : "ave", (int16)x, (int16)y1, NUM_AVE);
        WriteStringAndNumber(LANG_RU ? "мн\x93мкс" : "mn\x93max", (int16)x, (int16)y2, NUM_MIN_MAX);
    }

    x += 42;

    VLine(Display::HEIGHT - Grid::Bottom() - 4).Draw(x, Grid::Bottom(), Color::SEPARATOR);

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

        String(mesFreq).Draw(x + 3, Grid::Bottom() + 2, Color::FILL);
    }

    DrawTime(x + 3, Grid::Bottom() + 11);

    VLine(Display::HEIGHT - Grid::Bottom() - 4).Draw(x + 55, Grid::Bottom() + 2, Color::GRAY_50);

    Font::SetCurrent(Font::Type::_UGO2);

    // Флешка
    if (FDrive::IsConnected())
    {
    }

    if (CONNECTED_TO_USB || CABLE_USB_IS_CONNECTED)
    {
        Char(SYMBOL_USB).Draw4SymbolsInRect(x + 72, Grid::Bottom() + 2, CONNECTED_TO_USB ? Color::WHITE : Color::FLASH_01);
    }

    Color::SetCurrent(Color::FILL);
    // Пиковый детектор
    if (!SET_PEAKDET_DIS)
    {
        Char('\x12').Draw(x + 38, Grid::Bottom() + 11);
        Char('\x13').Draw(x + 46, Grid::Bottom() + 11);
    }

    if (MODE_WORK == ModeWork::Dir)
    {
        Font::SetCurrent(Font::Type::_5);
        WriteStringAndNumber("СГЛАЖ.:", (int16)(x + 57), (int16)(Grid::Bottom() + 10), (int)ENUM_SMOOTHING + 1);
        Font::SetCurrent(Font::Type::_8);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int BottomPart::WriteChannel(Chan::E ch, int x, int y)
{
    String(Chan(ch).IsA() ? "1:" : "2:").Draw(x, y, Color::Channel(ch));

    x += 7;

    static const char symbols[3] = {SYMBOL_COUPLE_AC, SYMBOL_COUPLE_DC, SYMBOL_COUPLE_GND};

    char string[2] = {symbols[SET_COUPLE(ch)], 0};

    String(string).Draw(x, y);

    x += 8;

    String(Range(SET_RANGE(ch)).Name()).Draw(x, y);

    x += 22;

    Voltage(FPGA::Math::RShift2Abs(SET_RSHIFT(ch), SET_RANGE(ch))).ToString(true).Draw(x, y);

    return x + 47;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BottomPart::WriteTBase(int x, int y)
{
    String(TBase(SET_TBASE).Name()).Draw(x, y, Color::FILL);
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
        Region(widthField, heightField).Draw(x, y, color);
    }
    const int SIZE = 100;

    char buffer[SIZE];
    snprintf(buffer, SIZE, "%s\xa5%s\xa5%s", Chan(ch).IsA() ? DICT(D1ch) : DICT(D2ch), ModeCouple(SET_COUPLE(ch)).UGO(), Range(range).ToString(divider));
    String(buffer).Draw(x + 1, y, colorDraw);

    char bufferTemp[SIZE];
    snprintf(bufferTemp, SIZE, "\xa5%s", RShift::ToString((uint16)SET_RSHIFT(ch), range, divider).CString());
    String(bufferTemp).Draw(x + 46, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BottomPart::WriteStringAndNumber(const char *text, int16 x, int16 y, int number)
{
    String(text).Draw(x, y, Color::FILL);

    const int SIZE = 100;
    char buffer[SIZE];

    if (number == 0)
    {
        snprintf(buffer, SIZE, "-");
    }
    else
    {
        snprintf(buffer, SIZE, "%d", number);
    }
    Text(buffer).DrawRelativelyRight(x + 41, y);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void BottomPart::DrawTime(int x, int y)
{
    int dField = 10;
    int dSeparator = 2;

    PackedTime time = CPU::RTC_::GetPackedTime();

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
