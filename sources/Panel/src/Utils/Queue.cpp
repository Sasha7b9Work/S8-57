#include "defines.h"
#include "Utils/Queue.h"


template class Queue<uint8>;


template<typename T>
Queue<T>::Queue() : pointer(nullptr), iFront(0), iBack(0)
{

}


template<typename T>
Queue<T>::~Queue()
{
    Destroy();
}


template<typename T>
void Queue<T>::Destroy()
{
    if (pointer != nullptr)
    {
        delete[] pointer; //-V2511
        pointer = nullptr;
        iFront = 0;
        iBack = 0;
    }
}


template<typename T>
void Queue<T>::Push(T elem)
{
    if (pointer == nullptr)
    {
        pointer = new T[1]; //-V2511
        *pointer = elem;
        iFront = 0;
        iBack = 1;
    }
    else
    {
        T *old = pointer;
        int num = iBack - iFront + 1;
        pointer = new T[static_cast<uint>(num)]; //-V2511
        for (int i = 0; i < num - 1; i++)
        {
            pointer[i] = old[i + iFront]; //-V2563
        }       
        pointer[num - 1] = elem;
        iFront = 0;
        iBack = num;
        delete[] old; //-V2511
    }
}


template<typename T>
T Queue<T>::Front() //-V2506
{
    if (pointer != nullptr)
    {
        T result = pointer[iFront]; //-V2563
        iFront++;
        if (iFront == iBack)
        {
            Destroy();
        }

        return result;
    }

    return T(0);
}


template<typename T>
T Queue<T>::Back()
{
    T result(0);

    if (pointer != nullptr)
    {
        result = pointer[iBack - 1]; //-V2563
        iBack--;
        if (iFront == iBack)
        {
            Destroy();
        }
    }

    return result;
}


template<typename T>
int Queue<T>::Size() const
{
    return (iBack - iFront);
}


template<typename T>
bool Queue<T>::IsEmpty() const
{
    return (Size() == 0);
}


template<typename T>
T &Queue<T>::operator[](int n) //-V2506
{
    if (pointer != nullptr)
    {
        int index = iFront + n;
        if (index >= iFront && index < iBack)
        {
            return pointer[index]; //-V2563
        }
    }

	static T result(0);

    return result;
}
