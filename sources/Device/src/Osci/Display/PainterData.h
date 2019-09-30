#pragma once
#include "Data/Reader.h"


#define SHIFT_IN_MEMORY     Osci::Display::PainterData::FirstPointOnScreen()



namespace Osci
{
    namespace Display
    {
        namespace PainterData
        {
            void DrawData();
            /// Индекс первой точки, выводимой поверх сетки
            int FirstPointOnScreen();
            /// Возвращает адрес первой и последней точки на экране в координатах экрана
            BitSet64 PointsOnDisplay();
            /// \brief Возращает адрес первой и последней точки в координатах экрана
            BitSet64 BytesOnDisplay();
            /// Эту функцию надо вызывать при переключении TPos для перерасчёта смещения первого выводимого байта относительно левого края экрана
            void ChangeTPos();
        };
    };
};
