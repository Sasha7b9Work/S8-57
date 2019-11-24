#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


/// Рекурсивная функция обработки массива структур StructSCPI.
/// В случае успешного выполнения возвращает адрес символа, расположенного за последним обработанным символом.
/// В случае неуспешного завершения - возвращает nullptr. Код ошибки находится в *error
static const char *Process(const char *buffer, const StructSCPI structs[]); //-V2504
/// Рекурсивная функция тестирования
static bool ProcessTest(const StructSCPI strct[]);
/// Обработка узла дерева node
static const char *ProcessNode(const char *begin, const StructSCPI *node);
/// Обработка листа node
static const char *ProcessLeaf(const char *begin, const StructSCPI *node);
/// Возвращает true, если символ является началом комнады - разделителем или '*'
static bool IsBeginCommand(const char &symbol);
/// Удаляет неправильные символы из начала строки
static void RemoveBadSymbolsFromBegin();
/// Удалить последовательность разделителей из начала строки до последнего имеющегося
static bool RemoveSeparatorsSequenceFromBegin();
/// Удалить все символы до первого разделителя
static bool RemoveSymbolsBeforeSeparator();

static String data;

static String badSymbols;


void SCPI::AppendNewData(const char *buffer, uint size)
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
        data.RemoveFromBegin(static_cast<uint>(end - data.c_str()));
    }
}


static const char *Process(const char *buffer, const StructSCPI strct[]) //-V2504
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
            else if (strct->IsLeaf())
            {
                return ProcessLeaf(end, strct);
            }
            else
            {
                LOG_WRITE("Сюда мы попасть ну никак не можем");
            }
        }

        strct++;
    }

    badSymbols.Append(*buffer);         // Перебрали все ключи в strct и не нашли ни одного соответствия. Поэтому помещаем начальный разделитель в бракованные символыа

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


static const char *ProcessNode(const char *begin, const StructSCPI *node)
{
    return Process(begin, node->strct);
}


static const char *ProcessLeaf(const char *begin, const StructSCPI *node)
{
    if (*begin == '\0')                     // Подстраховка от того, что символ окончания команды не принят
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


static void RemoveBadSymbolsFromBegin()
{
    while (RemoveSymbolsBeforeSeparator() || RemoveSeparatorsSequenceFromBegin())  { }
}


static bool RemoveSymbolsBeforeSeparator()
{
    bool result = false;

    while (data.Size() && !IsBeginCommand(data[0]))
    {
        badSymbols.Append(data[0]);
        data.RemoveFromBegin(1);
        result = true;
    }

    return result;
}


static bool RemoveSeparatorsSequenceFromBegin()
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


void SCPI::SendAnswer(const char *message)
{
    if(message[std::strlen(message) - 1] != 0x0D)
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


static bool IsBeginCommand(const char &symbol)
{
    return (symbol == SCPI::SEPARATOR) || (symbol == '*');
}


bool SCPI::Test()
{
    return ProcessTest(head);
}


static bool ProcessTest(const StructSCPI strct[])
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
        else if(strct->IsLeaf())
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
