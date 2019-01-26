#pragma once
#include "Display/Colors.h"
#include "FPGA/FPGA_Settings.h"
#include "Settings/SettingsChannel.h"
#include "Utils/String.h"
#include "Command.h"
#include "Hardware/HAL/HAL_PORTS.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TESTER_CONTROL                  (set.test_control)
#define TESTER_CONTROL_IS_U             (TESTER_CONTROL == Tester::Control::Voltage)
#define TESTER_POLARITY                 (set.test_polarity)
#define TESTER_POLARITY_IS_POSITITVE    (TESTER_POLARITY == Tester::Polarity::Positive)
#define TESTER_STEP_U                   (set.test_stepU)
#define TESTER_STEP_I                   (set.test_stepI)
#define TESTER_NUM_SMOOTH               (set.test_smooth)
#define TESTER_VIEW_MODE                (set.test_viewMode)
#define TESTER_VIEW_MODE_IS_LINES       (TESTER_VIEW_MODE == Tester::ViewMode::Lines)




/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Tester
{
public:
#define Port_TEST_ON  HAL::PORTS::Port::_F
//#define Pin_TEST_ON  HAL::PORTS::Pin::_13
#define Port_PNP      HAL::PORTS::Port::_F
//#define Pin_PNP      HAL::PORTS::Pin::_14
#define Port_U        HAL::PORTS::Port::_F
//#define Pin_U        HAL::PORTS::Pin::_15
#define Port_I        HAL::PORTS::Port::_G
//#define Pin_I        HAL::PORTS::Pin::_0
#define Port_TEST_STR HAL::PORTS::Port::_C
//#define Pin_TEST_STR    GPIO_PIN_9

    static uint16 Pin_TEST_ON;
    static uint16 Pin_PNP;
    static uint16 Pin_U;
    static uint16 Pin_I;
    static uint16 Pin_TEST_STR;

public:
    static const int NUM_STEPS = 5;
    /// Инициализация аппаратной части
    static void Init();

    static void DeInit();

    static void Enable();

    static void Disable();

    static void Update();

    static void ProcessStep();
    /// Загружает полярность из Settings
    static void LoadPolarity();
    /// Устанавливает шаг изменения напряжения в соотвествии с настройками Settings
    static void LoadStep();

    static int DeltaRShiftA();

    struct Display
    {
    public:
        static void Update();

        static void SetPoints(int numStep, const uint8 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS]);
    };

private:
    /// Загрузить FPGA в соответствии с установленными настройками
    static void LoadFPGA();
    /// Считать данные очередной ступеньки
    static void ReadData();
    /// Текущий шаг
    static int step;
    /// Шаг изменения напряжения
    static float stepU;
    /// Установленное в true значение означает, что вклюён режим тестера
    static bool enabled;

    static uint8 data[Chan::Size][NUM_STEPS][TESTER_NUM_POINTS];

public:
    /// Чем будем управлять в тестер-компоненте - напряжением или током
    struct Control
    {
        enum E
        {
            Voltage,
            Current
        } value;
        explicit Control(E v) : value(v) {};
    };

    struct Polarity
    {
        enum E
        {
            Positive,
            Negative
        } value;
        explicit Polarity(E v) : value(v) {};
    };

    struct StepU
    {
        enum E
        {
            _100mV,
            _500mV
        } value;
        explicit StepU(E v) : value(v) {};
    };

    struct StepI
    {
        enum E
        {
            _4mA,
            _20mA
        } value;
        explicit StepI(E v) : value(v) {};
    };

    struct Smoothing
    {
        enum E
        {
            _1,
            _2,
            _3,
            _4,
            _5
        } value;
        explicit Smoothing(E v) : value(v) {};
    };

    struct ViewMode
    {
        enum E
        {
            Lines,
            Points
        } value;
        explicit ViewMode(E v) : value(v) {};
    };

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
        explicit Scale(Osci::Settings::Range::E range, Chan::E _ch) : value((E)range), ch(_ch) {};
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
