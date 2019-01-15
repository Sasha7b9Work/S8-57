#include "defines.h"
#include "Settings.h"
#include "Hardware/EEPROM.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"


using namespace FPGA::SET;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
Settings set = Settings::defaultSettings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void Settings::Load(bool _default)
{
    if(_default || !EEPROM::LoadSettings())
    {
        Reset();
        FPGA::SET::Load();
        uint8 data[1024];
        TransformFromLoad(data);
        Menu::Init();
        FPGA::OnPressStart();
    }

    /*
    uint size = 0;
    AT25160N::ReadData(0, (uint8 *)&size, 4);

    if(size > 0 && size < 1024)
    {
        uint8 *data = (uint8 *)malloc(size);

        AT25160N::ReadData(0, data, size);

        uint crc32 = Hardware::CalculateCRC32((uint)data + 8, size - 8);

        if(crc32 == *((uint *)(data + 4)))
        {
            memcpy(&set, data, size);
        }
        else
        {
            Reset();
        }

        free(data);
    }
    else
    {
        Reset();
    }
    */
}
//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Save()
{
    uint8 data[1024];
    set.TransformForSave(data);
    EEPROM::SaveSettings();

    /*
    SET_SIZE = sizeof(Settings);
    SET_CRC32 = Hardware::CalculateCRC32((uint)((uint8 *)&set + 8), SET_SIZE - 8);

    AT25160N::WriteData(0, (uint8 *)&set, SET_SIZE);
    */
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Settings::Reset()
{
    set = defaultSettings;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString Range::Name() const
{
    static const struct StructRange
    {
        const char * names[Language::Number];
        StructRange(pString nRU, pString nEN)
        {
            names[Language::RU] = nRU;
            names[Language::EN] = nEN;
        }
    }
    names[Range::Number] =
    {
        StructRange("2ìÂ",  "2mV"),
        StructRange("5ìÂ",  "5mV"),
        StructRange("10ìÂ", "10mV"),
        StructRange("20ìÂ", "20mV"),
        StructRange("50ìÂ", "50mV"),
        StructRange("0.1Â", "0.1V"),
        StructRange("0.2Â", "0.2V"),
        StructRange("0.5Â", "0.5V"),
        StructRange("1Â",   "1V"),
        StructRange("2Â",   "2V"),
        StructRange("5Â",   "5V"),
        StructRange("10Â",  "10V"),
        StructRange("20Â",  "20V")
    };

    return names[value].names[LANG];
};

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
pString TBase::Name() const
{
    static pString names[TBase::Number][Language::Number] =
    {
        {"2íñ",     "2ns"},
        {"5íñ",     "5ns"},
        {"10íñ",    "10ns"},
        {"20íñ",    "20ns"},
        {"50íñ",    "50ns"},
        {"0.1ìêñ",  "0.1us"},
        {"0.2ìêñ",  "0.2us"},
        {"0.5ìêñ",  "0.5us"},
        {"1ìêñ",    "1us"},
        {"2ìêñ",    "2us"},
        {"5ìêñ",    "5us"},
        {"10ìêñ",   "10us"},
        {"20ìêñ",   "20us"},
        {"50ìêñ",   "50us"},
        {"0.1ìñ",   "0.1ms"},
        {"0.2ìñ",   "0.2ms"},
        {"0.5ìñ",   "0.5ms"},
        {"1ìñ",     "1ms"},
        {"2ìñ",     "2ms"},
        {"5ìñ",     "5ms"},
        {"10ìñ",    "10ms"},
        {"20ìñ",    "20ms"},
        {"50ìñ",    "50ms"},
        {"0.1ñ",    "0.1s"},
        {"0.2ñ",    "0.2s"},
        {"0.5ñ",    "0.5s"},
        {"1ñ",      "1s"},
        {"2ñ",      "2s"},
        {"5ñ",      "5s"},
        {"10ñ",     "10s"}
    };

    return names[value][LANG];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
float MaxDBforFFT(FFTmaxDB::E maxDB)
{
    static const float arrayMAX_DB_FOR_FFT[] = {-40.0F, -60.0F, -80.0F};

    return arrayMAX_DB_FOR_FFT[maxDB];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Settings &Settings::GetDefault()
{
    return (Settings &)defaultSettings;
}
