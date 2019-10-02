#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"


struct TableMeasures
{
    /// Отрисовать результаты автоматических измерений
    static void Draw();

    static int NumCols();

    static int NumRows();
    /// На сколько сжимать сетку по горизонтали
    static int GetDeltaGridLeft();
    /// Возвращает высоту поля для вывода автоматического измерения
    static int DY();
    /// Возвращает ширину поля для вывода автоматического измерения
    static int DX();


    class Cell
    {
    public:
        Cell(int _row, int _col) : row(_row), col(_col) {};
        void Draw(int x, int y);

    private:
        void DrawStringMeasure(int x, int y);

        int row;
        int col;
    };
};

struct CursorsOsci
{
    /// Нарисовать курсоры и результаты курсорных измерений
    static void Draw();
    /// Возвращает true,если нужно рисовать курсоры
    static bool  NecessaryDraw();
    /// Получить позицию курсора напряжения
    static float PosU(Chan::E ch, int numCur);
    /// Возвращает значение курсора времени
    static float PosT(Chan::E ch, int num);
    /// Получить строку курсора напряжения
    static String Voltage(Chan::E source, int numCur);
    /// Получть строку курсора времени
    static String Time(Chan::E source, int numCur);
    /// Возвращает строку процентных измерений по напряжению
    static String PercentsU(Chan::E source);
    /// Возвращает строку процентных измерений по времени
    static String PercentsT(Chan::E source);

    static void SetCursPosT_temp(Chan::E ch, int num, float value);
};


/// Страница выбора измерений
struct PageChoiceMeasures
{
    /// Обработка нажатий при открытой странице
    static void OnKeyEvent(const KeyEvent &event);
    /// Нужно вызывать для открытия/закрытия страницы выбора
    void OnOpenCloseEvent();
    /// Включить/отключить отображение
    static void ChangeState();

    static void Draw();
};


struct MeasurementsOsci
{
    /// \brief Установить сигнал для обработки. Данные берутся из DS, inA, inB.
    /// Выходные данные, соответствующие текущим настройками set, рассчитываются сразу и записываются в outA, outB.
    static void SetData();
    /// Возвращает true, если данные установлены
    static bool DataIsSetting();
    /// Расчитать все автоматические измерения
    static void CalculateMeasures();
    /// Возвращает активное измерение
    static Measure GetActiveMeasure();

    static char *Freq2String(float, bool, char buffer[20]);

    static char *Float2String(float, bool, char buffer[20]);

    static int markerTime[Chan::Size][2];

    static int markerVoltage[Chan::Size][2];
    /// Позиция активного измерения (на котором курсор)
    static int8 posActive;
};
