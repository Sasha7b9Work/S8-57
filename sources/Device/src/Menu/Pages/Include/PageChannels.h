#pragma once
#include "Menu/MenuItems.h"
#include "FPGA/FPGA_Settings.h"


struct SettingsChannel
{
    uint16                         shift[Chan::Size];      ///< Сдвиг канала по вертикали
    Osci::Settings::Range::E       range[Chan::Size];      ///< Масштаб канала по вертикали
    FPGA::Settings::ModeCouple::E  couple[Chan::Size];     ///< Связь по входу
    bool                           enabled[Chan::Size];    ///< Включен/выключен канал
    int8                           balanceShiftADC[2];     ///< Добавочное смещение для балансировки АЦП.
    FPGA::Settings::Bandwidth      bandwidth[2];           ///< Ограничение полосы.
    bool                           inverse[2];
    Divider::E                     divider[2];             ///< Множитель.
    CalibrationMode::E             calibrationMode[2];     ///< Режим калибровки.
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageChannelA
{
    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageChannelB
{
    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};
