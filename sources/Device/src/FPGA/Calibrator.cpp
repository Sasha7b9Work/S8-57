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

        return BadExit();
    }
    else if (!Calibrate(Chan::B))
    {
        Display::Message::ShowAndWaitKey("Калибровка канала 2 не прошла", true);

        return BadExit();
    }

    Display::Message::ShowAndWaitKey("Калибровка успешно завершена", true);

    NormalExit();
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

    ModeCouple(ch).SetGND();

    TBase::Set100ms();

    for (int range = 0; range < Range::Count; range++)
    {
        Balance(ch, static_cast<Range::E>(range));
    }

    std::memcpy(&old.dbg.nrst.shiftADC.shift[ch][0], &ShiftADC::Ref().shift[ch][0], sizeof(ShiftADC::Ref().shift[ch][0]) * Range::Count);

    set = old;

    Osci::Init();

    Display::Message::Hide();

    return true;
}


void Calibrator::Balance(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Range(ch).Set(range);

    RShift(ch).Set(0);

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
        ShiftADC::Set(ch, range, static_cast<int8>(delta * 200.0F / 125.0F + 0.5F));
    }
    else
    {
        ShiftADC::Set(ch, range, static_cast<int8>(delta * 200.0F / 125.0F - 0.5F));
    }
}


float Calibrator::FindStretchK(Chan::E ch)
{
    Osci::Stop();

    static const int NUM_POINTS = 300;

    uint8 buffer[NUM_POINTS];

    int numPoints = 0;

    uint8 *addr = ((ch == Chan::A) ? RD::DATA_A : RD::DATA_B) + 1;

    while (numPoints < NUM_POINTS)
    {
        if (!Transceiver::InInteraction())
        {
            HAL_FSMC::SetAddrData(addr);
            buffer[numPoints++] = HAL_FSMC::ReadData0();
        }
    }

    return 1.0F;
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

    ModeCouple(ch).SetAC();
    RShift(ch).Set(0);
    Range(ch).Set500mV();
    TBase::Set200us();
    TShift::Set(0);
    TrigSource::Set(ch);
    TrigLevel(ch).Set(0);

    float k = FindStretchK(ch);

    if (k > 0.0F)
    {
        old.dbg.nrst.stretchADC.stretch[ch] = k;
    }

    set = old;

    Osci::Init();

    Display::Message::Hide();

    return true;
}


Calibrator::Mode::E &Calibrator::Mode::Ref()
{
    return set.serv.calibratorMode;
}


void Calibrator::NormalExit()
{
    StretchADC::SetReal();
    ShiftADC::SetReal();
}


void Calibrator::BadExit()
{
    StretchADC::SetDisabled();
    ShiftADC::SetDisabled();
}
