#pragma once


/*
    Формат команды.
    1. Команда всегда начинается с символа ':'.
    2. Узлы разделяются символами ':'.
    3. Пробелы допускаются только перед параметром в количестве 1 шт.
    4. Команда должна заканчиваться символом с кодом 0x0D.
*/


typedef const char *(*FuncSCPI)(const char *);


/// Структура, соотвествующая узлу дерева.
struct StructSCPI
{
    const char *key;            /// Ключевое слово узла (морфема)

    const StructSCPI *strct;    /// Если структура имеет тип Node, то здесь хранится массив потомков - StructSCPI *structs.

    FuncSCPI  func;             /// Если структура имеет тип Leaf, то здесь хранится функция - обработчик листа типа FuncSCPI

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   /// Структура является "узлом" дерева, нужно идти дальше по дереву через structs
    bool IsLeaf() const { return func != nullptr; };    /// Стурктура является "листом" дерева, нужно выполнять функцию func
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
