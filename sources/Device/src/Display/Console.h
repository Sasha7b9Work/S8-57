// 2023/11/22 13:37:59 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


namespace Console
{
    void Init();

    void Draw();

    void AddString(const char *string);

    // ���������� true, ���� ������� ������ ������������ �� ������
    bool IsShown();

    // ��� ������� ���� �������, ����� ������������ �������� ������������ ���������� ����� � �������
    void OnChanged_MaxStringsInConsole();
};
