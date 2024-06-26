// 2023/11/21 10:10:58 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


/*
    ������ �������.
    1. ������� ������ ���������� � ������� ':'.
    2. ���� ����������� ��������� ':'.
    3. ������� ����������� ������ ����� ���������� � ���������� 1 ��.
    4. ������� ������ ������������� �������� � ����� 0x0D.
*/


class String;

typedef const char *(*FuncSCPI)(pCHAR);
typedef bool (*FuncTestSCPI)();
typedef void (*FuncHint)(String *);


// ���������, �������������� ���� ������.
struct StructSCPI
{
    const char *key;            // �������� ����� ���� (�������)

    const StructSCPI *strct;    // ���� ��������� ����� ��� Node, �� ����� �������� ������ �������� - StructSCPI *structs.

    FuncSCPI  func;             // ���� ��������� ����� ��� Leaf, �� ����� �������� ������� - ���������� ����� ���� FuncSCPI

    FuncTestSCPI test;

    const char *hint;

    FuncHint funcHint;

    bool IsEmpty() const { return key[0] == '\0'; };
    bool IsNode() const { return strct != nullptr; };   // ��������� �������� "�����" ������, ����� ���� ������ �� ������ ����� structs
    bool IsLeaf() const { return func != nullptr; };    // ��������� �������� "������" ������, ����� ��������� ������� func
};


#define SCPI_NODE(key, strct)                      {key, strct,   nullptr, nullptr, nullptr}
#define SCPI_LEAF(key, func, test, hint, funcHint) {key, nullptr, func,    test,    hint,   funcHint}
#define SCPI_EMPTY() {""}

#define SCPI_PROLOG(t)  if(SCPI::IsLineEnding(&t)) { SCPI::SendBadSymbols();
#define SCPI_EPILOG(t)  return t; } return nullptr;

#define SCPI_RUN_IF_END(func) if(end) { SCPI_PROLOG(end) func; SCPI_EPILOG(end) }

#define SCPI_REQUEST(func)                          \
    const char *end = SCPI::BeginWith(buffer, "?"); \
    SCPI_RUN_IF_END(func)

#define SCPI_PROCESS_ARRAY(names, func)             \
    for(int i = 0; i < names[i][0] != 0; i++)       \
    {                                               \
        end = SCPI::BeginWith(buffer, names[i]);    \
        SCPI_RUN_IF_END(func)                       \
    }                                               \
    return nullptr;

#define SCPI_EXIT_ERROR()   LOG_WRITE("������ ����� SCPI %s:%d", __FILE__, __LINE__); return false;

#define SCPI_APPEND_STRING(string) SCPI::AppendNewData(string.c_str(), static_cast<int>(std::strlen(string.c_str()))); SCPI::Update()

#define SCPI_IF_BEGIN_WITH_THEN(begin, func) end = SCPI::BeginWith(buffer, begin); if (end) { SCPI_PROLOG(end); func; SCPI_EPILOG(end); }


namespace SCPI
{
    // ������-��������� ������ �������
    const char SEPARATOR = ':';

    const int SIZE_SEPARATOR = 1;

    void AppendNewData(const char *buffer, int length);

    void Update();

    // ���������� true, ���� ��������� ��������� �� ����������� ������������������
    bool IsLineEnding(const char **bufer);

    // ������� ����� � � ����� �������� 0x0D, ���� ���
    void SendAnswer(pCHAR message);
    void SendAnswer(const String &message);

    // ������� ������ ��� ���� - ��� ������������ �������
    void SendData(pCHAR message);
    void SendData(const String &message);

    // ������� ��������� � SCPI - � ������� ���������� �������� � ������ ���������
    void SendMeasure(const String &message);

    // ���� ������ buffer ���������� � ������������������ �������� word, �� ���������� ��������� �� ������, ��������� �� ��������� �������� ������������������ word.
    // ����� ���������� nullptr.
    const char *BeginWith(const char *buffer, const char *word);

    // ������� ��������� �� ��������� ��������, ���� ������� �������
    void SendBadSymbols();

    bool Test();

    void ProcessHint(String *message, const char *const *names); //-V2504
};


namespace SCPI
{
    extern const StructSCPI channels[];     //-V2504

    extern const StructSCPI display[];      //-V2504

    extern const StructSCPI fft[];          //-V2504

    extern const StructSCPI freqmeter[];    //-V2504

    extern const StructSCPI head[];         //-V2504

    extern const StructSCPI key[];          //-V2504

    extern const StructSCPI measures[];     //-V2504

    extern const StructSCPI multimeter[];   //-V2504

    extern const StructSCPI tBase[];        //-V2504

    extern const StructSCPI tester[];       //-V2504

    extern const StructSCPI trigger[];      //-V2504

    namespace Sender
    {
        extern bool osci[Chan::Count];  // ������������� � true �������� ��������, ��� ����� ������������ ����� ������� � SCPI
        extern bool freqMeter;          // ������������� � true �������� ��������, ��� ����� �������� ���������� ������������ �������� � SCPI
        extern bool tester;             // �������� ������ ������� �����������
        extern bool multimeter;         // �������� � SCPI ������ �����������
        extern bool fft;                // �������� � SCPI ������
    }
}
