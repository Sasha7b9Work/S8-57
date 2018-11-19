#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define WIDTH_SCREEN    320
#define HEIGHT_SCREEN   240

#ifdef OPEN
#define BUFFER_WIDTH    1024
#define BUFFER_HEIGHT   600
#else
#define BUFFER_WIDTH    320
#define BUFFER_HEIGHT   240
#endif

#define HEIGHT_TITLE    19
#define WIDTH_ITEM      69
#define HEIGHT_ITEM     ((HEIGHT_SCREEN - HEIGHT_TITLE - 1) / 4)
#define HEIGHT_SIGNAL   ((HEIGHT_SCREEN - HEIGHT_TITLE) / 2)
#define WIDTH_SIGNAL    ((WIDTH_SCREEN - WIDTH_ITEM) - 1)
