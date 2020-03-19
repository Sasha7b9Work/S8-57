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
        enum E
        {
            Voltage,
            Current
        };

        static Control::E &Ref();
        operator Control::E()   { return Ref(); }
        static bool IsVoltage() { return Ref() == Voltage; }
    };

    struct Polarity
    {
        enum E
        {
            Positive,
            Negative
        };
        static Polarity::E &Ref();
        operator Polarity::E()   { return Ref(); }
        static bool IsPositive() { return Ref() == Positive; }
    };

    struct StepI
    {
        enum E
        {
            _4mA,
            _20mA
        };

        StepI() {};
        static StepI::E &Ref();
        operator StepI::E()  { return Ref(); }
        static bool Is20mA() { return Ref() == _20mA; }
    };

    struct StepU
    {
        enum E
        {
            _100mV,
            _500mV
        };
        StepU() {}
        static StepU::E &Ref();
        operator StepU::E()   { return Ref(); }
        static bool Is500mV() { return Ref() == _500mV; }
    };


    struct ViewMode
    {
        enum E
        {
            Lines,
            Points
        };

        static ViewMode::E &Ref();
        operator ViewMode::E() { return Ref(); }
    };

    struct ENUMAverage
    {
        static ENumAverage::E &Ref();
        operator ENumAverage::E() { return Ref(); }
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
