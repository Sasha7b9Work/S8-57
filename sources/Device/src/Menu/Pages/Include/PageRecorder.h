#pragma once
#include "Menu/MenuItems.h"
#include "Osci/Osci_Settings.h"


/// Где хранятся данные
struct RecorderTypeStorage
{
    enum E
    {
        RAM,    ///< ОЗУ
        ROM,    ///< Внутр ЗУ
        EXT     ///< Внешн ЗУ
    } value;
    explicit RecorderTypeStorage(E v) : value(v) {};
};

/// Используется при выборе оси
struct RecorderAxis
{
    enum E
    {
        X,
        Y
    } value;
    explicit RecorderAxis(E v) : value(v) {};
};

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
        Size
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
    bool                    enabledChanA;   ///< Включение записи канала 1
    bool                    enabledChanB;   ///< Включение записи канала 2
    bool                    enabledSensor;  ///< Включение записи показаний датчика
    RecorderTypeStorage::E  storageRecord;  ///< Куда сохранять данные
    RecorderTypeStorage::E  storagePlay;    ///< Откуда брать сохранённые данные
    RecorderAxis::E         axisMove;       ///< Текущая ось перемещения
    RecorderAxis::E         axisZoom;       ///< Текущая ось масштабирования
    RecorderScaleX          scaleX;         ///< Установленный масштаб по времени
    int8                    currentCursor;
    Range::E                unused[2];      ///< Масштаб по вертикали в режиме регистратора
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
struct PageRecorder
{
    static const Page *const self;

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageSource
    {
        static const Page *const self;
    };

    //---------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
    struct PageShow
    {
        static const Page *const self;

        //-----------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
        struct PageOperations
        {
            static const Page *const self;
        };

        struct PageView
        {
            static const Page *const self;

            //-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
            struct PageCursors
            {
                static const Page *const self;
            };
        };
    };
};
