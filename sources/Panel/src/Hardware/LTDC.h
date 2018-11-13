#pragma once


//////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LTDC_
{
friend class Display;

private:

    static void Init(uint front, uint back);

public:

    static void SetColors(uint clut[], uint numColors);

    static void ToggleBuffers();

private:

    static void SetBuffers(uint frontBuffer, uint backBuffer);
};
