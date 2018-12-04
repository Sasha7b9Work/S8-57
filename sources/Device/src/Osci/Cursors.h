#pragma once
#include "Settings/SettingsChannel.h"
#include "Utils/String.h"


/// Перемемещение курсоров при вращении ручки УСТАНОВКА - по точкам или процентам
#define CURS_MOVEMENT               (set.curs_movement)
/// Курсоры перемещаются при вращении ручка УСТАНОВКА с дискретностью 1%
#define CURS_MOVEMENT_IN_PERCENTS   (CURS_MOVEMENT == Cursors::Movement::Percents)
/// Курсоры перемещаются при вращении ручка УСТАНОВКА с дискретностью 1 пиксель
#define CURS_MOVEMENT_IN_PIXELS     (CURS_MOVEMENT == Cursors::Movement::Pixels)

/// Какими курсорами управляет ручка УСТАНОВКА
#define CURS_ACTIVE                 (set.curs_active)
/// Ручка УСТАНОВКА управляет курсорами напряжения
#define CURS_ACTIVE_U               (CURS_ACTIVE == Cursors::Active::U)
/// Ручка УСТАНОВКА управляет курсорами времени
#define CURS_ACTIVE_T               (CURS_ACTIVE == Cursors::Active::T)

/// Позиция курсора напряжения
#define CURsU_POS(ch, num)          (set.curs_posCurU[ch][num])
/// Позиция куросра времени
#define CURsT_POS(ch, num)          Cursors::PosT(ch, num)

/// Режим слежения за курсорами
#define CURS_LOOK_MODE(ch)          (set.curs_lookMode[ch])
/// Включено слежение за курсорами напряжения
#define CURS_LOOK_U(ch)             (CURS_LOOK_MODE(ch) == Cursors::LookMode::Voltage)
/// Включено слежение за курсорами времени
#define CURS_LOOK_T(ch)             (CURS_LOOK_MODE(ch) == Cursors::LookMode::Time)
/// Включено слежение за курсорами времени и напряжения
#define CURS_LOOK_BOTH(ch)          (CURS_LOOK_MODE(ch) == Cursors::LookMode::Both)

#define CURsU_CNTRL_CH(ch)          (set.curs_cntrlU[ch])
#define CURsU_CNTRL                 (CURsU_CNTRL_CH(CURS_SOURCE))
#define CURsU_CNTRL_1               (CURsU_CNTRL == Cursors::Control::_1)
#define CURsU_CNTRL_2               (CURsU_CNTRL == Cursors::Control::_2)
#define CURsU_CNTRL_1_2             (CURsU_CNTRL == Cursors::Control::_1_2)
#define CURsU_DISABLED              (CURsU_CNTRL == Cursors::Control::Disable)
#define CURsU_ENABLED               (!CURsU_DISABLED)

#define CURsT_CNTRL_CH(ch)          (set.curs_cntrlT[ch])
#define CURsT_CNTRL                 (CURsT_CNTRL_CH(CURS_SOURCE))
#define CURsT_CNTRL_1               (CURsT_CNTRL == Cursors::Control::_1)
#define CURsT_CNTRL_2               (CURsT_CNTRL == Cursors::Control::_2)
#define CURsT_CNTRL_1_2             (CURsT_CNTRL == Cursors::Control::_1_2)
#define CURsT_DISABLED              (CURsT_CNTRL == Cursors::Control::Disable)
#define CURsT_ENABLED               (!CURsT_DISABLED)

#define CURSORS_SHOW_FREQ           (set.curs_showFreq)

/// Курсоры какого канала сейчас активны
#define CURS_SOURCE                 (set.curs_source)
/// Активны курсоры канала A
#define CURS_SOURCE_A               (CURS_SOURCE == Chan::A)
/// Активны курсоры канала B
#define CURS_SOURCE_B               (CURS_SOURCE == Chan::B)

/// Абсолютное значение напряжения, соответствующее 100%
#define dUperc(ch)                  (set.curs_deltaU100percents[ch])
/// Абсолютное значение времени, соотвествующее 100%
#define dTperc(ch)                  (set.curs_deltaT100percents[ch])

#define CURS_SHOW                   (set.curs_showCursors)


class Cursors
{
public:
    /// Нарисовать курсоры и результаты курсорных измерений
    static void Draw();
    /// Возвращает true,если нужно рисовать курсоры
    static bool  NecessaryDraw();
    /// Получить позицию курсора напряжения
    static float PosU(Chan::E ch, int numCur);
    /// Возвращает значение курсора времени
    static float PosT(Chan::E ch, int num);
    /// Получить строку курсора напряжения
    static String Voltage(Chan::E source, int numCur);
    /// Получть строку курсора времени
    static String Time(Chan::E source, int numCur);

    static void SetCursPosT_temp(Chan::E ch, int num, float value);

public:
    /// Дискретность перемещения курсоров.
    struct Movement
    {
        enum E
        {
            Pixels,    ///< По пикселям экрана
            Percents   ///< По процентам
        } value;
        explicit Movement(E v) : value(v) {};
    };

    /// Какие курсоры сейчас активны. Какие активны, те и будут перемещаться по вращению ручки УСТАНОВКА.
    struct Active
    {
        enum E
        {
            U,
            T,
            None
        } value;
        explicit Active(E v) : value(v) {};
    };

    /// Режим слежения курсоров.
    struct LookMode
    {
        enum E
        {
            None,      ///< Курсоры не следят.
            Voltage,   ///< Курсоры следят за напряжением автоматически.
            Time,      ///< Курсоры следят за временем автоматически.
            Both       ///< Курсоры следят за временем и напряжением, в зависимости от того, какой курсоры вращали последним.
        } value;
        explicit LookMode(E v) : value(v) {};
    };

    /// Каким курсором управлять
    struct Control
    {
        enum E
        {
            _1,        ///< первым
            _2,        ///< вторым
            _1_2,      ///< обоими
            Disable    ///< никаким
        } value;
        explicit Control(E v) : value(v) {};
    };
};
