#pragma once
#include "Menu/MenuItems.h"
#include "Recorder/Recorder.h"
#include "Settings/SettingsOsci.h"


/// Масшаб по времени
struct RecorderScaleX
{
    enum E
    {
        _100ms, // TBase = 0.1c
        _200ms, // TBase = 0.2c
        _500ms, // TBase = 0.5c
        _1s,    // TBase = 1c
        _2s,    // TBase = 2c
        _5s,    // TBase = 5c
        _10s,   // TBase = 10c
        Count
    } value;
    explicit RecorderScaleX(E v) : value(v) {};
    static void Load();
    static void Change(int delta);
    static RecorderScaleX &Current();
    pString ToString() const;
    /// Возвращает скорость потребления памяти в байтах/сек
    uint BytesToSec() const;
    /// Возвращает время считывания одной точки
    uint TimeForPointMS() const;
};


struct SettingsRecorder
{ //-V802
    bool                    enA;            ///< Включение записи канала 1
    bool                    enB;            ///< Включение записи канала 2
    bool                    enSensor;       ///< Включение записи показаний датчика
    Recorder::TypeMemory::E typeMemory;     ///< Место хранения записываемых/показываемых данных
    Recorder::Axis::E       axisMove;       ///< Текущая ось перемещения
    Recorder::Axis::E       axisZoom;       ///< Текущая ось масштабирования
    RecorderScaleX          scaleX;         ///< Установленный масштаб по времени
    int8                    currentCursor;  ///< Курсор, которым сейчас можно управлять
    Range::E                unused[2];      ///< Масштаб по вертикали в режиме регистратора
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
