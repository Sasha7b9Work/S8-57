#pragma once


namespace Osci
{
    namespace Display
    {
        void Update();

        void DrawCursorTrigLevel();

        void DrawScaleLine(int x, bool forTrigLev);
        /// Установить признак того, что дисплей нуждается в перерисовке
        void SetFlagRedraw();

        namespace HiPart
        {
            void Draw();
        };

        namespace BottomPart
        {
            void Draw(int x, int y);

            void WriteCursors();
        };
    }
}
