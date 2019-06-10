#pragma once


namespace Osci
{
    namespace Averager
    {
        void Process(Chan::E ch, const uint8 *dataNew, int size);
        void SettingChanged();
        void Draw();
    }
}
