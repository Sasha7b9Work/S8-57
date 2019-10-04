#pragma once
#include "Utils/Stack.h"



class FrequencyCounter
{
public:
    /// �������������.
    static void Init();
    /// ������� ��������� ��� �������� �������
    static void LoadFreqSettings();
    /// ������� �������� ��� �������� �������
    static void LoadPeriodSettings();

    static void Update();
    /// ���������� ���������� �������� �������
    static float GetFreq();
    /// ��������� ��������
    static void Draw();

private:
    /// ���������� ��������� �������� ��������� � ���������, ��������������� �������� �������
    static void SetStateLamps();

    static void SetStateLampFreq();

    static void SetStateLampPeriod();

    static void ReadFreq();

    static void ReadPeriod();

    static void LoadSettings();

    static float FreqSetToFreq(const BitSet32 *fr);

    static float PeriodSetToFreq(const BitSet32 *period);
    /// ������������� � true �������� ��������, ��� ������� ����� ������� �� �������� �������
    static bool readPeriod;

    static float prevFreq;

    static float frequency;

    static float period;
};