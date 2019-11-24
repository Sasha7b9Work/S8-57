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


#define SCPI_NODE(key, strct) {key, strct, nullptr}
#define SCPI_LEAF(key, func) {key, nullptr, func}
#define SCPI_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG(t)  return t; } return nullptr;

#define SCPI_REQUEST(func)                          \
    const char *end = SCPI::BeginWith(buffer, "?"); \
    if (end) {                                      \
        SCPI_PROLOG(end)                            \
        func;                                       \
        SCPI_EPILOG(end) }

#define SCPI_PROCESS_ARRAY(num, names, func)        \
    for(int i = 0; i < num; i++) {                  \
        end = SCPI::BeginWith(buffer, names[i]);    \
        if(end) {                                   \
            SCPI_PROLOG(end)                        \
            func;                                   \
            SCPI_EPILOG(end) }}                     \
    return nullptr;

namespace SCPI
{
    /// Символ-разделить морфем команды
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(const char *buffer, uint length);

    void Update();
    /// Возвращает true, если указатель указывает на завершающую последовательность
    bool IsLineEnding(const char **bufer);
    /// Послать ответ
    void SendAnswer(const char *message);
    /// Если строка buffer начинается с последовательности символов word, то возвращает указатель на символ, следующий за последним символом последовательности word.
    /// Иначе возвращает nullptr.
    const char *BeginWith(const char *buffer, const char *word);
    /// Послать сообщение об ошибочных символах, если таковые имеются
    void SendBadSymbols();
};
