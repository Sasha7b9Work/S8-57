// 2023/11/22 13:38:17 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Font_d.h"


namespace DAdvancedFont
{
    // ���������� ����������� �����
    void Set(DTypeFont::E);

    // ���������� true, ���� � ������ row ���� �������
    bool RowNotEmpty(uint8 symbol, int row);

    // ���������� ������ �������
    uint8 GetWidth(uint8 symbol);

    // ���������� ������ �������
    uint8 GetHeight(uint8 symbol);

    // ���������� ������ ������ �������� �������
    uint8 GetHeight();

    bool BitIsExist(uint8 symbol, int row, int bit);
};
