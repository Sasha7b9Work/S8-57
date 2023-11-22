// 2023/11/22 13:39:53 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


namespace IntRAM
{
    // ��������� �� 16k ���� ������, ��������������� ��� ������ �����������
    uint16 *Averager32k(Chan::E ch);

    // ������ ��� ������ ������ � ������ �������������
    uint8 *DataRand(Chan::E ch);

    // ��������� �� ������ ����������� ������
    DataSettings *PrepareForP2P();
};
