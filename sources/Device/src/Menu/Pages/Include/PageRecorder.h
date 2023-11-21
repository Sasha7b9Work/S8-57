#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/DisplayRecorder.h"
#include "Recorder/Recorder.h"


#define S_REC_ENABLED_A         (set.rec._enA)
#define S_REC_ENABLED_B         (set.rec._enB)
#define S_REC_ENABLED_SENSOR    (set.rec._enSensor)
#define S_REC_CURSOR            (set.rec._cursor)
#define S_REC_CURSOR_IS_1       (S_REC_CURSOR == DisplayRecorder::Cursor::_1)
#define S_REC_CURSOR_IS_2       (S_REC_CURSOR == DisplayRecorder::Cursor::_2)
#define S_REC_INFO_IS_SHOWN     (set.rec._showInfo == 0)


struct SettingsRecorder
{ //-V802
    bool                       _enA;            // Включение записи канала 1
    bool                       _enB;            // Включение записи канала 2
    bool                       _enSensor;       // Включение записи показаний датчика
    Recorder::TypeMemory::E    _typeMemory;     // Место хранения записываемых/показываемых данных
    uint8                      _showInfo;       // Текущая ось перемещения
    Recorder::Axis::E          _axisZoom;       // Текущая ось масштабирования
    Recorder::ScaleX::E        _scaleX;         // Установленный масштаб по времени
    DisplayRecorder::Cursor::E _cursor;         // Курсор, которым сейчас можно управлять
};


namespace PageRecorder
{
    extern const Page *const self;

    namespace Source
    {
        extern const Page *const self;
    };

    namespace Show
    {
        extern const Page *const self;

        namespace CChoice
        {
            extern const Page *const self;
        };

        namespace Cursors
        {
            extern const Page *const self;
        };
    };
};
