#include "defines.h"
#include "Utils/Averager.h"
#include <cstdlib>



//template class Utils::Averager<float>;
template class Utils::AroundAverager<float>;



template <typename T>
Utils::Averager<T>::Averager(uint _size) : buffer(nullptr), size(_size), numELements(0U)
{
    buffer = (T *)std::malloc(sizeof(T) * _size);

    if (buffer == nullptr)
    {
        LOG_ERROR("Недостаточно памяти");
    }
}


template <typename T>
Utils::Averager<T>::~Averager()
{
    std::free(buffer);
}


template <typename T>
void Utils::Averager<T>::Push(T elem)
{
    if (numELements < size)
    {
        buffer[numELements] = elem;
        numELements++;
    }
    else
    {
        for (uint i = 1; i < size; i++)
        {
            buffer[i - 1] = buffer[i];
        }
        buffer[size - 1] = elem;
    }
}


template <typename T>
T Utils::Averager<T>::Value() const
{
    T result = (T)0;

    for (uint i = 0; i < numELements; i++)
    {
        result += buffer[i];
    }

    return result / numELements;
}


template <typename T>
Utils::AroundAverager<T>::AroundAverager(uint _parts) : value(0), parts(_parts), pushed(0)
{

}


template <typename T>
void Utils::AroundAverager<T>::Push(T elem)
{
    pushed++;

    if (pushed < parts)
    {
        value += elem;
    }
    else if (pushed == parts)
    {
        value += elem;
        value /= pushed;
    }
    else
    {
        value = (1.0F - 1.0F / parts) * value + (1.0F / parts) * elem;
    }
}


template <typename T>
T Utils::AroundAverager<T>::Value()
{
    if (pushed < parts)
    {
        return value / pushed;
    }

    return value;
}
