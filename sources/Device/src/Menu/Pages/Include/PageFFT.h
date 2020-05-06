#pragma once


struct ScaleFFT
{
#define S_FFT_SCALE         (set.fft._scale)
#define S_FFT_SCALE_IS_LOG  (S_FFT_SCALE == ScaleFFT::Log)

    enum E
    {
        Log,           // Это значение означает логарифмическую шкалу вывода спектра.
        Linear         // Это значение означает линейную шкалу вывода спектра.
    };
};


struct SourceFFT
{
#define S_FFT_SOURCE        (set.fft._source)
#define S_FFT_SOURCE_IS_A   (S_FFT_SOURCE == SourceFFT::A)
#define S_FFT_SOURCE_IS_B   (S_FFT_SOURCE == SourceFFT::B)

    enum E
    {
        A,
        B,
        Both
    };
};


struct WindowFFT
{
#define S_FFT_WINDOW                (set.fft._window)
#define S_FFT_WINDOW_IS_HAMMING     (S_FFT_WINDOW == WindowFFT::Hamming)
#define S_FFT_WINDOW_IS_BLACKMAN    (S_FFT_WINDOW == WindowFFT::Blackman)
#define S_FFT_WINDOW_IS_HANN        (S_FFT_WINDOW == WindowFFT::Hann)

    enum E
    {
        Rectangle,
        Hamming,
        Blackman,
        Hann
    };
};


struct MaxDBFFT
{
#define S_FFT_MAX_DB    (set.fft._maxDB)

    enum E
    {
        _40,
        _60,
        _80
    };

    static float MaxDBforFFT(MaxDBFFT::E maxDB)
    {
        static const float arrayMAX_DB_FOR_FFT[] = { -40.0F, -60.0F, -80.0F };

        return arrayMAX_DB_FOR_FFT[maxDB];
    }
};


#define S_FFT_ENABLED               (set.fft._enabled)
#define S_FFT_ACTIVE_CURSOR         (set.fft._cursor)
#define S_FFT_ACTIVE_CURSOR_IS_0    (S_FFT_ACTIVE_CURSOR == 0)

#define S_FFT_POS_CUR(num)      (set.fft._posCur[num])
#define S_FFT_POS_CUR_0         (S_FFT_POS_CUR(0))
#define S_FFT_POS_CUR_1         (S_FFT_POS_CUR(1))
#define S_FFT_POS_ACTIVE_CURSOR (S_FFT_POS_CUR(S_FFT_ACTIVE_CURSOR))


struct SettingsFFT
{ //-V802
    bool            _enabled;
    uint8           _posCur[2];      // Позиция курсора спектра. Изменяется 0...256.
    ScaleFFT::E     _scale;
    SourceFFT::E    _source;
    WindowFFT::E    _window;
    MaxDBFFT::E     _maxDB;
    uint8           _cursor;         // Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
};


struct PageFFT
{
    static const Page *const self;

    struct Cursors
    {
        static const Page *const self;
    };
};
