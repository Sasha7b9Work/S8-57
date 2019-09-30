#pragma once



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



namespace Math
{
    /// ¬озвращает номер младшего бита, равного единице
    int LowSignedBit(uint value);
    /// \brief —равнивает два числа. ¬озвращает true, если числа отличаютс€ друг от друга не более, чем на epsilonPart. ѕри этом дл€ расчЄта epsilonPart 
    /// используетс€ большее в смысле модул€ число.
    bool FloatsIsEquals(float value0, float value1, float epsilonPart);

    bool IsEquals(float x, float y);

    void Smoothing(uint8 *data, int numPoints, int numSmooth);
    
    int MinFrom2Int(int val0, int val1);

    uint8 MaxFromArray(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MinFromArray(const uint8 *data, int firstPoint, int lastPoint);

    void MinMaxFromArray(const uint8 *data, int firstPoint, int lastPoint, uint8 *outMin, uint8 *outMax);

    uint8 MaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
    /// \brief ¬озвращает координату x пересечени€ линии, проход€щей через (x0, y0), (x1, y1), с горизонтальной линией, проход€щей через точку с 
    /// ординатой yHorLine.
    float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);
    
    void CalculateMathFunction(float *dataAandResult, const float *dataB, int numPoints);

    uint8 CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);

    void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
    /// Ќайти первый элемент массива, не соотвествующий заданному
    int FindAnotherElement(const uint8 *data, uint8 value, int numElements);
    /// ¬озвращает случайное число из диапазона [min; max]
    float RandFloat(float min, float max);

    int DigitsInIntPart(float value);
    /// \brief ќкругл€ет число с плавающей точкой. numDigits - полное число знаков, по которым производитс€ округление.
    /// Ќапример, RoundFloat(12.37137, 4) округлит до 12.40)
    float RoundFloat(float value, int numDigits);
    /// ¬ычисление 10**pow.
    int Pow10(int pow);
    /// ќбменивает местами содержимое пам€ти по адресам value0 и value1
    template<class T> void Swap(T *value0, T *value1);
    /// –азмещает переменные value0 и value1 в пор€дке возрастани€
    template<class T> void Sort(T *value0, T *value1);

    template<class T> int  Sign(T x);

    template<class T> T    Abs(T x);

    template<class T> T    Min(T x1, T x2);

    template<class T> T    Max(T x1, T x2);
    /// ¬озвращает максимальное значение из трЄх
    template<class T> T    Max(T val1, T val2, T val3);

    template<class T> void CircleIncrease(T *value, T min, T max);

    template<class T> void CircleDecrease(T *value, T min, T max);
    /// »нкрементировать *value, но не больше, чем max
    template<class T> void LimitationIncrease(T *value, T max);
    /// ƒекрементировать *value, но не меньше, чем min
    template<class T> void LimitationDecrease(T *value, T min);

    template<class T> void Limitation(T *value, T min, T max);

    template<class T> T    LimitationRet(T value, T min, T max);
    /// ѕрибавить к *value term и ограничить, если результат выходит за границы [min, max]
    template<class T> void AdditionThisLimitation(T *value, int term, T min, T max);
};
