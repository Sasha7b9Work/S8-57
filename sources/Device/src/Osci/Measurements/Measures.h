#pragma once
#include "Settings/SettingsOsci.h"



struct Measure
{
public:
    // ¬иды измерений
    struct Type
    {
        enum E
        {
            None,
            VoltageMax,
            VoltageMin,
            VoltagePic,
            VoltageMaxSteady,
            VoltageMinSteady,
            VoltageAmpl,
            VoltageAverage,
            VoltageRMS,
            VoltageVybrosPlus,
            VoltageVybrosMinus,
            Period,
            Freq,
            TimeNarastaniya,
            TimeSpada,
            DurationPlus,
            DurationMinus,
            SkvaznostPlus,
            SkvaznostMinus,
            DelayPlus,
            DelayMinus,
            PhazaPlus,
            PhazaMinus,
            Number
        } value;
        explicit Type(E v) : value(v) {};
    };

    Measure(int r, int c) : row(r), col(c) {};

    Type::E GetType();

    String GetStringMeasure(Chan::E ch, char *buffer, int lenBuf);
    /// ”станавливает активным следующее или предыдущее измерение
    static void ChangeActive(int delta);
    /// –ассчитать позицию курсора напр€жени€, соответствующю заданной позиции курсора posCurT
    static float CalculateCursorU(Chan::E ch, float posCurT);
    /// –ассчитать позицию курсора времени, соответствующую заданной позиции курсора напр€жени€ posCurU
    static float CalculateCursorT(Chan::E ch, float posCurU, int numCur);

    static void SetMarkerVoltage(Chan::E ch, int num, float value);

    static void SetMarkerTime(Chan::E ch, int num, int value);

    static char GetChar(Type::E measure);

    static void ShortPressOnSmallButonMarker();
    /// ¬озвращает true, если измерение активное - выбрано ручкой
    bool IsActive();

    String Name();

    static String GetName(Type::E type);

private:

    /// —делать активным
    static void SetActive(int row, int col);
    /// —трока в таблице, в которой находитс€ данное измерение
    int row;
    ///  олонка в таблице, в которой находитс€ данное измерение
    int col;
};
