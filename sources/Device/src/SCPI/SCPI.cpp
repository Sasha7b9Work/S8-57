#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


const char *ErrorSCPI::prolog = "!!! ERROR !!! ";


/// Рекурсивная функция обработки массива структур StructSCPI.
/// В случае успешного выполнения возвращает адрес символа, расположенного за последним обработанным символом.
/// В случае неуспешного завершения - возвращает nullptr. Код ошибки находится в *error
static const char *Process(const char *buffer, const StructSCPI structs[], ErrorSCPI *error); //-V2504
/// Обработка узла дерева node
static const char *ProcessNode(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// Обработка листа node
static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// Возвращает true, если символ является разделителем или '*'
static bool IsSeparator(const char &symbol);
/// Удаляет неправильные символы из начала строки
static void RemoveBadSymbolsFromBegin();
/// Удалить последовательность разделителей из начала строки до последнего имеющегося
static bool RemoveSeparatorsSequenceFromBegin();
/// Удалить все символы до первого разделителя
static bool RemoveSymbolsBeforeSeparator();

static String data;

static String badSymbols;

void SCPI::AppendNewData(const char *buffer, uint)
{
    data.Append(buffer);

    SU::ToUpper(data.c_str());
}


void SCPI::Update()
{
    RemoveBadSymbolsFromBegin();

    if(data.Size() == 0)
    {
        return;
    }

    ErrorSCPI error;

    const char *end = Process(data.c_str(), head, &error);

    error.SendMessage();

    if(end)
    {
        data.RemoveFromBegin(static_cast<uint>(end - data.c_str()));
    }
}


static const char *Process(const char *buffer, const StructSCPI strct[], ErrorSCPI *error) //-V2504
{
    while (!strct->IsEmpty())
    {
        const char *end = SCPI::BeginWith(buffer, strct->key);

        if (end)
        {
            if (strct->IsNode())
            {
                return ProcessNode(end, strct, error);
            }
            else if (strct->IsLeaf())
            {
                return ProcessLeaf(end, strct, error);
            }
            else
            {
                LOG_WRITE("Сюда мы попасть ну никак не можем");
            }
        }

        strct++;
    }

    if (data.Size())
    {
        data.RemoveFromBegin(1);
    }

    RemoveBadSymbolsFromBegin();

    return nullptr;
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


static const char *ProcessNode(const char *begin, const StructSCPI *node, ErrorSCPI *error)
{
    return Process(begin, node->strct, error);
}


static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error)
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

    error->Set(ErrorSCPI::InvalidSequence, begin, begin + 1);

    return begin + 1;
}


bool SCPI::IsLineEnding(const char **buffer)
{
    bool result = (**buffer == 0x0D);

    if (result)
    {
        *(*buffer)++;
    }

    return result;
}


void ErrorSCPI::SendMessage()
{
    if(state == Success)
    {
        return;
    }

    static const char *names[] =
    {
        "",
        "Invalid sequence"
    };

    if (state == InvalidSequence)
    {
        String invalidSequence;

        const char *p = startInvalidSequence;

        while(p < endInvalidSequence)
        {
            invalidSequence.Append(*p++);
        }

        String message("%s %s : %s", prolog, names[state], invalidSequence.c_str());

        SCPI::SendAnswer(message.c_str());
    }
}


static void RemoveBadSymbolsFromBegin()
{
    while (RemoveSymbolsBeforeSeparator() || RemoveSeparatorsSequenceFromBegin())  { }
}


static bool RemoveSymbolsBeforeSeparator()
{
    if (data.Size() && !IsSeparator(data[0]))
    {
        ErrorSCPI error(ErrorSCPI::InvalidSequence);

        error.startInvalidSequence = data.c_str();
        error.endInvalidSequence = data.c_str();

        for(uint i = 0; i < data.Size() && data[i] != SCPI::SEPARATOR && data[i] != '*'; i++)
        {
            error.endInvalidSequence++;
        }

        error.SendMessage();

        data.RemoveFromBegin(static_cast<uint>(error.endInvalidSequence - error.startInvalidSequence));

        return true;
    }

    return false;
}


static bool RemoveSeparatorsSequenceFromBegin()
{
    bool result = false;

    ErrorSCPI error;

    error.Set(ErrorSCPI::Success, data.c_str(), data.c_str());

    while ((error.endInvalidSequence - error.startInvalidSequence) + 2U <= data.Size()   &&
        IsSeparator(*error.endInvalidSequence)                                          && 
        IsSeparator(*(error.endInvalidSequence + 1)))
    {
        result = true;

        error.state = ErrorSCPI::InvalidSequence;

        error.endInvalidSequence++;
    }

    if (error.state == ErrorSCPI::InvalidSequence)
    {
        error.SendMessage();

        data.RemoveFromBegin(static_cast<uint>(error.endInvalidSequence - error.startInvalidSequence));
    }

    return result;
}


void SCPI::SendAnswer(char *message)
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


static bool IsSeparator(const char &symbol)
{
    return (symbol == SCPI::SEPARATOR) || (symbol == '*');
}


void SCPI::SendBadSymbols()
{
//    if (!badSymbols.Size())
//    {
//
//    }
}
