// 2023/11/21 10:11:04 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"
#include <cstring>


namespace SCPI
{
    // ����������� ������� ��������� ������� �������� StructSCPI.
    // � ������ ��������� ���������� ���������� ����� �������, �������������� �� ��������� ������������ ��������.
    // � ������ ����������� ���������� - ���������� nullptr. ��� ������ ��������� � *error
    static const char *Process(const char *buffer, const StructSCPI structs[]); //-V2504

    // ����������� ������� ������������
    static bool ProcessTest(const StructSCPI strct[]); //-V2504

    // ��������� ���� ������ node
    static const char *ProcessNode(const char *begin, const StructSCPI *node);

    // ��������� ����� node
    static const char *ProcessLeaf(const char *begin, const StructSCPI *node);

    // ���������� true, ���� ������ �������� ������� ������� - ������������ ��� '*'
    static bool IsBeginCommand(const char &symbol);

    // ������� ������������ ������� �� ������ ������
    static void RemoveBadSymbolsFromBegin();

    // ������� ������������������ ������������ �� ������ ������ �� ���������� ����������
    static bool RemoveSeparatorsSequenceFromBegin();

    // ������� ��� ������� �� ������� �����������
    static bool RemoveSymbolsBeforeSeparator();

    static String data;

    static String badSymbols;

    bool SCPI::Sender::osci[Chan::Count] = { false, false };
    bool SCPI::Sender::freqMeter = false;
    bool SCPI::Sender::tester = false;
    bool SCPI::Sender::multimeter = false;
    bool SCPI::Sender::fft = false;
}


void SCPI::AppendNewData(const char *buffer, int size)
{
    data.Append(buffer, size);

    SU::ToUpper(data.c_str());

    RemoveBadSymbolsFromBegin();

    if (data.Size() == 0)
    {
        SendBadSymbols();
    }
}


void SCPI::Update()
{
    RemoveBadSymbolsFromBegin();

    if(data.Size() == 0)
    {
        SendBadSymbols();
        return;
    }

    const char *end = Process(data.c_str(), head);

    if(end)
    {
        data.RemoveFromBegin(static_cast<int>(end - data.c_str()));
    }
}


const char *SCPI::Process(const char *buffer, const StructSCPI strct[]) //-V2504
{
    while (!strct->IsEmpty())
    {
        const char *end = SCPI::BeginWith(buffer, strct->key);

        if (end)
        {
            if (strct->IsNode())
            {
                return ProcessNode(end, strct);
            }
            else if (strct->IsLeaf()) //-V2516
            {
                return ProcessLeaf(end, strct);
            }
        }

        strct++;
    }

    badSymbols.Append(*buffer);         // ��������� ��� ����� � strct � �� ����� �� ������ ������������. ������� �������� ��������� ����������� � ����������� ��������

    return buffer + 1;
}


const char *SCPI::BeginWith(const char *buffer, const char *word)
{
    while (*word)
    {
        if (*buffer == '\0')
        {
            return nullptr;
        }

        if (*word == *buffer)
        {
            ++word;
            ++buffer;
        }
        else
        {
            break;
        }
    }

    return (*word == '\0') ? buffer : nullptr;
}


const char *SCPI::ProcessNode(const char *begin, const StructSCPI *node)
{
    return Process(begin, node->strct);
}


const char *SCPI::ProcessLeaf(const char *begin, const StructSCPI *node)
{
    if (*begin == '\0')                     // ������������ �� ����, ��� ������ ��������� ������� �� ������
    {
        return nullptr;
    }

    const char *result = node->func(begin);

    if (result)
    {
        return result;
    }

    badSymbols.Append(*begin);

    return begin + 1;
}


bool SCPI::IsLineEnding(const char **buffer)
{
    bool result = (**buffer == 0x0D);

    if (result)
    {
        *(*buffer)++; //-V532
    }

    return result;
}


void SCPI::SendBadSymbols()
{
    if (badSymbols.Size())
    {
        String message("!!! ERROR !!! Invalid sequency : %s", badSymbols.c_str());
        SCPI::SendAnswer(message.c_str());
        badSymbols.Free();
    }
}


void SCPI::RemoveBadSymbolsFromBegin()
{
    while (RemoveSymbolsBeforeSeparator() || RemoveSeparatorsSequenceFromBegin())  { }
}


bool SCPI::RemoveSymbolsBeforeSeparator()
{
    bool result = false;

    while ((data.Size() != 0) && !IsBeginCommand(data[0]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


bool SCPI::RemoveSeparatorsSequenceFromBegin()
{
    bool result = false;

    while (data.Size() > 1 && IsBeginCommand(data[0]) && IsBeginCommand(data[1]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


void SCPI::SendAnswer(pCHAR message)
{
    if(message[std::strlen(message) - 1] != 0x0D) //-V2513
    {
        String msg(message);
        msg.Append(0x0D);
        VCP::SendStringAsynch(msg.c_str());
    }
    else
    {
        VCP::SendStringAsynch(message);
    }
}


void SCPI::SendAnswer(const String &message)
{
    SendAnswer(message.c_str());
}


void SCPI::SendData(pCHAR message)
{
    VCP::SendStringAsynch(message);
}


void SCPI::SendData(const String &message)
{
    VCP::SendStringAsynch(message.c_str());
}


bool SCPI::IsBeginCommand(const char &symbol)
{
    return (symbol == SCPI::SEPARATOR) || (symbol == '*');
}


bool SCPI::Test()
{
    return ProcessTest(head);
}


bool SCPI::ProcessTest(const StructSCPI strct[]) //-V2504
{
    while(!strct->IsEmpty())
    {
        if(strct->IsNode())
        {
            if(!ProcessTest(strct->strct))
            {
                return false;
            }
        }
        else if(strct->IsLeaf()) //-V2516
        {
            if(!strct->test())
            {
                return false;
            }
        }

        strct++;
    }

    return true;
}


void SCPI::ProcessHint(String *message, const char *const *names)
{
    message->Append(" {");
    for(int i = 0; i < names[i][0] != 0; i++)
    {
        message->Append(names[i]);
        message->Append(" |");
    }
    message->RemoveFromEnd();
    message->Append('}');
    SCPI::SendAnswer(message->c_str());
}


void SCPI::SendMeasure(const String &str)
{
    String message;

    for (int i = 0; i < str.Size(); i++)
    {
        char symbol = str[i];

        if (static_cast<uint8>(symbol) == 0xa8)     // ���� ���������� ������ ����
        {
            continue;
        }

        if (symbol == '\x10')     { symbol = ' '; }
        else if (symbol == '�')
        {
            if (str[i + 1] == '�') { message.Append('u'); i++; }
            else                   { message.Append('m');      }
            continue;
        }
        else if (symbol == '�')    { symbol = 'n'; }
        else if (symbol == '�')    { symbol = 's'; }
        else if (symbol == '�')    { symbol = 'V'; }
        else if (symbol == '�')    { symbol = 'A'; }
        else if (symbol == '�')    { symbol = 'M'; }
        else if (symbol == '�')    { symbol = 'k'; }
        else if ((symbol == '�') && (str[i + 1] == '�')) //-V2516
        {
            message.Append('H');  message.Append('z');  i++; continue;
        }

        message.Append(symbol);
    }

    SendAnswer(message.c_str());
}
