// 2023/11/22 13:46:50 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace CursorsMeasurements
{
    // Нарисовать курсоры и результаты курсорных измерений
    void Draw();

    // Возвращает true,если нужно рисовать курсоры
    bool  NecessaryDraw();

    // Получить позицию курсора напряжения
    float PosU(Chan::E ch, int numCur);

    // Возвращает значение курсора времени
    float PosT(Chan::E ch, int num);

    // Получить строку курсора напряжения
    String Voltage(Chan::E source, int numCur);

    // Получть строку курсора времени
    String Time(Chan::E source, int numCur);

    // Возвращает строку процентных измерений по напряжению
    String PercentsU(Chan::E source);

    // Возвращает строку процентных измерений по времени
    String PercentsT(Chan::E source);

    void SetCursPosT_temp(Chan::E ch, int num, float value);
};
