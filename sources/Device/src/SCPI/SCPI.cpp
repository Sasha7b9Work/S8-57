#include "defines.h"
#include "Hardware/VCP.h"
#include "SCPI/HeadSCPI.h"
#include "SCPI/SCPI.h"
#include "Utils/Buffer.h"
#include "Utils/StringUtils.h"
#include <cstring>


enum ErrorCode
{
    Success
};


/// Если строка buffer начинается с последовательности символов word, то возвращает указатель на символ, следующий за последним символом последовательности word.
/// Иначе возвращает nullptr.
static char *BeginWith(const char *buffer, const char *word);
/// Рекурсивная функция обработки массива структур StructSCPI.
/// В случае успешного выполнения возвращает адрес символа, расположенного за последним обработанным символом.
/// В случае неуспешного завершения - возвращает указатель на buffer.
static char *Process(const char *buffer, const StructSCPI structs[], ErrorCode *error);



void SCPI::AddNewData(const char *buffer, uint length)
{
    String string(buffer);

    SU::ToUpper(string.CString(), length);
    
    Buffer data(length + 1);
    std::memcpy(data.data, buffer, length);
    data.data[length - 1] = 0;
    
    if (SU::EqualsStrings(reinterpret_cast<char *>(data.data), const_cast<char *>("*IDN?")))
    {
        const char *answer = "MNIPI, S8-57, v.1.2";
        VCP::SendDataAsynch(reinterpret_cast<const uint8 *>(answer), std::strlen(answer) + 1);
    }

    ErrorCode error(Success);

    Process(buffer, head, &error);
}


static char *Process(const char *buffer, const StructSCPI *structs, ErrorCode *)
{
    const StructSCPI *str = structs;

    while (str)
    {
        char *end = BeginWith(buffer, str->key);

        if (end)
        {
            if (str->type == StructSCPI::Node)
            {

            }
            else if (str->type == StructSCPI::Leaf)
            {

            }
            else
            {
                LOG_WRITE("Сюда мы попасть ну никак не можем");
            }
        }

        str++;
    }

    return nullptr;
}


char *BeginWith(const char *, const char *)
{
    return nullptr;
}
