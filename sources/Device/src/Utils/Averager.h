#pragma once



namespace Utils
{
    template<typename T>
    // Усреднятор - усредняет последние попавшие в себя значения
    class Averager
    {
    public:
        Averager(int size);
    
        ~Averager();
        
        void Push(T elem);
        
        T Value() const;

    private:
        
        T *buffer;
        
        int size;
        
        int numELements;
    };


    // Приблизительно усреднение
    template <typename T>
    class AroundAverager
    {
    public:
        
        AroundAverager(int parts);
        
        // Пушит очередное значение и одновременно возвращает усреднённое
        void Push(T elem);
        
        T Value();

    private:
        
        T value;
        
        // На сколько частей делится значение
        const int parts;
        
        // Сколько значений обработано
        int pushed;
    };
}
