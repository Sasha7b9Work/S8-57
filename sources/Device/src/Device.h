#pragma once
#include "Multimeter/Multimeter.h"
#include "Osci/Osci.h"
#include "Tester/Tester.h"


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
            Count
        } value;
        explicit Mode(E v) : value(v) {};
    };

    void Init();

    void Update();

    void Reset();

    // ������������� ����� ������
    void SetMode(Mode::E mode);

    // ���������� ������� ����� ������
    Mode::E CurrentMode();

    bool InModeTester();

    bool InModeMultimeter();

    bool InModeRecorder();

    bool InModeOsci();
};
