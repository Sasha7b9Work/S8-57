#pragma once
#include "Display_Types.h"
#include "Keyboard/Keyboard.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class LogEntity
{
    friend class Log;
};

#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Cообщения, выводимые на экран
struct Warning
{
    enum E
    {
        LimitChan1_Volts,               ///< Предел масштаба по напряжениию первого канала.
        LimitChan2_Volts,               ///< Предел масштаба по напряжению второго канала.
        LimitSweep_Time,                ///< Предел масштаба по времени.
        EnabledPeakDet,                 ///< Включён пиковый детектор.
        LimitChan1_RShift,              ///< Предел смещения по напряжению первого канала.
        LimitChan2_RShift,              ///< Предел смещения по напряжению второго канала.
        LimitSweep_Level,               ///< Предел уровня синхронизации.
        LimitSweep_TShift,              ///< Предел смещения по времени.
        TooSmallSweepForPeakDet,        ///< Слишком быстрая развёртка для пикового детектора.
        TooFastScanForRecorder,         ///< Слишком быстрая развёртка для поточечного вывода.
        FileIsSaved,                    ///< Файл сохранён на флешку.
        SignalIsSaved,                  ///< Сигнал сохранён в ППЗ.
        SignalIsDeleted,                ///< Из ППЗУ удалён сигнал.
        MenuDebugEnabled,               ///< Включено меню ОТЛАДКА.
        TimeNotSet,                     ///< Время не установлено.
        SignalNotFound,                 ///< Сигнал не найден (автопоиск).
        SetTPosToLeft,                  ///< Рекомендация.
        NeedRebootDevice,               ///< Требуется перезагрузка.
        ImpossibleEnableMathFunction,   ///< Невозможно включить математическую функцию.
        ImpossibleEnableFFT,            ///< Невозможно включить вычисление БПФ.
        WrongFileSystem,                ///< Невозможно подключить флешку.
        WrongModePeackDet,              ///< Невозможно включить пиковый детектор.
        DisableChannelB,                ///< Требуется выключить канал 2.
        TooLongMemory,                  ///< Сообщение о невозможности включения режима 32к.
        NoPeakDet32k,                   ///< Пиковый детектор не работает при длине памяти 32к.
        NoPeakDet16k,                   ///< Пиковый детектор не работает при длине памяти 16к.
        Warn50Ohms,                     ///< Предупреждение об ограничение входного уровня на 50 Омах.
        WarnNeedForFlashDrive,          ///< Требуется подключенная флешка.
        FirmwareSaved,                  ///< Прошивка сохранена на флешку.
        FullyCompletedOTP,              ///< 
        Number
    } value;
};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace Display
{
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    void Init();

    void Update();

    //static void SetKey(Key::E key);

    void RemoveAddDrawFunction();

    void ShowWarning(Warning::E warning);
    /// @brief Установить функцию и режим отрисовки экрана.
    /// @details Возможны три варианта.
    /// 1. DrawMode_Hand - в этом случае будет вызываться функция func(), определяемая пользователем, с частотой 25 раз в секунду.
    /// Используется в случаях, когда не выполняется главный цикл.
    /// 2. DrawMode::Auto и func == 0 - в этом случае будет выполняться функция Update() в главном цикле.
    /// 3. DrawMode::Auto и func != 0 - в этом случае будет выполняться функция func(), определяемая пользователем, но в теле
    /// главного цикла, будучи вызываемой из Update() вместо Update().
    void SetDrawMode(DrawMode::E mode, pFuncVV func);

    void FuncOnWaitStart(const char *text, bool eraseBackground);

    void FuncOnWaitStop();

    void SetAddDrawFunction(pFuncVV func, uint time = MAX_UINT);

    void ChangedRShiftMarkers(bool active);

    //static void Clear();

    extern Key::E key;

    void SetOrientation(Orientation orientation);
    /// Возвращает режим усреднения
    //static ModeAveraging::E GetModeAveraging();
    /// Устанавливает ограничение частоты кадров
    static void SetNumSignalsInS(int maxFPS);
    /// Возвращает число точек сглаживающего фильтра (режим ДИСПЛЕЙ - Сглаживание)
    //static int NumPointSmoothing();
    /// Возвращает адрес первой и последней точки на экране в координатах экрана
    BitSet64 PointsOnDisplay();
    /// \brief Возращает адрес первой и последней точки в координатах экрана
    BitSet64 BytesOnDisplay();
    /// Возвращает время, через которое меню автоматически скрывается, если не было больше нажатий
    int TimeMenuAutoHide();
    /// Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    bool IsSeparate();
    /// brightness = 1..100
    static void SetBrightness(int16 brightness);

    //static int NumAverage();
    /// Это смещение экрана по памяти в режиме пикового детектора оно будет в два раза меньше, чем байт, потому что каждая точка представлена двумя байтами
    //static int ShiftInMemoryInPoints();
};
