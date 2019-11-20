#pragma once


/*
    Формат команды.
    1. Команда всегда начинается с символа ':'.
    2. Узлы разделяются символами ':'.
    3. Пробелы допускаются только перед параметром в количестве 1 шт.
    4. Команда должна заканчиваться символом с кодом 0x0D.
*/


struct ErrorSCPI
{
    enum State
    {
        Success,
        UnknownCommand
    };

    ErrorSCPI(State s) : state(s) {};

    void SendMessage();
    
    State state;

    String additionalMessage;

private:
    const char *prolog = "ERROR !!! ";
};


typedef const char *(*FuncSCPI)(const char *, ErrorSCPI *);


/// Структура, соотвествующая узлу дерева.
struct StructSCPI
{
    enum Type
    {
        Empty,
        Node,           /// Структура является "узлом" дерева, нужно идти дальше по дереву через structs
        Leaf            /// Стурктура является "листом" дерева, нужно выполнять функцию func
    };

    Type type;          /// Тип структуры

    const char  *key;   /// Ключевое слово узла (морфема)

    void *param;        /// Этот указатель следует интерпретировать в зависимости от типа структуры.
                        /// Если структура имеет тип Node, то здесь хранится массив потомков - StructSCPI *structs.
                        /// Если структура имеет тип Leaf, то здесь хранится функция - обработчик листа типа FuncSCPI
};


struct SCPI
{
    /// Символ-разделить морфем команды
    static const char SEPARATOR = ':';

    static void AppendNewData(const char *buffer, uint length);

    static void Update();

    /// Возвращает true, если указатель указывает на завершающую последовательность
    static bool IsLineEnding(const char *bufer);
};
