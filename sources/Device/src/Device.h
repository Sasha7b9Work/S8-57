#pragma once
#include "Hardware/Hardware.h"
#include "Multimeter/Multimeter.h"
#include "Osci/Osci.h"
#include "Tester/Tester.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Device
{
    struct Mode
    {
        enum E
        {
            Osci,
            Tester,
            Multimeter,
            Recorder,
            Number
        } value;

        explicit Mode(E v) : value(v) {};
    };

    void Init();

    void Update();
    /// Устанавливает режим работы
    void SetMode(Mode::E mode);

    /////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
    namespace State
    {
        Mode::E CurrentMode();

        bool InModeTester();

        bool InModeMultimeter();

        bool InModeRecorder();

        bool InModeOsci();

        bool RecorderEnabled();
    }
};
