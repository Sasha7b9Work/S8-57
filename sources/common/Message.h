#pragma once



class Message
{
public:
    explicit Message();
    explicit Message(int size, uint8);
    explicit Message(int size, uint8, uint8);
    explicit Message(int size, uint8, uint16, uint8);
    explicit Message(int size, uint8, uint8, uint);
    explicit Message(int size, uint8, uint16, uint8, uint8);
    explicit Message(uint8, uint8, uint8, uint8 *, uint16, uint8 *, uint16);
    explicit Message(int size, uint8, uint16, uint8, uint16, uint8);
    explicit Message(uint8, uint16, uint8, char *);
    ~Message();

    int Size() const { return allocated; }

    uint8 *Data(int pos = 0) { return data + pos; }	// -V104

    class Create
    {
    public:

        static void DrawHLine(Message &message, int y, int x0, int x1);

        static void DrawRectangle(Message &message, int x, int y, int width, int height);
    };

private:

    // Положить один байт
    void PutByte(uint8);
    
    // Положить послова
    void PutHalfWord(uint16);
    
    // Положить слово
    void PutWord(uint);
    
    // Выделить память
    bool Allocate(int size);
    
    // Освободить занятую память
    void Free();
    
    // Сколько выделено памяти
    int allocated;
    
    // Сколько заполнено (функциями PutX())
    int used;
    
    // Указатель на выделенный участок памяти
    uint8 *data;
};
