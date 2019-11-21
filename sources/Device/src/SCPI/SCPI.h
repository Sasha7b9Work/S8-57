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
        InvalidSequence
    };

    ErrorSCPI(State s = Success) : state(s), startInvalidSequence(nullptr), endInvalidSequence(nullptr) {};

    void Set(State s, const char *start, const char *end)
    {
        state = s;
        startInvalidSequence = start;
        endInvalidSequence = end;
    };

    void SendMessage();
    
    State state;
    /// В случае определения неправильной последовательности этот указатель указывает на символ, следующий за последним
    const char *startInvalidSequence;
    const char *endInvalidSequence;

private:
    static const char *prolog;
};


typedef const char *(*FuncSCPI)(const char *);


/// Структура, соотвествующая узлу дерева.
struct StructSCPI
{
//    enum Type
//    {
//        Empty,
//        Node,           /// Структура является "узлом" дерева, нужно идти дальше по дереву через structs
//        Leaf            /// Стурктура является "листом" дерева, нужно выполнять функцию func
//    };
//
//    Type type;          /// Тип структуры
//
    const char *key;    /// Ключевое слово узла (морфема)

    const StructSCPI *strct;  /// Этот указатель следует интерпретировать в зависимости от типа структуры.
                        /// Если структура имеет тип Node, то здесь хранится массив потомков - StructSCPI *structs.
                        /// Если структура имеет тип Leaf, то здесь хранится функция - обработчик листа типа FuncSCPI

    FuncSCPI  func;

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };
    bool IsLeaf() const { return func != nullptr; };
};


#define DEF_NODE(key, strct) {key, strct, nullptr}
#define DEF_LEAF(key, func) {key, nullptr, func}
#define DEF_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG(t)  return t; } return nullptr;


struct SCPI
{
    /// Символ-разделить морфем команды
    static const char SEPARATOR = ':';

    static const int SIZE_SEPARATOR = 1;

    static void AppendNewData(const char *buffer, uint length);

    static void Update();

    /// Возвращает true, если указатель указывает на завершающую последовательность
    static bool IsLineEnding(const char **bufer);
    /// Послать ответ
    static void SendAnswer(char *message);
    /// Если строка buffer начинается с последовательности символов word, то возвращает указатель на символ, следующий за последним символом последовательности word.
    /// Иначе возвращает nullptr.
    static const char *BeginWith(const char *buffer, const char *word);
    /// Послать сообщение об ошибочных символах, если таковые имеются
    static void SendBadSymbols();
};
