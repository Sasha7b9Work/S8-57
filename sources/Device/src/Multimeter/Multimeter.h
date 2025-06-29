// 2023/11/22 13:47:45 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace DisplayMultimeter
{
    void Update();

    // ����� ��� ������� ��������� ��������� �� �������
    void SetMeasure(const uint8 buffer[13]);

    void ChangedMode();

    int Width();

    const int HEIGHT = 30;
};


namespace Multimeter
{
    // �������������
    void Init();

    void DeInit();

    // �������� �����������, ��� ��������� ��������� ������ � ����� ��������� ���������� ��������� ����� �������
    void ChangeMode();

    void Calibrate(int calibr);

    void ChangeAVP();

    // ���� (zero != 0) - ������� ����� ����
    void LoadZero(int zero);

    void Update();
};


// ����� ��������� �����������
struct MultimeterMeasure
{
#define S_MULT_MEASURE                  (set.mult._meas)
#define S_MULT_MEASURE_IS_CURRENT_DC    (S_MULT_MEASURE == MultimeterMeasure::CurrentDC)
#define S_MULT_MEASURE_IS_CURRENT_AC    (S_MULT_MEASURE == MultimeterMeasure::CurrentAC)
#define S_MULT_MEASURE_IS_RESISTANCE    (S_MULT_MEASURE == MultimeterMeasure::Resistance)
#define S_MULT_MEASURE_IS_TEST_DIODE    (S_MULT_MEASURE == MultimeterMeasure::TestDiode)
#define S_MULT_MEASURE_IS_BELL          (S_MULT_MEASURE == MultimeterMeasure::Bell)

    enum E
    {
        VoltageDC,
        VoltageAC,
        CurrentDC,
        CurrentAC,
        Resistance,
        TestDiode,
        Bell,
        Count
    };
    static char Symbol();

    // �������� ��� ��������� �� ��������� ������
    static MultimeterMeasure::E GetCode(const char buffer[13]);
};


// ��� - ��������� �������
struct ModeAVP
{
#define S_MULT_MODE_AVP     (set.mult._avp)
#define S_MULT_AVP_DISABLED (S_MULT_MODE_AVP == ModeAVP::Off)
#define S_MULT_AVP_ENABLED  (S_MULT_MODE_AVP == ModeAVP::On)

    enum E
    {
        Off,
        On
    };
};


// ������ ��������� ����������� ����������
struct RangeVoltageDC
{
#define S_MULT_RANGE_VOLTAGE_DC (set.mult._rangeVoltageDC)

    enum E
    {
        _2V,
        _20V,
        _500V
    };
};


// ������ ��������� ����������� ����������
struct RangeVoltageAC
{
#define S_MULT_RANGE_VOLTAGE_AC     (set.mult._rangeVoltageAC)

    enum E
    {
        _2V,
        _20V,
        _400V
    };
};


// ������ ��������� ����������� ����
struct RangeCurrentAC
{
#define S_MULT_RANGE_CURRENT_AC         (set.mult._rangeCurrentAC)
#define S_MULT_RANGE_CURRENT_AC_IS_20mA (S_MULT_RANGE_CURRENT_AC == RangeCurrentAC::_20mA)

    enum E
    {
        _20mA,
        _2A
    };
};


// ������ ��������� ����������� ����
struct RangeCurrentDC
{
#define S_MULT_RANGE_CURRENT_DC         (set.mult._rangeCurrentDC)
#define S_MULT_RANGE_CURRENT_DC_IS_20mA (S_MULT_RANGE_CURRENT_DC == RangeCurrentDC::_20mA)

    enum E
    {
        _20mA,
        _2A
    };
};


// ������ ��������� ������������ ����������� ����
struct RangeResistance
{
#define S_MULT_RANGE_RESISTANCE         (set.mult._rangeResist)
#define S_MULT_RANGE_RESISTANCE_IS_10M  (S_MULT_RANGE_RESISTANCE == RangeResistance::_10M)

    enum E
    {
        _2k,
        _20k,
        _200k,
        _10M
    };
};
