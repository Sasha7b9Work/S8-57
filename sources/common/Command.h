#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct Command
{           //  смещение               0  |     1    |     2      |      3      |     4        |      5       |   6    |         Размер
    enum E
    {
/* 00 */    None,                   //    |          |            |             |              |              |        |
/* 01 */    ButtonPress,            // 01 |  Control | TypePress  |             |              |              |        |
/* 02 */    Paint_BeginScene,       // 02 |          |            |             |              |              |        |
/* 03 */    Paint_EndScene,         // 03 |          |            |             |              |              |        |
/* 04 */    Paint_SetColor,         // 04 |   Color  |            |             |              |              |        |
/* 05 */    Paint_FillRegion,       // 05 |   x[0:7] |  x[8:15]   |      y      | width[0:7]   | width[8:15]  | height |
/* 06 */    Paint_DrawText,         // 06 |   x[0:7] |  x[8:15]   |      y      | num symbols  |              |        |
/* 07 */    Paint_SetPalette,       // 07 | numColor | value[0:7] | value[8:15] | value[16:23] | value[24:31] |        |
/* 08 */    Paint_DrawRectangle,    // 08 |   x[0:7] |  x[8:15]   |      y      | width[0:7]   | width[8:15]  | height |
/* 09 */    Paint_DrawVLine,        // 09 |   x[0:7] |  x[8:15]   |      y0     |      y1      |              |        |
/* 10 */    Paint_DrawHLine,        // 10 |    y     |  x0[0:7]   |   x0[8:15]  |   x1[0:7]    |  x1[8:15]    |        |
/* 11 */    Paint_SetFont,          // 11 | typeFont |            |             |              |              |        |
/* 12 */    Paint_SetPoint,         // 12 |   x[0:7] |  x[8:15]   |      y      |              |              |        |
/* 13 */    Paint_DrawLine,         // 13 |  x0[0:7] | x0[8:15]   |     y0      |   x1[0:7]    |   x1[8:15]   |   y1   |
//  В первом байте хранится режим - младший бит == 0 - линии, 0 - точки
//  В старшем полубайте хранится количество усреднений 0 - 1, 1 - 2, 2 - 4, 3 - 8, 4 - 16, 5 - 32, 6 - 64, 7 - 128, 8 - 256
/* 14 */    Paint_TesterLines,      // 14 |   mode   |   Color    | x[TESTER_NUM_POINTS]        y[TESTER_NUM_POINTS]         3 + 240 * 2 + 240 = 723
/* 15 */    Paint_DrawBigText,      // 15 |   x[0:7] |  x[8:15]   |      y      | num symbols  |              |        |
/* 16 */    Screen,                 // 16 |  numRow  |
/* 17 */    Paint_VPointLine,       // 17 |   x[0:7] |  x[8:15]   |      y      |   delta      | count        |
/* 18 */    Paint_HPointLine,       // 18 |   x[0:7] |  x[8:15]   |      y      |   delta      | count        |
/* 19 */    Paint_SetMinWidthFont,  // 19 |   width  |
/* 20 */    Paint_SetTextSpacing,   // 20 |   0 | 1  |
/* 21 */    Size
    } value;
};


#define TESTER_NUM_POINTS 240


#define SIZE_STRING (320)
