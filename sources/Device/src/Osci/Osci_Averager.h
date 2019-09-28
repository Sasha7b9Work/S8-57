#pragma once
#include "Osci/Osci_Settings.h"


namespace Osci
{
    namespace Averager
    {
        void Process(Chan::E ch, const uint8 *dataNew, int size);
        void SettingChanged();
        void Draw();
    }
}
