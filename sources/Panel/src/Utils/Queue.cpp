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
        delete[] pointer;
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
        pointer = new T[1];
        *pointer = elem;
        iFront = 0;
        iBack = 1;
    }
    else
    {
        T *old = pointer;
        int num = iBack - iFront + 1;
        pointer = new T[static_cast<uint>(num)];
        for (int i = 0; i < num - 1; i++)
        {
            pointer[i] = old[i + iFront];
        }       
        pointer[num - 1] = elem;
        iFront = 0;
        iBack = num;
        delete[] old;
    }
}


template<typename T>
T Queue<T>::Front() //-V2506
{
    if (pointer != nullptr)
    {
        T result = pointer[iFront];
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
        result = pointer[iBack - 1];
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
            return pointer[index];
        }
    }

	static T result(0);

    return result;
}
