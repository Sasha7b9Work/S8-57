#pragma once



namespace Utils
{
    template<typename T>
    // Усреднятор - усредняет последние попавшие в себя значения
    class Averager
    {
    public:
        Averager(uint size);
        ~Averager();
        void Push(T elem);
        T Value() const;
    private:
        T *buffer;
        uint size;
        uint numELements;
    };

    // Приблизительно усреднение
    template <typename T>
    class AroundAverager
    {
    public:
        AroundAverager(uint parts);
        // Пушит очередное значение и одновременно возвращает усреднённое
        void Push(T elem);
        T Value();
    private:
        T value;
        // На сколько частей делится значение
        const uint parts;
        // Сколько значений обработано
        uint pushed;
    };
}
