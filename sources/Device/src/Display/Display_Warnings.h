#pragma once


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
