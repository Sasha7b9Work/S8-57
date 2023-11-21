// 2023/11/21 13:15:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"


struct TableMeasures
{
    // ���������� ���������� �������������� ���������
    static void Draw();

    static int NumCols();

    static int NumRows();
    // �� ������� ������� ����� �� �����������
    static int GetDeltaGridLeft();
    // ���������� ������ ���� ��� ������ ��������������� ���������
    static int DY();
    // ���������� ������ ���� ��� ������ ��������������� ���������
    static int DX();


    struct Cell
    {
    public:
        Cell(int _row, int _col) : row(_row), col(_col) {};
        void Draw(int x, int y);

    private:
        void DrawStringMeasure(int x, int y);

        int row;
        int col;
    };
};


// �������� ������ ���������
struct PageChoiceMeasures
{
    // ��������� ������� ��� �������� ��������
    static void OnKeyEvent(const KeyEvent &event);
    // ����� �������� ��� ��������/�������� �������� ������
    void OnOpenCloseEvent();
    // ��������/��������� �����������
    static void ChangeState();

    static void Draw();
};


namespace AutoMeasurements
{
    // \brief ���������� ������ ��� ���������. ������ ������� �� DS, inA, inB.
    // �������� ������, ��������������� ������� ����������� set, �������������� ����� � ������������ � outA, outB.
    void SetData();

    // ���������� true, ���� ������ �����������
    bool DataIsSetting();

    // ��������� ��� �������������� ���������
    void CalculateMeasures();

    // ���������� �������� ���������
    Measure GetActiveMeasure();

    char *Freq2String(float, bool, char buffer[20]);

    char *Float2String(float, bool, char buffer[20]);

    extern int markerTime[Chan::Count][2];

    extern int markerVoltage[Chan::Count][2];

    // ������� ��������� ��������� (�� ������� ������)
    extern int8 posActive;
};


// �������� �� ���������� ���������� �������� � SCPI
struct AutoMeasuresSender
{
    // ��������� ������ ��� ��������� � ������� � SCPI
    static void DesignateForSending(TypeMeasure::E type);

    // ��� ���������, ������� ����� ������ � SCPI. ���� Count, �� �� ���� ������ ��������
    static TypeMeasure::E sended;
};
