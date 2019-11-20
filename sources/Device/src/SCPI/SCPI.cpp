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
static const char *Process(const char *buffer, const StructSCPI structs[], ErrorSCPI *error);
/// Обработка узла дерева node
static const char *ProcessNode(const char *begin, const StructSCPI *node, ErrorSCPI *error);
/// Обработка листа node
static const char *ProcessLeaf(const char *begin, const StructSCPI *node, ErrorSCPI *error);


void SCPI::AddNewData(const char *buffer, uint)
{
    String text(buffer);

    SU::ToUpper(text.CString());

    ErrorSCPI error(SCPI_Success);

    Process(text.CString(), head, &error);
}


static const char *Process(const char *buffer, const StructSCPI *strct, ErrorSCPI *error)
{
    *error = SCPI_Success;

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
        if (*word == *buffer)
        {
            ++word;
            ++buffer;
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
