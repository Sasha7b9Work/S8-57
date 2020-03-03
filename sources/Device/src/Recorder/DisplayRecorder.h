#pragma once


struct Record;


class DisplayRecorder
{
public:

    static void Update();

    // Сместить окно просмотра влево
    static void MoveLeft();

    // Сместить окно просмотра вправо
    static void MoveRight();

    // Переместить текущий курсор влево
    static void MoveCursorLeft();

    // Переместить текущий курсор вправо
    static void MoveCursorRight();

    // 
    static bool InProcessUpdate();

private:
    
    // Изобразить установленные настройки
    static void DrawSettings(int x, int y);
    
    // Отобразить данные
    static void DrawData(Record *record);

    // Нарисовать данные канала
    static void DrawChannel(Record *record, Chan::E ch);

    // Нарисовать данные датчика
    static void DrawSensor(Record *record);


    static void DrawMemoryWindow();

    // Возвращает значение Y экрана для value точки
    static int Y(int value);


    static void DrawCursors();


    static void DrawParametersCursors();


    static char *TimeCursor(int numCur, char buffer[20]);


    static char *VoltageCursor(Chan::E, int, char[20]);


    static char *DeltaTime(char buffer[20]);

    // true, если в данный момент происходит отрисовка
    static bool inProcessUpdate;


    static uint16 posCursor[2];

    // С этой точки начинается вывод
    static int startPoint;
};
