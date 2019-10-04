#pragma once


#define SHIFT_IN_MEMORY     DisplayOsci::PainterData::FirstPointOnScreen()


struct DisplayOsci
{
    static void Update();

    static void DrawCursorTrigLevel();

    void DrawScaleLine(int x, bool forTrigLev);
    /// Установить признак того, что дисплей нуждается в перерисовке
    static void SetFlagRedraw();

    struct HiPart
    {
        static void Draw();
    };

    struct BottomPart
    {
        static void Draw(int x, int y);
    };

    struct Accumulator
    {
        /// Эту функцию нужно вызывать после каждой отрисовки сигналов
        static void NextFrame();
        /// Сброс информации
        static void Reset();
    };

    struct PainterData
    {
        static void DrawData();
        /// Индекс первой точки, выводимой поверх сетки
        static int FirstPointOnScreen();
        /// Возвращает адрес первой и последней точки на экране в координатах экрана
        static BitSet64 PointsOnDisplay();
        /// \brief Возращает адрес первой и последней точки в координатах экрана
        static BitSet64 BytesOnDisplay();
        /// Эту функцию надо вызывать при переключении TPos для перерасчёта смещения первого выводимого байта относительно левого края экрана
        static void ChangeTPos();
    };

    struct MemoryWindow
    {
        static void Draw();

        static int X();

        static int Y();

        static int Width();

        static int Height();
    };
};
