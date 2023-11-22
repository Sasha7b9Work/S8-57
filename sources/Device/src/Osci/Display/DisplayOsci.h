// 2023/11/22 13:47:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"
#include "Menu/Pages/Include/PageTime.h"


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


namespace DisplayOsci
{
    void Update();

    void DrawScaleLine(int x, bool forTrigLev);

    // ���������� ������� ����, ��� ������� ��������� � �����������
    void SetFlagRedraw();


    namespace HiPart
    {
        void Draw();

        // ���������� ������ ����� - ������������� � ����� ������
        void DrawRightPart(int x0, int y0);
    };


    namespace BottomPart
    {
        void Draw(int x, int y);
    };


    namespace Accumulator
    {
        // ��� ������� ����� �������� ����� ������ ��������� ��������
        void NextFrame();

        // ����� ����������
        void Reset();
    };


    // ���������� ��������� �� ����� ������ �������
    namespace ShiftInMemory
    {
        // ���������� ������ ������ ����� �� ������
        int16 Get();

        // ������������� ������ ������ ����� �� ������
        void Set(int16 shift);

        // �������� ��� ��������� TPos
        void OnChangeTPos();

        // �������� �������� � ������ �� delta �����
        void Change(int delta);

        // ���������� ������ ������ ����� �� ��������� ��� ������ tPos
        int16 Default(TPos::E tPos);
    };


    namespace PainterData
    {
        void DrawData();

        // ���������� ����� ������ � ��������� ����� �� ������ � ����������� ������
        BitSet64 PointsOnDisplay();

        // \brief ��������� ����� ������ � ��������� ����� � ����������� ������
        BitSet64 BytesOnDisplay();

        extern bool needSendToSCPI_FFT;

        // ���������� ���������� ������ - ��������������� ������� ����������
        void DrawCurrent();

        // ���������� ������ �� ���
        void DrawRAM();

        // ���������� ������ �� ����
        void DrawROM();

        void DrawChannel(Chan::E ch);

        void DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale);

        void DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x);

        void DrawModeLinesPeakDetOff(int center, const uint8 *data, float scale, int x);

        void DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale);

        void DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x);

        void DrawModePointsPeakDetOff(int center, const uint8 *data, float scale, int x);

        // ���������� ������
        void DrawSpectrum();

        void DrawSpectrum(const uint8 *dataIn, int numPoints, Chan::E ch);

        void DrawSpectrumChannel(const float *spectrum, Color color);

        void WriteParametersFFT(Chan::E ch, float freq0, float density0, float freq1, float density1);
    };


    struct MemoryWindow
    {
        static void Draw();

        static int X();

        static int Y();

        static int Width();

        static int Height();
    };


    // ��������� ��� ������������ ���������, �������� �������� ���������� ������������ ������ ����� (� ������� ���������� ������� ������ ����, ��� ���� �������� ���������)
    struct DrawingValueParameter
    {
        enum E
        {
            TrigLevel,
            RangeA,
            RangeB,
            RShiftA,
            RShiftB,
            TBase,
            TShift
        };

        // ����� ��������� ��������� (�������� �����) ����� �������� ��� �������
        static void Enable(DrawingValueParameter::E v);

        static void Draw();

    private:
        static bool                     needDrawParameter;
        static DrawingValueParameter::E parameter;

        static void Disable();

        // ���������� ������ � �������������� �� ������ y
        static void DrawBoundedText(int y, const String *text, Color color);
    };

    // ������� ����, ��� ������� ��������� � ������ �����������
    extern bool needRedraw;
};
