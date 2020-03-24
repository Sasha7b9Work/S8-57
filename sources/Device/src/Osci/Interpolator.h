#pragma once
#include "defines.h"
#include "FPGA/FPGA.h"


class Interpolator
{
public:
    static void Run(uint8 *data, uint num);
};


static uint8 *begin = nullptr;  // Начало обрабатываемых данных
static uint8 *end = nullptr;    // Конец обрабатываемых данных


// Структура описывает отрезок
struct Segment
{
    uint8 *start;   // Указатель на первый элемент отрезка
    uint8 *end;     // Указатель на первый элемент за отрезком
};


// Находит первый пустой сегмент (т.е. в котором находятся толдько "пустые" элементы), начиная с элемента с адресом start
static bool FindEmptySegment(uint8 *start, Segment *segment);

// Интерполировать данные в сегменте
static void InterpolateSegment(Segment *segment);

// Находим адрес пустого элемента. Возвращает end, если такого элемента нет
static uint8 *FindEmptyElement(uint8 * const start);

// Найти первый "непустой" элемент данных. Возвращает end, если такого элемента нет
static uint8 *FindReadedElement(uint8 * const start);


void Interpolator::Run(uint8 *data, uint num)
{
    begin = data;
    end = begin + num;

    Segment segment;

    segment.end = begin;

    while(FindEmptySegment(segment.end, &segment))
    {
        InterpolateSegment(&segment);
    }
}


static bool FindEmptySegment(uint8 *start, Segment *segment)
{
    segment->start = FindEmptyElement(start);

    segment->end = FindReadedElement(segment->start);

    return (segment->start != end) && (segment->end != end);
}


static uint8 *FindEmptyElement(uint8 * const start)
{
    uint8 *element = start;

    while(element != end)
    {
        if(*element == VALUE::NONE)   // Если очередной элемент "пустой"
        {
            break;                  // то мы его нашли
        }
        element++;
    }

    return element;
}


static uint8 *FindReadedElement(uint8 * const start)
{
    uint8 *element = start;

    while(element != end)
    {
        if(*element != VALUE::NONE)
        {
            break;
        }
        element++;
    }

    return element;
}


static void InterpolateSegment(Segment *segment)
{
    if(segment->start == end || segment->start == begin || segment->end == end)
    {
        return;
    }

    float before = *(segment->start - 1);               // Значение перед "пустым" сегментом
    float after = *(segment->end);                      // Значение после "пустого" сегмента

    int numTicks = segment->end - segment->start + 2;   // На столько отрезков разбит наш сегмент

    float delta = (after - before) / numTicks;

    for(int tick = 0; tick < numTicks - 2; tick++)
    {
        float value = before + delta * (tick + 1);

        *(segment->start + tick) = static_cast<uint8>(value + 0.5F);
    }
}
