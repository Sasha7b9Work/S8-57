// 2023/11/22 21:14:11 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "defines.h"


#define PI 3.141592653589793f

#define LIMIT_BELOW(x, min)                 if((x) < (min)) { x = (min); }

#define LIMIT_ABOVE(x, max)                 if((x) > (max)) { x = (max); }

#define SET_MIN_IF_LESS(x, min)             if((x) < (min)) { (min) = (x); }

#define SET_MAX_IF_LARGER(x, max)           if((x) > (max)) { (max) = (x); }

#define LIMITATION_BOUND(var, min, max)     if((var) < (min)) { (min) = (var); }; if((var) > (max)) { (max) = (var); };

#define ROUND(x) ((x) += 0.5f, (x))

#define SET_IF_LARGER(x, max, newValue)     if((x) > (max)) { (x) = (newValue); }

#define LIMITATION_ABOVE(var, value, max)   var = (value); if((var) > (max)) { (var) = (max); }

#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); };

#define IN_RANGE(x, min, max)               ((x) >= (min) && (x) <= (max))


/// ���������� true, ���� value ������ � �������� [min; max]
bool IntInRange(int value, int min, int max);
/// ���������� ����� �������� ����, ������� �������
int LowSignedBit(uint value);
/// ���������� ������������ �������� �� ���
float MaxFloat(float val1, float val2, float val3);
/// \brief ���������� ��� �����. ���������� true, ���� ����� ���������� ���� �� ����� �� �����, ��� �� epsilonPart. ��� ���� ��� ������� epsilonPart 
/// ������������ ������� � ������ ������ �����.
bool FloatsIsEquals(float value0, float value1, float epsilonPart);

bool IsEquals(float x, float y);

float RandFloat(float min, float max);



namespace Math
{
    void Smoothing(uint8 *data, int numPoints, int numSmooth);

    int MinFrom2Int(int val0, int val1);

    uint8 MaxFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);

    uint8 MinFromArray_RAM(const uint16 *data, int firstPoint, int lastPoint);

    uint8 MaxFromArray(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MinFromArray(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MaxFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);

    uint8 MinFromArrayWithErrorCode(const uint8 *data, int firstPoint, int lastPoint);
    /// \brief ���������� ���������� x ����������� �����, ���������� ����� (x0, y0), (x1, y1), � �������������� ������, ���������� ����� ����� � 
    /// ��������� yHorLine.
    float GetIntersectionWithHorizontalLine(int x0, int y0, int x1, int y1, int yHorLine);

    uint8 CalculateFiltr(const uint8 *data, int x, int numPoints, int numSmoothing);

    void CalculateFiltrArray(const uint8 *dataIn, uint8 *dataOut, int numPoints, int numSmoothing);
    /// ����� ������ ������� �������, �� �������������� ���������
    int FindAnotherElement(const uint8 *data, uint8 value, int numElements);

    int DigitsInIntPart(float value);
    /// \brief ��������� ����� � ��������� ������. numDigits - ������ ����� ������, �� ������� ������������ ����������.
    /// ��������, RoundFloat(12.37137, 4) �������� �� 12.40)
    float RoundFloat(float value, int numDigits);
};


/// ���������� 10**pow.
int Pow10(int pow);

/// ���������� ������� ���������� ������ �� ������� value0 � value1
template<class T> void Swap(T *value0, T *value1) { T temp = *value0; *value0 = *value1; *value1 = temp; }

/// ��������� ���������� value0 � value1 � ������� �����������
template<class T> void Sort(T *value0, T *value1) { if (*value1 < *value0) { Swap<T>(value0, value1); } }

template<class T> int Sign(T x) { if (x > (T)(0)) { return 1; } if (x < (T)(0)) { return -1; } return 0; }

template<class T> T Abs(T x) { if (x < (T)0) { return -x; }  return x; }

template<class T> T Min(T x1, T x2) { return x1 < x2 ? x1 : x2; }

template<class T> T Max(T x1, T x2) { return x1 > x2 ? x1 : x2; }

template<class T> void CircleIncrease(T *value, T min, T max)
{
    if (*value < max) { ++(*value); }
    else { *value = (T)min; }
}

template<class T> void CircleDecrease(T *value, T min, T max)
{
    if (*value > min) { --(*value); }
    else { *value = (T)max; }
}
/// ���������������� *value, �� �� ������, ��� max
template<class T> static void LimitationIncrease(T *value, T max) { if ((*value) < max) { ++(*value); } }
/// ���������������� *value, �� �� ������, ��� min
template<class T> static void LimitationDecrease(T *value, T min) { if (*value > min) { --(*value); } }

template<class T> static void Limitation(T *value, T min, T max)
{
    if (*value < min) { *value = min; }
    else if (*value > max) { *value = max; }
    else
    {
        // ����� ������
    }
}

template<class T> static T LimitationRet(T value, T min, T max)
{
    if (value < min) { return min; }
    if (value > max) { return max; }
    return value;
}
/// ��������� � *value term � ����������, ���� ��������� ������� �� ������� [min, max]
template<class T> static void AddtionThisLimitation(T *value, int term, T min, T max)
{
    if (term > 0) {
        if (max - term >= *value) { *value += (T)term; }
        else { *value = max; }
    }
    else {
        if (min - term <= *value) { *value += (T)term; }
        else { *value = min; }
    }
}

#define _bitset(bits)                               \
  ((uint8)(                                         \
  (((uint8)((uint)bits / 01)        % 010) << 0) |  \
  (((uint8)((uint)bits / 010)       % 010) << 1) |  \
  (((uint8)((uint)bits / 0100)      % 010) << 2) |  \
  (((uint8)((uint)bits / 01000)     % 010) << 3) |  \
  (((uint8)((uint)bits / 010000)    % 010) << 4) |  \
  (((uint8)((uint)bits / 0100000)   % 010) << 5) |  \
  (((uint8)((uint)bits / 01000000)  % 010) << 6) |  \
  (((uint8)((uint)bits / 010000000) % 010) << 7)))


#define BIN_U8(bits) (_bitset(0##bits))

#define BIN_U32(bits3, bits2, bits1, bits0) (BIN_U8(bits3) << 24 | BIN_U8(bits2) << 16 | BIN_U8(bits1) << 8 | BIN_U8(bits0))

#define HEX_FROM_2(hex1, hex0) ((uint)(0x##hex1) << 16 | (uint)0x##hex0)
