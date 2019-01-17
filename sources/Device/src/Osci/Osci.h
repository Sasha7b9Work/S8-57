#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Osci
{
    void Init();

    void Update();

    void DeInit();

    void Stop(bool pause = false);

    namespace Display
    {
        void Update();

        void DrawCursorTrigLevel();

        void DrawScaleLine(int x, bool forTrigLev);

        void DrawMeasures();
    }
};
