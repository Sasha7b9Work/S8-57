#include "defines.h"
#include "log.h"
#include "FPGA/FPGA.h"
#include "Hardware/Memory/IntRAM.h"
#include "Hardware/Timer.h"
#include "Osci/DeviceSettings.h"
#include "Osci/Osci.h"
#include "Osci/Math/OsciMath.h"


static uint8 *begin = nullptr;  // Начало обрабатываемых данных
static uint8 *end = nullptr;    // Конец обрабатываемых данных


// Структура описывает отрезок
struct Segment
{
    uint8 *start;   // Указатель на первый элемент отрезка
    uint8 *end;     // Указатель на первый элемент за отрезком
};


// Интерполировать данные одного канала
static void InterpolateChannel(uint8 *data, int size);

// Находит первый пустой сегмент (т.е. в котором находятся толдько "пустые" элементы), начиная с элемента с адресом start
static bool FindEmptySegment(uint8 *start, Segment *segment);

// Интерполировать данные в сегменте
static void InterpolateSegment(Segment *segment);

// Находим адрес пустого элемента. Возвращает end, если такого элемента нет
static uint8 *FindEmptyElement(uint8 * const start);

// Найти первый "непустой" элемент данных. Возвращает end, если такого элемента нет
static uint8 *FindReadedElement(uint8 * const start);


static bool FindEmptySegment(uint8 *start, Segment *segment)
{
    segment->start = FindEmptyElement(start);

    segment->end = FindReadedElement(segment->start);

    return (segment->start != end) && (segment->end != end);
}


static uint8 *FindEmptyElement(uint8 * const start)
{
    uint8 *element = start;

    uint8 *interpol = IntRAM::DataRand(ChanA) + (start - begin);

    while(element != end)
    {
        if(*interpol == VALUE::NONE || *element == VALUE::NONE)
        {
            break;
        }
        element++;
        interpol++;
    }

    return element;
}


static uint8 *FindReadedElement(uint8 * const start)
{
    uint8 *element = start;
    uint8 *interpol = IntRAM::DataRand(ChanA) + (start - begin);
    
    while(element != end)
    {
        if(*element != VALUE::NONE && *interpol != VALUE::NONE)
        {
            break;
        }
        element++;
        interpol++;
    }

    return element;
}


static void InterpolateSegment(Segment *segment)
{
    if((segment->start == end) ||
       (segment->start == begin) ||
       (segment->end == end))
    {
        return;
    }

    float before = *(segment->start - 1);               // Значение перед "пустым" сегментом
    float after = *(segment->end);                      // Значение после "пустого" сегмента

    int numTicks = segment->end - segment->start + 1;   // На столько отрезков разбит наш сегмент

    float delta = (after - before) / numTicks;

    for(int tick = 0; tick < numTicks - 1; tick++)
    {
        float value = before + delta * (tick + 1);

        *(segment->start + tick) = static_cast<uint8>(value + 0.5F);
    }
}


void InterpolatorLinear::Run(DataSettings *ds)
{
    if(ds->enableA)
    {
        InterpolateChannel(ds->dataA, ds->BytesInChannel());
    }
    if(ds->enableB)
    {
        InterpolateChannel(ds->dataB, ds->BytesInChannel());
    }
}


static void InterpolateChannel(uint8 *data, int size)
{
    int readed = 0;                                     // Число реально считанных точек
    uint8 *pointer = data;
    uint8 *last = pointer + size;
    int halfSize = size / 2;

    while(pointer < last)
    {
        if(*pointer++ != VALUE::NONE)
        {
            readed++;
        }

        if(readed > halfSize)
        {
            begin = data;
            end = begin + size;

            Segment segment;

            segment.end = begin;

            while(FindEmptySegment(segment.end, &segment))
            {
                InterpolateSegment(&segment);
            }

            break;
        }
    }
}
