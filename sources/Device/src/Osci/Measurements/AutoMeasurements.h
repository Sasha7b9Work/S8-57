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


    struct Cell
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


struct AutoMeasurements
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
