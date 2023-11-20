#pragma once


namespace Grid
{
    // ��������� ������������ ��� ������� ��������, ������� �������� ����������� ��� ����� �������� � ���������� ����� - ��������, ��������� ����
    static const int WIDTH = 280;
    static const int HEIGHT = 200;
    static const int SIZE_CELL = 20;

    void Draw();

    // ��� �������� ����� �������� � ����������� �� ������������� ������
    int Left();
    int Top();
    int Width();
    int Height();
    int Bottom();
    int Right();
    int ChannelCenterHeight();
    int ChannelHeight();
    int FullBottom();
    int MathBottom();
    int MathHeight();
    int ChannelBottom();
    // ���������� ������ y-���������� ����, � ������� ����� �������� ��������������
    int BottomForWarnings();
};
