#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


/// Если строка buffer начинается с последовательности символов word, то возвращает указатель на символ, следующий за последним символом последовательности word.
/// Иначе возвращает nullptr.
static const char *BeginWith(const char *buffer, const char *word);
/// Рекурсивная функция обработки массива структур StructSCPI.
/// В случае успешного выполнения возвращает адрес символа, расположенного за последним обработанным символом.
/// В случае неуспешного завершения - возвращает nullptr. Код ошибки находится в *error
static const char *Process(const char *buffer, const StructSCPI structs[], ErrorSCPI *error); //-V2504
/// Обработка узла дерева node
static const char *ProcessNode(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// Обработка листа node
static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// Удалить начальные символы до begin из data
static void RemoveFromBegin(const char *begin);
/// Удалить все символы до разделителя
static void RemoveSymbolsBeforeSeparator();


static String data;


void SCPI::AddNewData(const char *buffer, uint)
{
    data.Append(buffer);

    RemoveSymbolsBeforeSeparator();

    SU::ToUpper(data.CString());

    ErrorSCPI error(ErrorSCPI::Success);

    const char *end = Process(data.CString(), head, &error);

    if (end)
    {
        RemoveFromBegin(end);
    }
    else
    {
        error.SendMessage();
    }
}


static const char *Process(const char *buffer, const StructSCPI strct[], ErrorSCPI *error) //-V2504
{
    while (strct->type != StructSCPI::Empty)
    {
        const char *end = BeginWith(buffer, strct->key);

        if (end)
        {
            if (strct->type == StructSCPI::Node)
            {
                return ProcessNode(end, strct, error);
            }
            else if (strct->type == StructSCPI::Leaf)
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

    return nullptr;
}


static const char *BeginWith(const char *buffer, const char *word)
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
    const StructSCPI *params = reinterpret_cast<const StructSCPI *>(node->param);

    return Process(begin, params, error);
}


static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error)
{
    FuncSCPI func = reinterpret_cast<FuncSCPI>(node->param);

    return func(begin, error);
}


bool SCPI::IsLineEnding(const char *buffer)
{
    return (*buffer == 0x0D);
}


void ErrorSCPI::SendMessage()
{
    if (state == Success)
    {
        return;
    }

    if (state == UnknownCommand)
    {
        VCP::SendString("UNKNOWN COMMAND");
        VCP::SendString(additionalMessage.CString());
    }
}


static void RemoveFromBegin(const char *begin)
{
    data.RemoveFromBegin(static_cast<uint>(begin - data.CString()));
}


static void RemoveSymbolsBeforeSeparator()
{
    if (data.Size() && data[0] != SCPI::SEPARATOR)
    {
        ErrorSCPI error(ErrorSCPI::UnknownCommand);

        while (data.Size() && data[0] != SCPI::SEPARATOR)
        {
            char symbols[2] = { data[0], '\0' };
            error.additionalMessage.Append(symbols);
            data.RemoveFromBegin(1);
        }

        error.SendMessage();
    }
}
