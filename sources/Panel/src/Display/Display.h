#pragma once
#include "defines.h"



class Display
{
public:
    static const int WIDTH = 320;
    static const int HEIGHT = 240;

    static void Init();

    static uint8 *GetBuffer();

    static uint8 *GetBufferEnd();

    static void ToggleBuffers();

private:
    // Экран
    static uint8 *frontBuffer;
    // Задний буфер. В нём происходит отрисовка, и затем изображение копируется во frontBuffer
    static uint8 *backBuffer;
};


extern LTDC_HandleTypeDef hltdc;
