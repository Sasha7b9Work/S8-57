#include "defines.h"
#include "DataBuffer.h"
#include "DataSettings.h"
#include <cstring>
#include <climits>

#include "Data/Heap.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Вспомогательный класс для урощения работы с массивом DataSettings
class Stack
{
public:

    static uint Size();
    /// В конструктор передаётся адрес первого элемента и количество элементов
    static void Clear();

    static void Push(DataSettings *ds);

    static DataSettings *Top();
};

/// Максимальное число сохранённых сигналов - количество точек равно 512, пиковый детекотор выключен, включён только один канал
#define MAX_DATAS 150

/// \todo Эти данные могут храниться в backup SRAM
/// Элементы хранятся от самого старого к самому молодому строго по порядку.
/// Признаком того, что элемент пустой, является равенство нулю первых восьми байт элемента (указатели на данные обоих каналов равны нулю)
static DataSettings settings[MAX_DATAS];
/// По этому индексу хранятся последние данные в settings. -1 - данне не добавлены
static int lastIndex = -1;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
/// Возвращает индекс первого пустого элемента в settings
static int Stack_FindFirstEmptyElement();
/// Возвращает адрес в буфере, куда можно записать данные. Если 0, то места для записи нет
static uint8 *Stack_AddressToPlace(const DataSettings *ds);
/// Удаляет самые старые данные 
static void Stack_RemoveFirst();
/// Дописывает в конец новые данные
static void Stack_AddToEnd(DataSettings *ds, uint8 *address);


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void DataBuffer::Init()
{
    Stack::Clear();

    uint8 *buffer = (uint8 *)Heap::Data();

    for(uint i = 0; i < Heap::Size(); i++)
    {
        buffer[i] = (uint8)i;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint DataBuffer::Size()
{
    return Stack::Size();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Stack::Clear()
{
    std::memset(settings, 0, sizeof(DataSettings) * MAX_DATAS);
    for (int i = 0; i < MAX_DATAS; i++)
    {
        settings[i].Erase();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Stack::Push(DataSettings *ds)
{
    uint8 *address = Stack_AddressToPlace(ds);  // Находим адрес для записи данных

    if(address == 0)                            // Если нет места для размещения новых данных
    {
        Stack_RemoveFirst();                    // то удаляем самые старые данные
        address = Stack_AddressToPlace(ds);
    }

    Stack_AddToEnd(ds, address);                // И добавляем в конец новые данные
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
DataSettings *Stack::Top()
{
    if (lastIndex == -1)
    {
        return 0;
    }
    return &settings[lastIndex];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
uint Stack::Size()
{
    uint result = 0U;

    for (int i = 0; i < MAX_DATAS; i++)
    {
        if (settings[i].IsExist())
        {
            result++;
        }
    }

    return result;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static uint8 *Stack_AddressToPlace(const DataSettings *_ds)
{
    // В этом массиве хранится true, если участок памяти свободен
    bool freeData[MAX_DATAS];

    // Делаем все участки "свободными"
    for(int i = 0; i < MAX_DATAS; i++)
    {
        freeData[i] = true;
    }

    uint8 *buffer = (uint8 *)Heap::Data();

    // Сейчас будем их "занимать"
    for(int i = 0; i < MAX_DATAS; i++)
    {
        DataSettings *ds = &settings[i];

        if(ds->IsExist() &&                 // Если данные имеют смысл
            !ds->IsEmpty())             // и с данными
        {
            int size = _ds->SizeData();                     // Размер одного "блока" в буфере данных

            uint8 *addressFirst = &buffer[0];               // Адрес начального "блока" данных
            uint8 *address = ds->Data();                    // Адрес найденного "блока" данных

            uint index = (uint)(address - addressFirst) / (uint)size;    // Столько "блоков" данных разделяет начальный и текущий   // -V202

            freeData[index] = false;                        // Устанавливаем признак того, что данный блок занят
        }
    }

    // Теперь находим первый свободный блок данных
    int index = 0;
    for(; index < MAX_DATAS; index++)
    {
        if(freeData[index])
        {
            break;
        }
    }

    uint8 *address = &buffer[0] + index * _ds->SizeData();          // Находим адрес первого свободного блока данных

    return (&buffer[Heap::Size()] - address) >= _ds->SizeData() ? address : 0;   // -V104
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Stack_RemoveFirst()
{
    // Просто смещаем все элементы влево на один
    std::memcpy(&settings[0], &settings[1], sizeof(DataSettings) * (MAX_DATAS - 1));

    // И затираем последний элемент в массиве
    std::memset(&settings[MAX_DATAS - 1], 0, sizeof(DataSettings));
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void Stack_AddToEnd(DataSettings *ds, uint8 *address)
{
    int index = Stack_FindFirstEmptyElement();

    settings[index] = *ds;
    
    lastIndex = index;

    if(ds->DataA())
    {
        std::memcpy(address, ds->DataA(), (uint)ds->SizeChannel());
        settings[index].dataA = address;
        address += ds->SizeChannel();   // -V102
    }
    if(ds->DataB())
    {
        std::memcpy(address, ds->DataB(), (uint)ds->SizeChannel());
        settings[index].dataB = address;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static int Stack_FindFirstEmptyElement() // -V2506
{
    for (int i = 0; i < MAX_DATAS; i++)
    {
        DataSettings *ds = &settings[i];
        if (!ds->IsExist() || ds->IsEmpty())
        {
            return i;
        }
    }

    return INT_MAX;
}
