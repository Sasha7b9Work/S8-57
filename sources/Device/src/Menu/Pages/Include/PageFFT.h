#pragma once


#define S_FFT_SCALE     (set.fft.scale)
#define S_FFT_SCALE_IS_LOG  (S_FFT_SCALE == ScaleFFT::Log)
struct ScaleFFT
{
    enum E
    {
        Log,           // Это значение означает логарифмическую шкалу вывода спектра.
        Linear         // Это значение означает линейную шкалу вывода спектра.
    };
};


struct SourceFFT
{
    enum E
    {
        A,
        B,
        Both
    };
};


struct WindowFFT
{
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


#define S_FFT_ENABLED       (set.fft._enabled)

#define S_FFT_POS_CUR(num)  (set.fft._posCur[num])
#define S_FFT_POS_CUR_0     (S_FFT_POS_CUR(0))
#define S_FFT_POS_CUR_1     (S_FFT_POS_CUR(1))


struct SettingsFFT
{ //-V802
    bool            _enabled;
    uint8           _posCur[2];      // Позиция курсора спектра. Изменяется 0...256.
    ScaleFFT::E     scale;
    SourceFFT::E    source;
    WindowFFT::E    window;
    MaxDBFFT::E     maxDB;
    uint8           cursor;         // Определяет, каким курсором спектра управляет ручка УСТАНОВКА.
};


struct PageFFT
{
    static const Page *const self;

    struct Cursors
    {
        static const Page *const self;
    };
};
