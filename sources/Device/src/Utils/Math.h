#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define PI 3.141592653589793F

#define LIMIT_BELOW(x, min)                 if((x) < (min)) { x = (min); }

#define LIMIT_ABOVE(x, max)                 if((x) > (max)) { x = (max); }

#define SET_MIN_IF_LESS(x, min)             if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)           if((x) > (max)) { (max) = (x); }

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };
/// \todo Ёто выражение неправильно работает с отрицательными числами
#define ROUND(type, x) ((type)((x) + 0.5F))

#define SET_IF_LARGER(x, max, newValue)     if((x) > (max)) { (x) = (newValue); }

#define LIMITATION_ABOVE(var, value, max)   var = (value); if((var) > (max)) { (var) = (max); }

#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); };

#define IN_RANGE(x, min, max)               ((x) >= (min) && (x) <= (max))


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Math
{
public:
    /// ¬озвращает номер младшего бита, равного единице
    static int LowSignedBit(uint value);
    /// \brief —равнивает два числа. ¬озвращает true, если числа отличаютс€ друг от друга не более, чем на epsilonPart. ѕри этом дл€ расчЄта epsilonPart 
    /// используетс€ большее в смысле модул€ число.
    static bool FloatsIsEquals(float value0, float value1, float epsilonPart);

    static bool IsEquals(float x, float y);

    static void Smoothing(uint8 *data, int numPoints, int numSmooth);
    
    static int MinFrom2Int(int val0, int val1);

    static uint8 MaxFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);

    static uint8 MinFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);

    static uint8 MaxFromArray(const uint8 *data, int firstPoint, int lastPoint);

    static uint8 MinFromArray(const uint8 *data, int firstPoint, int lastPoint);

    static uint8 MaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);

    static uint8 MinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
    /// \brief ¬озвращает координату x пересечени€ линии, проход€щей через (x0, y0), (x1, y1), с горизонтальной линией, проход€щей через точку с 
    /// ординатой yHorLine.
    static float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
    
    static void CalculateMathFunction(float *dataAandResult, float *dataB, int numPoints);

    static uint8 CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);

    static void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
    /// Ќайти первый элемент массива, не соотвествующий заданному
    static int FindAnotherElement(uint8 *data, uint8 value, int numElements);
    /// ¬озвращает случайное число из диапазона [min; max]
    static float RandFloat(float min, float max);

    static int DigitsInIntPart(float value);
    /// \brief ќкругл€ет число с плавающей точкой. numDigits - полное число знаков, по которым производитс€ округление.
    /// Ќапример, RoundFloat(12.37137, 4) округлит до 12.40)
    static float RoundFloat(float value, int numDigits);
    /// ¬ычисление 10**pow.
    static int Pow10(int pow);
    /// ќбменивает местами содержимое пам€ти по адресам value0 и value1
    template<class T> static void Swap(T *value0, T *value1);
    /// –азмещает переменные value0 и value1 в пор€дке возрастани€
    template<class T> static void Sort(T *value0, T *value1);

    template<class T> static int  Sign(T x);

    template<class T> static T    Abs(T x);

    template<class T> static T    Min(T x1, T x2);

    template<class T> static T    Max(T x1, T x2);
    /// ¬озвращает максимальное значение из трЄх
    template<class T> static T    Max(T val1, T val2, T val3);

    template<class T> static void CircleIncrease(T *value, T min, T max);

    template<class T> static void CircleDecrease(T *value, T min, T max);
    /// »нкрементировать *value, но не больше, чем max
    template<class T> static void LimitationIncrease(T *value, T max);
    /// ƒекрементировать *value, но не меньше, чем min
    template<class T> static void LimitationDecrease(T *value, T min);

    template<class T> static void Limitation(T *value, T min, T max);

    template<class T> static T    LimitationRet(T value, T min, T max);
    /// ѕрибавить к *value term и ограничить, если результат выходит за границы [min, max]
    template<class T> static void AdditionThisLimitation(T *value, int term, T min, T max);
};
