#pragma once
#include "Keyboard/Keyboard.h"



#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

#define MOSI_HEIGHT         9
#define MOI_HEIGHT_TITLE    19
#define MOI_WIDTH_D_IP      34      /* ”величение ширины открытого ip-адреса в случае наличи€ порта */
#define GRID_DELTA          20      /*  оличество точек в клетке */

#define TIME_MESSAGES               5


#define DISPLAY_ORIENTATION         (set.dbg_Orientation)
#define DISPLAY_ORIENTATION_IS_NORMAL (DISPLAY_ORIENTATION == Display::Orientation::Normal)

#define SET_NUM_BYTES_ON_DISPLAY    (SET_PEAKDET_EN ? 281 * 2 : 281)

#define SHIFT_IN_MEMORY_IN_POINTS   Display::ShiftInMemoryInPoints()


struct DrawMode
{
    enum E
    {
        Auto,
        Hand
    } value;
};

struct Display
{
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    static bool IsRun();

    static void Init();
    
    // «десь происходит вс€ отрисовка
    static void Update();
    
    // ¬озвращаемое значение true означает, что дисплей находитс€ в состо€нии отрисовки
    static bool InProcess();
         
    static void LoadBrightness();
};
