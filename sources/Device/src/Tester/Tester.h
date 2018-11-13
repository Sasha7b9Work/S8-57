#pragma once
#include "Settings/SettingsChannel.h"


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define TESTER_NUM_POINTS               (240)
#define TESTER_CONTROL                  (set.test_control)
#define TESTER_CONTROL_IS_U             (TESTER_CONTROL == Tester::Control::Voltage)
#define TESTER_POLARITY                 (set.test_polarity)
#define TESTER_POLARITY_IS_POSITITVE    (TESTER_POLARITY == Tester::Polarity::Positive)
#define TESTER_STEP_U                   (set.test_stepU)
#define TESTER_STEP_I                   (set.test_stepI)
#define TESTER_NUM_SMOOTH               (set.test_smooth)
#define TESTER_VIEW_MODE                (set.test_viewMode)
#define TESTER_VIEW_MODE_IS_LINES       (TESTER_VIEW_MODE == Tester::ViewMode::Lines)




///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Tester
{
public:
    /// Инициализация аппаратной части
    static void Init();

    static void Enable();

    static void Disable();

    static void Update();

    static void ProcessStep();
    /// Загружает полярность из Settings
    static void LoadPolarity();
    /// Устанавливает шаг изменения напряжения в соотвествии с настройками Settings
    static void LoadStep();

    static const int NUM_STEPS = 5;

    class Graphics
    {
    public:
        static void Update();

        static void SetPoints(int numStep, uint8 dx[TESTER_NUM_POINTS], uint8 dy[TESTER_NUM_POINTS]);

    private:
        static void DrawData(int numStep, int x0, int y0);
    };

private:
    /// Загрузить FPGA в соответствии с установленными настройками
    static void LoadFPGA();
    /// Считать данные очередной ступеньки
    static void ReadData();
    /// Запустить процесс чтения очередной ступеньки
    static void StartFPGA();
    /// Текущий шаг
    static int step;
    /// Шаг изменения напряжения
    static float stepU;
    /// Установленное в true значение означает, что вклюён режим тестера
    static bool enabled;

    static uint8 data[Chan::Number][NUM_STEPS][TESTER_NUM_POINTS];

public:
    /// Чем будем управлять в тестер-компоненте - напряжением или током
    struct Control
    {
        enum E
        {
            Voltage,
            Current
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct Polarity
    {
        enum E
        {
            Positive,
            Negative
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct StepU
    {
        enum E
        {
            _100mV,
            _500mV
        } value;
        operator uint8() const { return (uint8)value; };
    };

    struct StepI
    {
        enum E
        {
            _4mA,
            _20mA
        } value;
        operator uint8() const { return (uint8)value; };
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
        operator uint8() const { return (uint8)value; };
    };

    struct ViewMode
    {
        enum
        {
            Lines,
            Points
        } value;
        operator uint8() const { return (uint8)value; };
    };
};
