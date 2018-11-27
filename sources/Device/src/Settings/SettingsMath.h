#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsMath Settings Math
 *  @{
 */

struct FuncModeDraw
{
    enum E
    {
        Disable,
        Separate,
        Together
    } value;
    explicit FuncModeDraw(E v) : value(v) {};
};

struct ModeRegSet
{
    enum E
    {
        Range,
        RShift
    } value;
    explicit ModeRegSet(E v) : value(v) {};
};

struct Function
{
    enum E
    {
        Sum,
        Mul
    } value;
    explicit Function(E v) : value(v) {};
};

struct WindowFFT
{
    enum E
    {
        Rectangle,
        Hamming,
        Blackman,
        Hann
    } value;
    explicit WindowFFT(E v) : value(v) {};
};

struct SourceFFT
{
    enum E
    {
        A,
        B,
        Both
    } value;
    explicit SourceFFT(E v) : value(v) {};
};

struct ScaleFFT
{
    enum E
    {
        Log,           ///< Это значение означает логарифмическую шкалу вывода спектра.
        Linear         ///< Это значение означает линейную шкалу вывода спектра.
    } value;
    explicit ScaleFFT(E v) : value(v) {};
};

struct FFTmaxDB
{
    enum E
    {
        _40,
        _60,
        _80
    } value;
    explicit FFTmaxDB(E v) : value(v) {};
};

#define SOURCE_FFT                  (set.math_sourceFFT)
#define SOURCE_FFT_A                (SOURCE_FFT == SourceFFT_ChannelA)
#define SOURCE_FFT_B                (SOURCE_FFT == SourceFFT_ChannelB)

#define WINDOW_FFT                  (set.math_windowFFT)
#define WINDOW_FFT_IS_HAMMING       (WINDOW_FFT == WindowFFT::Hamming)
#define WINDOW_FFT_IS_BLACKMAN      (WINDOW_FFT == WindowFFT::Blackman)
#define WINDOW_FFT_IS_HANN          (WINDOW_FFT == WindowFFT::Hann)
#define MAX_DB_FFT                  (set.math_FFTmaxDB)
#define MAX_DB_FOR_FFT              MaxDBforFFT(MAX_DB_FFT)
#define FFT_ENABLED                 (set.math_enableFFT)
#define MATH_ENABLED                (FUNC_MODE_DRAW_IS_ENABLED || FFT_ENABLED)

#define SCALE_FFT                   (set.math_scaleFFT)
#define SCALE_FFT_IS_LOG            (SCALE_FFT == ScaleFFT::Log)
#define SCALE_FFT_IS_LINEAR         (SCALE_FFT == ScaleFFT::Linear)

#define MATH_FUNC                   (set.math_function)
#define MATH_FUNC_IS_MUL            (MATH_FUNC == Function::Mul)
#define MATH_FUNC_IS_SUM            (MATH_FUNC == Function::Sum)

#define FUNC_MODE_DRAW              (set.math_modeDraw)
#define FUNC_MODE_DRAW_IS_ENABLED   (FUNC_MODE_DRAW != FuncModeDraw::Disable)
#define FUNC_MODE_DRAW_IS_SEPARATE  (FUNC_MODE_DRAW == FuncModeDraw::Separate)
#define FUNC_MODE_DRAW_IS_TOGETHER  (FUNC_MODE_DRAW == FuncModeDraw::Together)

#define FFT_POS_CURSOR(num)         (set.math_posCur[num])
#define FFT_POS_CURSOR_0            (FFT_POS_CURSOR(0))
#define FFT_POS_CURSOR_1            (FFT_POS_CURSOR(1))

#define MATH_CURRENT_CUR            (set.math_currentCursor)
#define MATH_CURRENT_CUR_IS_0       (MATH_CURRENT_CUR == 0)

#define MATH_DIVIDER                (set.math_divider)

#define MATH_MODE_REG_SET           (set.math_modeRegSet)
#define MATH_MODE_REG_SET_IS_RSHIFT (MATH_MODE_REG_SET == ModeRegSet::RShift)
#define MATH_MODE_REG_SET_IS_RANGE  (MATH_MODE_REG_SET == ModeRegSet::Range)

float MaxDBforFFT(FFTmaxDB::E maxDB);

/** @}  @}
 */
