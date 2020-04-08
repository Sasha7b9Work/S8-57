#pragma once
#include "defines.h"
#include "common/Command.h"
#include "Display/Colors.h"
#include "Display/DisplayTypes.h"
#include "Display/Font/Font_p.h"


struct Painter
{
    // Вызывается в начале отрисовки каждого кадра. Заполняет буфер цветом color
    static void BeginScene();
    
    // Вызывается в конце отрисовки каждого кадра. Переносит содержимое буфера на экран
    static void EndScene();
    
    // Пересылает строку в панель
    static void SendRow(int row);

    static void SetColor(Color color);
    
    // Возвращает текущий цвет рисования
    static Color GetColor();
    
    // Устанавливает цвету заданное значение. Загружается в дисплей LoadPalette() или SetPalette()
    static void SetColorValue(Color color, uint value);
    
    /// Загружает в дисплей все цвета
    static void LoadPalette();

    // Нарисовать данные тестер-компонента. В младшем бите - линиями или точками рисовать. В старшей тетраде - количество усреднений
    static void DrawTesterData(uint8 mode, Color color, const uint16 x[TESTER_NUM_POINTS], const uint8 y[TESTER_NUM_POINTS]);
    
    // Установить цвет рисования
    // Нарисовать точку текущим цветом
    static void SetPoint(int x, int y);

    static void DrawVPointLine(int x, int y, int delta, int count);

    static void DrawHPointLine(int x, int y, int delta, int count);
    
    // Нарисовать горизонтальную линию
    static void DrawHLine(int y, int x0, int x1);
    
    // Нарисовать вертикальную линию
    static void DrawVLine(int x, int y0, int y1);
    
    // Нарисовать произвольную линию
    static void DrawLine(int x0, int y0, int x1, int y1);
    
    // \brief Рисует прерывистую горизонтальную линию. dFill - длина штриха, dEmpty - расст. между штрихами. Линия всегда начинается со штриха. 
    // dStart указывает смещение первой рисуемой точки относительно начала штриха.
    static void DrawDashedHLine(int y, int x0, int x1, int dFill, int dEmpty, int dStart);
    
    // Рисует прерывистую вертикальную линию.
    static void DrawDashedVLine(int x, int y0, int y1, int dFill, int dEmpty, int dStart);

    static void DrawRectangle(int x, int y, int width, int height);

    static void FillRegion(int x, int y, int width, int height);

    static uint ReduceBrightness(uint colorValue, float newBrightness);

    static int DrawFormatText(int x, int y, char *format, ...);
    
    // Пишет строку в позиции x, y
    static int DrawFormText(int x, int y, Color color, pString text, ...);
    
    // Посылает строку изображения numString в устройство
    static void SendScreenToDevice();
};
