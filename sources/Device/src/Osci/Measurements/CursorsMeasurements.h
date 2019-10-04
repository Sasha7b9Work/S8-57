#pragma once


struct CursorsMeasurements
{
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
    /// Возвращает строку процентных измерений по напряжению
    static String PercentsU(Chan::E source);
    /// Возвращает строку процентных измерений по времени
    static String PercentsT(Chan::E source);

    static void SetCursPosT_temp(Chan::E ch, int num, float value);
};
