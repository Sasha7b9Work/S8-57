// 2023/11/21 13:48:42 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


struct DataSettings;


class AveragerOsci
{
public:
    static void Process(Chan::E ch, const uint8 *newData, int size);
    static void SettingChanged();
};



namespace InterpolatorLinear
{
    void Run(DataSettings *ds);
};


class InterpolatorSinX_X
{
public:
    static void Run(DataSettings *ds);
};
