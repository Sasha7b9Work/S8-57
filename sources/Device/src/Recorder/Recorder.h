#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define RECORDER_VIEW_ALLWAYS   (set.rec_viewAlways)
#define RECORDER_SOURCE_A       (set.rec_sourceChanA)
#define RECORDER_SOURCE_B       (set.rec_sourceChanB)
#define RECORDER_SOURCE_SENSOR  (set.rec_sourceSensor)
#define RECORDER_STORAGE_RECORD (set.rec_storageRecord)
#define RECORDER_STORAGE_PLAY   (set.rec_storagePlay)


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Recorder
{
public:

    /// ѕоказывать ли всегда результаты измерений
    struct ViewAlways
    {
        enum E
        {
            Off,
            On
        } value;
    };

    /// C каким источником работаем
    struct DataStorage
    {
        enum E
        {
            PC,
            Flash
        } value;
    };

    static void Init();

    class Graphics
    {
    public:
        static void Update();
    };
};
