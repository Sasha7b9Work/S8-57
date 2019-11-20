#pragma once
#include "Multimeter/Multimeter.h"
#include "Osci/Osci.h"
#include "Tester/Tester.h"



struct Device
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

    static void Init();

    static void Update();

    static void Reset();
    /// Обработать поступившую команду SCPI
    static void ProcessDataSCPI(const char *data);


    struct State
    {
        /// Устанавливает режим работы
        static void SetMode(Mode::E mode);
        /// Возвращает текущий режим работы
        static Mode::E CurrentMode();

        static bool InModeTester();

        static bool InModeMultimeter();

        static bool InModeRecorder();

        static bool InModeOsci();
    };
};
