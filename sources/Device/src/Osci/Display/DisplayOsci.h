// 2023/11/22 13:47:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"
#include "Menu/Pages/Include/PageTime.h"


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


struct DisplayOsci
{
    static void Update();

    void DrawScaleLine(int x, bool forTrigLev);

    // Установить признак того, что дисплей нуждается в перерисовке
    static void SetFlagRedraw();


    struct HiPart
    {
        static void Draw();
    private:
        // Нарисовать правую часть - синхронизация и режим работы
        static void DrawRightPart(int x0, int y0);
    };


    struct BottomPart
    {
        static void Draw(int x, int y);
    };


    struct Accumulator
    {
        // Эту функцию нужно вызывать после каждой отрисовки сигналов
        static void NextFrame();

        // Сброс информации
        static void Reset();
    };


    // Управление выводимой на экран частью сигнала
    struct ShiftInMemory
    {
        // Возвращает индекс первой точки на экране
        static int16 Get();

        // Устанавливает индекс первой точки на экране
        static void Set(int16 shift);

        // Вызывать при изменении TPos
        static void OnChangeTPos();

        // Изменить смещение в памяти на delta точек
        static void Change(int delta);

        // Возвращает индекс первой точки по умолчанию при данном tPos
        static int16 Default(TPos::E tPos);
    };


    struct PainterData
    {
        static void DrawData();

        // Возвращает адрес первой и последней точки на экране в координатах экрана
        static BitSet64 PointsOnDisplay();

        // \brief Возращает адрес первой и последней точки в координатах экрана
        static BitSet64 BytesOnDisplay();

        static bool needSendToSCPI_FFT;

    private:

        // Нарисовать актуальные данные - соответствующие текущим установкам
        static void DrawCurrent();

        // Нарисовать данные из ОЗУ
        static void DrawRAM();

        // Нарисовать данные из ППЗУ
        static void DrawROM();

        static void DrawChannel(Chan::E ch);

        static void DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale);

        static void DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x);

        static void DrawModeLinesPeakDetOff(int center, const uint8 *data, float scale, int x);

        static void DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale);

        static void DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x);

        static void DrawModePointsPeakDetOff(int center, const uint8 *data, float scale, int x);

        // Нарисовать спектр
        static void DrawSpectrum();

        static void DrawSpectrum(const uint8 *dataIn, int numPoints, Chan::E ch);

        static void DrawSpectrumChannel(const float *spectrum, Color color);

        static void WriteParametersFFT(Chan::E ch, float freq0, float density0, float freq1, float density1);
    };


    struct MemoryWindow
    {
        static void Draw();

        static int X();

        static int Y();

        static int Width();

        static int Height();
    };


    // Структура для обслуживания параметра, значение которого необходимо отрисовывать поверх сетки (в течение некоторого времени поисле того, как этот параметр изменился)
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

        // После изменения параметра (вращения ручки) нужно вызывать эту функцию
        static void Enable(DrawingValueParameter::E v);

        static void Draw();

    private:
        static bool                     needDrawParameter;
        static DrawingValueParameter::E parameter;

        static void Disable();

        // Отобразить строку в прямоугольнике на высоте y
        static void DrawBoundedText(int y, const String *text, Color color);
    };

private:

    // Признак того, что дисплей нуждается в полной перерисовке
    static bool needRedraw;
};
