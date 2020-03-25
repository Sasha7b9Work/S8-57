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
    volatile ShiftPoint::E *interpolated = randShift.points;
    
    for(int i = 0; i < 50; i++)
    {
        if(interpolated[i] == ShiftPoint::READED && begin[i] == VALUE::NONE)
        {
            begin[i] = VALUE::NONE;
        }
    }
    
    segment->start = FindEmptyElement(start);

    segment->end = FindReadedElement(segment->start);
    
    volatile int indexStart = segment->start - begin;
    volatile int indexEnd = segment->end - begin;
    
    indexStart = indexStart;
    indexEnd = indexEnd;

    bool result = (segment->start != end) && (segment->end != end);
    
    if(indexStart > 1000)
    {
        indexStart = indexStart;
    }
    
    return result;
}


static uint8 *FindEmptyElement(uint8 * const start)
{
    uint8 *element = start;
    
    volatile ShiftPoint::E *interpolated = randShift.points;
    interpolated = interpolated;

    while(element != end)
    {
        uint8 value = *element;
        
        int index = element - begin;
        
        if((value == VALUE::NONE) || randShift.Interpolated(index))   // Если очередной элемент "пустой"
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
    
    volatile ShiftPoint::E *interpolated = randShift.points;
    interpolated = interpolated;

    while(element != end)
    {
        uint8 value = *element;
        
        int index = element - begin;
        
        if(!randShift.Interpolated(index))
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

    for(int tick = 0; tick < numTicks - 1; tick++)
    {
        float value = before + delta * (tick + 1);

        *(segment->start + tick) = static_cast<uint8>(value + 0.5F);
    }
}
