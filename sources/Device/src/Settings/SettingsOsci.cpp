#include "defines.h"
#include "SettingsOsci.h"
#include "Settings.h"
#include "FPGA/FPGA_HAL.h"
#include "Osci/Osci.h"
#include "Utils/Math.h"
#include "Hardware/Timer.h"
#include "Osci/Display/Osci_Display.h"
#include "Display/Grid.h"
#include "Display/Display_Primitives.h"
#include "FPGA/FPGA_Math.h"
#include "Utils/Values.h"


using namespace Display::Primitives;
using namespace FPGA::HAL::GPIO;


///< Нужно ли рисовать горизонтальную линию уровня смещения уровня синхронизации.
static bool needDraw = false;
namespace Trig
{
    /// Установленное в true значение означает, что нужно выводить значок синхроимпульса
    bool pulse = false;
}


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Trig::Level::Load()
{
    /// \todo Здесь много лишних движений. Нужно что-то сделать с вводом SET_TRIGLEV_SOURCE
    uint16 value = (uint16)((Trig::Level::MAX + Trig::Level::MIN) - set.trig.lev[set.trig.source]);

    WriteRegisters(Pin::SPI3_CS1, (uint16)(0xa000 | (value << 2)));

    Osci::Restart();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Level::Change(int delta)
{
    Math::AdditionThisLimitation<uint16>(&set.trig.lev[set.trig.source], STEP_TRIGLEV * delta, Trig::Level::MIN, Trig::Level::MAX);

    Load();

    NeedForDraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DisableDrawing()
{
    needDraw = false;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::NeedForDraw()
{
    if (!set.fft.enabled && (set.trig.modeFind == TrigModeFind::Hand))
    {
        needDraw = true;
        Timer::SetAndStartOnce(Timer::Type::ShowLevelTrigLev, DisableDrawing, 2000);
        Osci::Display::SetFlagRedraw();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::Level::Set(int level)
{
    set.trig.lev[set.trig.source] = (uint16)(level);

    Math::Limitation<uint16>(&set.trig.lev[set.trig.source], Trig::Level::MIN, Trig::Level::MAX);

    Load();

    NeedForDraw();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Trig::SyncPulse()
{
    return pulse;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Trig::DrawOnGrid()
{
    if (needDraw)
    {
        int width = 85;
        int height = 18;

        int x = (Grid::Right() - Grid::Left()) / 2 + Grid::Left() - width / 2;
        int y = Grid::ChannelBottom() - height - 20;

        Region(width, height).DrawBounded(x, y, Color::BACK, Color::FILL);

        float trigLevVal = FPGA::Math::RShift2Abs(set.trig.lev[set.trig.source], set.ch[set.trig.source].range) * Divider((uint8)set.ch[set.trig.source].divider).ToAbs();

        Voltage voltage(trigLevVal);

        String("Синхр %s", voltage.ToString(true).CString()).Draw(x + 7, y + 5, Color::FILL);
    }
}
