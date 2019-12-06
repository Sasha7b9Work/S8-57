#include "defines.h"
#include "common/Transceiver.h"
#include "FPGA/Calibrator.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include <cmath>
#include <cstring>


void Calibrator::Calibrate()
{
    StretchADC::SetReal();
    ShiftADC::SetReal();

    if (!Calibrate(Chan::A))
    {
        Display::Message::ShowAndWaitKey("Калибровка канала 1 не прошла", true);
    }
    else if (!Calibrate(Chan::B))
    {
        Display::Message::ShowAndWaitKey("Калибровка канала 2 не прошла", true);
    }
    else
    {
        Display::Message::ShowAndWaitKey("Калибровка успешно завершена", true);
    }
}


bool Calibrator::Calibrate(Chan::E ch)
{
    Display::Message::ShowAndWaitKey(ch == Chan::A ?  "Подключите встроенный калибратор ко входу 1 и нажмите любую кнопку" :
                                                      "Подключите встроенный калибратор ко входу 2 и нажмите любую кнопку", true);

    return Balance(ch) && Stretch(ch);
}


bool Calibrator::Balance(Chan::E ch)
{
    Settings old = set;

    ShiftADC::SetDisabled();

    static const pString messages[Chan::Count] =
    {
        "Балансировка канала 1",
        "Балансировка канала 2"
    };

    Display::Message::Show(messages[ch], true);

    ModeCouple(ch, ModeCouple::GND);

    TBase::Load(TBase::_100ms);

    for (int range = 0; range < Range::Count; range++)
    {
        Balance(ch, static_cast<Range::E>(range));
    }

    std::memcpy(&old.dbg.nrst.shiftADC[ch][0], &set.dbg.nrst.shiftADC[ch][0], sizeof(set.dbg.nrst.shiftADC[ch][0]) * Range::Count);

    set = old;

    Osci::Init();

    Display::Message::Hide();

    return true;
}


void Calibrator::Balance(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Range(ch).Load(range);

    RShift(ch, 0);

    Osci::Start();

    float sum = 0;

    int numPoints = 0;


    uint8 *addr = ((ch == Chan::A) ? RD::DATA_A : RD::DATA_B) + 1;

    while (numPoints < 100)
    {
        if (!Transceiver::InInteraction())
        {
            if (HAL_PIO::Read(HPort::_G, HPin::_1))
            {
                HAL_FSMC::SetAddrData(addr);
                sum += HAL_FSMC::ReadData0();
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0.0F)
    {
        ShiftADC(ch).Set(range, static_cast<int8>(delta * 200.0F / 125.0F + 0.5F));
    }
    else
    {
        ShiftADC(ch).Set(range, static_cast<int8>(delta * 200.0F / 125.0F - 0.5F));
    }
}


bool Calibrator::Stretch(Chan::E ch)
{
    static const pString messages[Chan::Count] =
    {
        "Растяжка канала 1",
        "Растяжка канала 2"
    };

    Display::Message::Show(messages[ch], true);

    Settings old = set;

    ShiftADC::SetReal();

    StretchADC::SetDisabled();



    old.dbg.nrst.stretchADC[Chan::A] = set.dbg.nrst.stretchADC[Chan::A];

    set = old;

    Osci::Init();

    Display::Message::Hide();

    return true;
}
