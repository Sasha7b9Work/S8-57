#pragma once
#include "common/Command.h"
#include "Hardware/HAL/HAL_PIO.h"
#include "Menu/Pages/Include/PageDisplay.h"


class Tester
{
public:
    static const int NUM_STEPS = 5;
    
    // Инициализация аппаратной части
    static void Init();

    void DeInit();

    static void Enable();

    static void Disable();

    static void StartStop();

    static void Update();

    static void ProcessStep();
    
    // Загружает полярность из Settings
    static void LoadPolarity();
    
    // Устанавливает шаг изменения напряжения в соотвествии с настройками Settings
    static void LoadStep();

    static int16 DeltaRShiftA();

    // Масштаб
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
            Count
        } value;
        Chan::E ch;
        explicit Scale(Range::E range, Chan::E _ch) : value(static_cast<E>(range)), ch(_ch) {};
        pString ToString() const;
    };

    
    // Смещение
    struct Shift
    {
        Shift(int16 rShift, Chan::E _ch) : shift(rShift), ch(_ch) {};
        String ToString(Scale::E scale);
    private:
        int16 shift;
        Chan::E ch;
    };

    
    // Чем будем управлять в тестер-компоненте - напряжением или током
    struct Control
    {
#define S_TEST_CONTROL (set.test._control)
#define S_TEST_CONTROL_IS_VOLTAGE (S_TEST_CONTROL == Tester::Control::Voltage)

        enum E
        {
            Voltage,
            Current
        };
    };

    
    struct Polarity
    {
#define S_TEST_POLARITY             (set.test._polarity)
#define S_TEST_POLARITY_IS_POSITIVE (S_TEST_POLARITY == Tester::Polarity::Positive)

        enum E
        {
            Positive,
            Negative
        };
    };

    
    struct StepI
    {
#define S_TEST_STEP_I           (set.test._stepI)
#define S_TEST_STEP_I_IS_4uA    (S_TEST_STEP_I == Tester::StepI::_4uA)

        enum E
        {
            _4uA,
            _20uA
        };
    };

    
    struct StepU
    {
#define S_TEST_STEP_U           (set.test._stepU)
#define S_TEST_STEP_U_IS_600mV  (S_TEST_STEP_U == Tester::StepU::_600mV)

        enum E
        {
            _600mV,
            _3V
        };
    };


    struct ViewMode
    {
#define S_TEST_VIEW_MODE        (set.test._viewMode)

        enum E
        {
            Lines,
            Points
        };
    };
};


struct DisplayTester
{
    static void Update();
    
    // Устанавливает точки для рисования, соответствующие шагу numStep
    static void SetPoints(int numStep, const uint16 dx[TESTER_NUM_POINTS], const uint8 dy[TESTER_NUM_POINTS]);

private:
    
    // Если true, то шаг готов для вывода
    static bool ready[Tester::NUM_STEPS];
    
    // Написать легенду изображения
    static void DrawLegend(int x, int y);
    
    // Отображает параметры одного канала
    static void DrawParametersChannel(Chan::E ch, int x, int y);
    
    // Возвращает цвет, которым нужно рисовать соответствующую "ступеньку"
    static Color ColorForStep(int step);
    
    // Рисовать данные ступеньки numStep
    static void DrawData(int step);
    
    // Возвращает числовое значение величины соответствующей "ступеньки"
    static String ValueForStep(int step);
};
