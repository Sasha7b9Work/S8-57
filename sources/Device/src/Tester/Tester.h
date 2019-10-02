#pragma once
#include "Command.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Settings/SettingsOsci.h"


struct DisplayTester
{
    static void Update();
    /// Устанавливает точки для рисования, соответствующие шагу numStep
    static void SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS]);
};


struct Tester
{
#define Port_TEST_ON  HPort::_F
#define Port_PNP      HPort::_F
#define Port_U        HPort::_F
#define Port_I        HPort::_G
#define Port_TEST_STR HPort::_C

    static uint16 Pin_TEST_ON;
    static uint16 Pin_PNP;
    static uint16 Pin_U;
    static uint16 Pin_I;
    static uint16 Pin_TEST_STR;

    static const int NUM_STEPS = 5;
    /// Инициализация аппаратной части
    static void Init();

    void DeInit();

    static void Enable();

    static void Disable();

    static void StartStop();

    static void Update();

    static void ProcessStep();
    /// Загружает полярность из Settings
    static void LoadPolarity();
    /// Устанавливает шаг изменения напряжения в соотвествии с настройками Settings
    static void LoadStep();

    static int DeltaRShiftA();

    /// Масштаб
    struct Scale
    {
        enum E
        {
            _2uA,
            _5uA,
            _10uA,
            _20uA,
            _50uA,
            _100uA,
            _200uA,
            _500uA,
            _1mA,
            _2mA,
            _5mA,
            _10mA,
            _20mA,
            Number
        } value;
        Chan::E ch;
        explicit Scale(Range::E range, Chan::E _ch) : value(static_cast<E>(range)), ch(_ch) {};
        pString ToString() const;
    };

    /// Смещение
    struct Shift
    {
        Shift(uint16 rShift, Chan::E _ch) : shift(rShift), ch(_ch) {};
        String ToString(Scale::E scale);
    private:
        uint16 shift;
        Chan::E ch;
    };
};
