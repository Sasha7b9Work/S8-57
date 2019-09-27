#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define REC_VIEW_ALLWAYS                (set.rec_viewAlways)
#define REC_SRC_A                       (set.rec_sourceChanA)
#define REC_SRC_A_IS_ENABLED            (REC_SRC_A)
#define REC_SRC_A_IS_DISABLED           (!(REC_SRC_A_IS_ENABLED))
#define REC_SRC_B                       (set.rec_sourceChanB)
#define REC_SRC_B_IS_ENABLED            (REC_SRC_B)
#define REC_SRC_B_IS_DISABLED           (!(REC_SRC_B_IS_ENABLED))
#define REC_SRC_SENSOR                  (set.rec_sourceSensor)
#define REC_SRC_SENSOR_IS_ENABLED       (REC_SRC_SENSOR)
#define REC_SOURCE_SENSOR_IS_DISABLED   (!(REC_SRC_SENSOR_IS_ENABLED))
#define REC_STORAGE_RECORD              (set.rec_storageRecord)
#define REC_STORAGE_PLAY                (set.rec_storagePlay)
#define REC_AXIS_MOVE                   (set.rec_axisMove)
#define REC_AXIS_ZOOM                   (set.rec_axisZoom)

#define REC_SCALE_X                     (Recorder::Settings::ScaleX::Current())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Recorder
{
    namespace Settings
    {
        /// Где хранятся данные
        struct TypeStorage
        {
            enum E
            {
                RAM,    ///< ОЗУ
                ROM,    ///< Внутр ЗУ
                EXT     ///< Внешн ЗУ
            } value;
            explicit TypeStorage(E v) : value(v) {};
        };

        /// Используется при выборе оси
        struct Axis
        {
            enum E
            {
                X,
                Y
            } value;
            explicit Axis(E v) : value(v) {};
        };

        /// Масшаб по времени
        struct ScaleX
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
            explicit ScaleX(E v) : value(v) {};
            static void Load();
            static void Change(int delta);
            static ScaleX &Current();
            pString ToString() const;
            /// Возвращает скорость потребления памяти в байтах/сек
            uint BytesToSec() const;
            /// Возвращает время считывания одной точки
            uint TimeForPointMS() const;
        };

        extern int8 currentCursor;
    };
}

