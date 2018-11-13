#pragma once


///////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class CF
{
public:
    /// Выводит в консоль num первых значений из buffer
    static void LogBufferU8(uint8 *buffer, int num);
    /// Возвращает число слов в строке string
    static int NumWords(char *string);
    /// Возвращает указатель на n слово в строке. Если char == 0 - слова нет, если ret value == 0xffffffff - выходной буфер слишком мал
    static char *GetWord(char *string, int n, char *out, int size);
};


// В буфере после каждого вызова Push остаются пять наибольших после последнего вызова Clear() значений
class BufferMax5
{
public:
    BufferMax5();
    void Clear();
    void Push(uint16 value);
    uint16 GetValue(int index);
private:
    uint16 buffer[5];
    int numElements;
};
