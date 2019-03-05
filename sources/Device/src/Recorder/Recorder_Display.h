#pragma once


namespace Recorder
{
    namespace Display
    {
        void Update();
        /// Сместить окно просмотра влево
        void MoveLeft();
        /// Сместить окно просмотра вправо
        void MoveRight();
        /// Переместить текущий курсор влево
        void MoveCursorLeft();
        /// Переместить текущий курсор вправо
        void MoveCursorRight();
    }
}
