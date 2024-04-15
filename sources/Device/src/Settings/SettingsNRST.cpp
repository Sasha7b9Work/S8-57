#include "defines.h"
#include "Hardware/Memory/ROM.h"
#include "Settings/SettingsNRST.h"
#include <cstring>


SettingsNRST setNRST;


static const SettingsNRST defaultNRST =
{
    0,                                   // size
    0,                                   // numSmoothForRand
    0,                                   // correctionTime
    0,                                   // enum_gate_max
    0,                                   // enum_gate_min
    {{0}, {0}},                          // shiftADC
    {1.0F, 1.0F}                         // stretchADCtype
};


void SettingsNRST::Init()
{
    SettingsNRST *saved = ROM::NRST::GetSaved();

    if(!saved ||                                    // ≈сли нет сохраЄнных настроек
       saved->size == static_cast<uint>(-1))        // или в пам€ти по этому адресу ещЄ не было ничего записано
    {
        setNRST = defaultNRST;                      // “о примен€ем значени€ по умолчанию
    }
    else if(saved->size != sizeof(*this))           // ≈сли размер сохранЄнных настроек не равен размеру нашей структуры (а он не может быть больше)
    {
        setNRST = defaultNRST;                      // то примен€ем значени€ по умолчанию
        std::memcpy(&setNRST, saved, saved->size);  // на загружаем из пам€ти те значени€, что сть
        size = sizeof(*this);
    }
    else
    {
        setNRST = *saved;                           // ¬ этой точке размеры считанной структуры и нашей совпадают
    }
}


void SettingsNRST::Save()
{
    SettingsNRST *saved = ROM::NRST::GetSaved();

    size = sizeof(*this);

    if(!saved ||                        // ≈сли нет сохранЄнных настроек
       saved->size != size ||           // или размеры структур не совпадают
       *saved != setNRST)               // или настройки записанные и текущие не совпадают
    {
        ROM::NRST::Save(&setNRST);      // то сохран€ем настройки
    }
}


void SettingsNRST::Reset()
{
    setNRST = defaultNRST;
}


bool SettingsNRST::operator!=(const SettingsNRST &rhs)
{
    return std::memcmp(this, &rhs, sizeof(*this)) != 0; //-V1014
}


void SettingsNRST::SetExtraShift(int8 shift)
{
    for(int ch = 0; ch < Chan::Count; ch++)
    {
        for(int range = 0; range < Range::Count; range++)
        {
            NRST_EX_SHIFT(ch, range) = shift;
        }
    }
}


void SettingsNRST::ResetExtraStretch()
{
    NRST_EX_STRETCH(ChanA) = 1.0F;
    NRST_EX_STRETCH(ChanB) = 1.0F;
}
