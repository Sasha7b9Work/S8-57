#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"
#include "Display/Font/Font.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Battery.h"
#include "Hardware/VCP.h"
#include "Osci/Display/DisplayOsci.h"
#include "Osci/Measurements/Measurements.h"
#include "Settings/Settings.h"


void DisplayOsci::BottomPart::Draw(int x0, int y0)
{
    Font::SetCurrent(TypeFont::_UGO2);

    // Флешка
    if (FDrive::IsConnected())
    {
    }

    if (CONNECTED_TO_USB || CABLE_USB_IS_CONNECTED)
    {
        Char(SymbolUGO2::USB).Draw4SymbolsInRect(x0 + 72, y0 + 2, CONNECTED_TO_USB ? Color::WHITE : Color::FLASH_01);
    }

    Color::FILL.SetAsCurrent();
    // Пиковый детектор
    if (set.time.peakDet != PeakDetMode::Disabled)
    {
        Char('\x12').Draw(x0 + 38, y0 + 11);
        Char('\x13').Draw(x0 + 46, y0 + 11);
    }

    Battery::Draw(x0, y0);

    VLine(18).Draw(x0, y0 + 1);
}
