#include "defines.h"
#include "FPGA/Calibrator.h"
#include "FPGA/TypesFPGA.h"
#include "Hardware/HAL/HAL.h"
#include "Osci/Osci.h"
#include "Settings/SettingsNRST.h"
#include <cmath>
#include <cstring>


void Calibrator::Calibrate()
{
    StretchADC::SetTypeReal();
    ExtraShift::SetTypeReal();

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

    Display::Message::Show(ch == Chan::A ? "Калибрую канал 1" : "Калибрую канал 2", true);

    bool result = Balance(ch, false) && Stretch(ch);

    Display::Message::Hide();

    return result;
}


bool Calibrator::Balance(Chan::E ch, bool showHint)
{
    SettingsNRST old = setNRST;

    ExtraShift::SetTypeDisabled();

    static const pString messages[Chan::Count] =
    {
        "Балансирую канал 1",
        "Балансирую канал 2"
    };

    if (showHint)
    {
        Display::Message::Show(messages[ch], true);
    }

    ModeCouple(ch).SetGND();

    TBase::Set100ms();

    for (int range = 0; range < Range::Count; range++)
    {
        Balance(ch, static_cast<Range::E>(range));
    }

    std::memcpy(&old.exShift.value[ch][0], &setNRST.exShift.value[ch][0], sizeof(setNRST.exShift.value[ch][0]) * Range::Count);

    setNRST = old;

    Osci::Init();

    if (showHint)
    {
        Display::Message::Hide();
    }

    return true;
}


void Calibrator::Balance(Chan::E ch, Range::E range)
{
    Osci::Stop();

    Range(ch).Set(range);

    RShift(ch).Set(0);

    Osci::Start(false);

    float sum = 0;

    int numPoints = 0;

    uint8 *addr = ((ch == Chan::A) ? RD::DATA_A : RD::DATA_B) + 1;

    while (numPoints < 100)
    {
        if (!HAL_BUS::Panel::InInteraction())
        {
            if (HAL_PIO::Read(PIN_P2P))
            {
                HAL_BUS::FPGA::SetAddrData(addr);
                sum += HAL_BUS::FPGA::ReadA0();
                numPoints++;
            }
        }
    }

    float delta = std::fabsf(sum / numPoints - 127.0F);

    if (delta > 0.0F)
    {
        ExtraShift::SetValue(ch, range, static_cast<int8>(delta * 200.0F / 125.0F + 0.5F));
    }
    else
    {
        ExtraShift::SetValue(ch, range, static_cast<int8>(delta * 200.0F / 125.0F - 0.5F));
    }
}


float Calibrator::FindStretchK(Chan::E ch)
{
    Osci::Stop();

    float sumMIN = 0.0F;
    float sumMAX = 0.0F;

    int numMIN = 0;
    int numMAX = 0;

    uint8 *addr = ((ch == Chan::A) ? RD::DATA_A : RD::DATA_B) + 1;

    for (int i = 0; i < 300; i++)
    {
        if (!HAL_BUS::Panel::InInteraction())
        {
            HAL_BUS::FPGA::SetAddrData(addr);
            uint8 d = HAL_BUS::FPGA::ReadA0();

            if (d > VALUE::MAX - 32)
            {
                sumMAX += d;
                numMAX++;
            }
            else if (d < VALUE::MIN + 32)
            {
                sumMIN += d;
                numMIN++;
            }
            else
            {
                return -1.0F;
            }
        }
    }

    static const float pointsInPixel = (VALUE::MAX - VALUE::MIN) / 200.0F;  // Столько пикселей на экране занимает одна точка сигнала по вертикали

    float patternMIN = VALUE::MIN + pointsInPixel * 20;
    float patternMAX = VALUE::MAX - pointsInPixel * 20;
    float patternDELTA = patternMAX - patternMIN;

    float min = sumMIN / numMIN;
    float max = sumMAX / numMAX;
    float delta = max - min;

    return (patternDELTA / delta);
}


bool Calibrator::Stretch(Chan::E ch)
{
    SettingsNRST old = setNRST;

    ExtraShift::SetTypeReal();
    StretchADC::SetTypeDisabled();

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
        old.stretchADC.value[ch] = k;
    }

    setNRST = old;

    Osci::Init();

    return (k > 0.0F);
}


Calibrator::Mode::E &Calibrator::Mode::Ref()
{
    return set.serv.calibratorMode;
}


void Calibrator::NormalExit()
{
    StretchADC::SetTypeReal();
    ExtraShift::SetTypeReal();
}


void Calibrator::BadExit()
{
    StretchADC::SetTypeDisabled();
    ExtraShift::SetTypeDisabled();
}
