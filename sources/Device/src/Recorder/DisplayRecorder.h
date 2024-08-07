// 2023/11/22 13:46:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"


struct Record;


namespace DisplayRecorder
{
    void Update();

    // �������� ���� ��������� �����
    void MoveWindowLeft();

    // �������� ���� ��������� ������
    void MoveWindowRight();

    // ����������� ������� ������ �����
    void MoveCursorLeft();

    // ����������� ������� ������ ������
    void MoveCursorRight();

    // ���������� true, ���� ��� ��� ���������� ����������� ������
    bool InProcessUpdate();

    // ���������� ������������ ������
    void SetDisplayedRecord(Record *record);

    struct Cursor
    {
        enum E
        {
            _1,
            _2
        };
    };


    // �������� ����������� ���� �� ������ � ������ ���������
    struct SpeedWindow
    {
        enum E
        {
            Cell,           // 1 ������
            _1Window,       // 1 ����
            _10Windows      // 10 ����
        } value;

        SpeedWindow(E v) : value(v)  { }

        int NumPoints() const;      // ���������� ����� �����, �� ������� ����� �������� �����
    };

    extern SpeedWindow speed;
};
