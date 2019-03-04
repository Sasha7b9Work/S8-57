#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RECORDER_VIEW_ALLWAYS           (set.rec_viewAlways)
#define RECORDER_SOURCE_A               (set.rec_sourceChanA)
#define RECORDER_SOURCE_A_IS_ENABLED    (RECORDER_SOURCE_A == true)
#define RECORDER_SOURCE_B               (set.rec_sourceChanB)
#define RECORDER_SOURCE_B_IS_ENABLED    (RECORDER_SOURCE_B == true)
#define RECORDER_SOURCE_SENSOR          (set.rec_sourceSensor)
#define RECORDER_STORAGE_RECORD         (set.rec_storageRecord)
#define RECORDER_STORAGE_PLAY           (set.rec_storagePlay)
#define RECORDER_AXIS_MOVE              (set.rec_axisMove)
#define RECORDER_AXIS_ZOOM              (set.rec_axisZoom)


#define RECORDER_SCALE_X        (Recorder::Settings::ScaleX::Current())


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Recorder
{
    namespace Settings
    {
        /// Показывать ли всегда результаты измерений
        struct ViewAlways
        {
            enum E
            {
                InMode,
                InAlways
            } value;
            explicit ViewAlways(E v) : value(v) {};
        };

        /// C каким источником работаем
        struct TypeStorage
        {
            enum E
            {
                RAM,
                Flash,
                PC
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
        };
    };
}

