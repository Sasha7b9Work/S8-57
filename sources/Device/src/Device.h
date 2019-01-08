#pragma once
#include "Hardware/Hardware.h"
#include "Multimeter/Multimeter.h"
#include "Osci/Osci.h"
#include "Tester/Tester.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Device
{
public:

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

    static void Init();

    static void Update();

    static Mode::E CurrentMode();

    static bool InModeTester();

    static bool InModeMultimeter();

    /// Устанавливает режим работы
    static void SetMode(Mode::E mode);

private:
    static Mode::E currentMode;
};
