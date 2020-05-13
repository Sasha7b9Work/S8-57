#pragma once


struct DataSettings;


class AveragerOsci
{
public:
    static void Process(Chan::E ch, const uint8 *newData, int size);
    static void SettingChanged();
};



class InterpolatorLinear
{
public:
    static void Run(DataSettings *ds);
};


class InterpolatorSinX_X
{
public:
    static void Run(DataSettings *ds);
};
