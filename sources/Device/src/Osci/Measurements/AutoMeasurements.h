// 2023/11/21 13:15:01 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Osci/Measurements/Measures.h"
#include "Keyboard/Keyboard.h"


struct TableMeasures
{
    // Отрисовать результаты автоматических измерений
    static void Draw();

    static int NumCols();

    static int NumRows();
    // На сколько сжимать сетку по горизонтали
    static int GetDeltaGridLeft();
    // Возвращает высоту поля для вывода автоматического измерения
    static int DY();
    // Возвращает ширину поля для вывода автоматического измерения
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


// Страница выбора измерений
struct PageChoiceMeasures
{
    // Обработка нажатий при открытой странице
    static void OnKeyEvent(const KeyEvent &event);
    // Нужно вызывать для открытия/закрытия страницы выбора
    void OnOpenCloseEvent();
    // Включить/отключить отображение
    static void ChangeState();

    static void Draw();
};


namespace AutoMeasurements
{
    // \brief Установить сигнал для обработки. Данные берутся из DS, inA, inB.
    // Выходные данные, соответствующие текущим настройками set, рассчитываются сразу и записываются в outA, outB.
    void SetData();

    // Возвращает true, если данные установлены
    bool DataIsSetting();

    // Расчитать все автоматические измерения
    void CalculateMeasures();

    // Возвращает активное измерение
    Measure GetActiveMeasure();

    char *Freq2String(float, bool, char buffer[20]);

    char *Float2String(float, bool, char buffer[20]);

    extern int markerTime[Chan::Count][2];

    extern int markerVoltage[Chan::Count][2];

    // Позиция активного измерения (на котором курсор)
    extern int8 posActive;
};


// Отвечает за посылаение результата имерения в SCPI
struct AutoMeasuresSender
{
    // Назначить данный тип измерения к посылке в SCPI
    static void DesignateForSending(TypeMeasure::E type);

    // Тип измерения, которое нужно залать в SCPI. Если Count, то не надо ничего посылать
    static TypeMeasure::E sended;
};
