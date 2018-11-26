#pragma once
#include "FPGA/FPGATypes.h"
#include "Data/Reader.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PainterData
{
public:
    
    static void DrawData();

    static void DrawChannel(Chan::E ch);

private:
    /// Нарисовать актуальные данные - соответствующие текущим установкам
    static void DrawCurrent();
    /// Нарисовать данные из ОЗУ
    static void DrawRAM();
    /// Нарисовать данные из ППЗУ
    static void DrawROM();

    static void DrawTPos(int leftX, int rightX);

    static void DrawTShift(int leftX, int rightX, int numPoints);
    /// Возвращает (-1), если точка не считана (NONE_VALUE)
    static int Ordinate(uint8 x, float scale);
    /// \brief Возвращает точку в экранной координате. Если точка не считана (NONE_VALUE), возвращает -1.
    /// Процедура ограничивает width числом 255.
    static void SendToDisplayDataInRect(Chan::E chan, int x, int *min, int *max, int width);

    static StructDataDrawing *dataStruct;
};
