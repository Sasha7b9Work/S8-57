// 2023/11/22 13:20:16 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "common/Command.h"


typedef bool(*pFuncBU8)(uint8);


namespace DDecoder
{
    void AddData(uint8 data);

    // ���������� true, ���� ���� ���������
    bool Update();

    // ���������� ������ ������, ���������� ���������
    int BytesInBuffer();

    // ���������� ��������� �� ������ ������� �������������� ������
    uint8 *Buffer();
};
