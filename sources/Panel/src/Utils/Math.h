#pragma once
#include "defines.h"



#define LIMITATION(var, min, max)           if(var < (min)) { (var) = (min); } else if(var > (max)) { var = (max); };

class Math
{
public:
    // Возвращает максимальное значение из трёх
    template<class T> static T    Max(T val1, T val2, T val3);
    
    template<class T> static bool IsEquals(T x, T y);
    // Обменивает местами содержимое памяти по адресам value0 и value1
    template<class T> static void Swap(T *value0, T *value1);
    
    template<class T> static int  Sign(T x);
    
    template<class T> static void Limitation(T *value, T min, T max);
};
