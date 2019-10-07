#pragma once


template<typename T>
class Queue
{
public:
    Queue();

    ~Queue();

    void Push(T elem);

    T Front();

    T Back();

    uint Size() const;

    T operator[](int n);
private:

    void Destroy();
    /// Указатель на массив элементов
    T *pointer;
    /// Индекс первого элемента
    int iFront;
    /// Индекс элемента за последним
    int iBack;
};
