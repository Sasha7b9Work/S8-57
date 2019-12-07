#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/Recorder.h"
#include "Settings/SettingsOsci.h"


struct SettingsRecorder
{ //-V802
    bool                    enA;            ///< Включение записи канала 1
    bool                    enB;            ///< Включение записи канала 2
    bool                    enSensor;       ///< Включение записи показаний датчика
    Recorder::TypeMemory::E typeMemory;     ///< Место хранения записываемых/показываемых данных
    Recorder::Axis::E       axisMove;       ///< Текущая ось перемещения
    Recorder::Axis::E       axisZoom;       ///< Текущая ось масштабирования
    Recorder::ScaleX::E     scaleX;         ///< Установленный масштаб по времени
    int8                    currentCursor;  ///< Курсор, которым сейчас можно управлять
};


struct PageRecorder
{
    static const Page *const self;

    struct PageSource
    {
        static const Page *const self;
    };

    struct PageShow
    {
        static const Page *const self;

        struct PageChoice
        {
            static const Page *const self;
        };

        struct PageCursors
        {
            static const Page *const self;
        };
    };
};
