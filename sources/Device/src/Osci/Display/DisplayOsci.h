// 2023/11/22 13:47:31 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/ParametersOsci.h"
#include "Menu/Pages/Include/PageTime.h"


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


namespace DisplayOsci
{
    void Update();

    void DrawScaleLine(int x, bool forTrigLev);

    // Установить признак того, что дисплей нуждается в перерисовке
    void SetFlagRedraw();


    namespace HiPart
    {
        void Draw();

        // Нарисовать правую часть - синхронизация и режим работы
        void DrawRightPart(int x0, int y0);
    };


    namespace BottomPart
    {
        void Draw(int x, int y);
    };


    namespace Accumulator
    {
        // Эту функцию нужно вызывать после каждой отрисовки сигналов
        void NextFrame();

        // Сброс информации
        void Reset();
    };


    // Управление выводимой на экран частью сигнала
    namespace ShiftInMemory
    {
        // Возвращает индекс первой точки на экране
        int16 Get();

        // Устанавливает индекс первой точки на экране
        void Set(int16 shift);

        // Вызывать при изменении TPos
        void OnChangeTPos();

        // Изменить смещение в памяти на delta точек
        void Change(int delta);

        // Возвращает индекс первой точки по умолчанию при данном tPos
        int16 Default(TPos::E tPos);
    };


    namespace PainterData
    {
        void DrawData();

        // Возвращает адрес первой и последней точки на экране в координатах экрана
        BitSet64 PointsOnDisplay();

        // \brief Возращает адрес первой и последней точки в координатах экрана
        BitSet64 BytesOnDisplay();

        extern bool needSendToSCPI_FFT;

        // Нарисовать актуальные данные - соответствующие текущим установкам
        void DrawCurrent();

        // Нарисовать данные из ОЗУ
        void DrawRAM();

        // Нарисовать данные из ППЗУ
        void DrawROM();

        void DrawChannel(Chan::E ch);

        void DrawModeLines(Chan::E ch, int left, int center, const uint8 *data, float scale);

        void DrawModeLinesPeakDetOn(int center, const uint8 *data, float scale, int x);

        void DrawModeLinesPeakDetOff(int center, const uint8 *data, float scale, int x);

        void DrawModePoints(Chan::E ch, int left, int center, const uint8 *data, float scale);

        void DrawModePointsPeakDetOn(int center, const uint8 *data, float scale, int x);

        void DrawModePointsPeakDetOff(int center, const uint8 *data, float scale, int x);

        // Нарисовать спектр
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

    // Признак того, что дисплей нуждается в полной перерисовке
    extern bool needRedraw;
};
