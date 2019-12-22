#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"
#include "common/Display/Font/Font_d.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Battery.h"
#include "Hardware/VCP.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/AutoMeasurements.h"
#include "Settings/Settings.h"


void DisplayOsci::BottomPart::Draw(int x0, int y0)
{
    DFont::Set(DTypeFont::_UGO2);

    // Флешка
    if (FDrive::IsConnected())
    {
    }

    if (VCP::connectedToUSB || VCP::cableUSBisConnected)
    {
        Char(SymbolUGO2::USB).Draw4SymbolsInRect(x0 + 72, y0 + 2, VCP::connectedToUSB ? Color::WHITE : Color::FLASH_01);
    }

    Color::FILL.SetAsCurrent();
    // Пиковый детектор
    if (PeakDetMode().IsEnabled())
    {
        Char('\x12').Draw(x0 + 38, y0 + 11);
        Char('\x13').Draw(x0 + 46, y0 + 11);
    }

    Battery::Draw(x0, y0);

    VLine(18).Draw(x0, y0 + 1, Color::FILL);
}
