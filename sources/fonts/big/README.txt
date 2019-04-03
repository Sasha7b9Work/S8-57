В этой папке находятся большие шрифты - для особых случаев

Перед конвертацией файл, сгенерированный FontEd, нужно пересохранить с помощью paint.net

Формат структуры шрифта таков

struct BigSymbol
{
    uint8 code;
    uint8 width;
    uint16 offset;
};

struct BigFont
{
    int8       height;      // Высота глифа
    uint8      numSymbols;  // Количество символов в таблице
    uint8     *data;        // Адрес данных
    BigSymbol *symbols;     // Это первый символ. Остальные расположены за ним
};

Скрипт генерирует структуру BigFont и массив c данными шрифта вида

uint8 dataFont24[N] =
{
    233, ...
};

BigSymbol symbolsFont24[N] =
{
    {30, 5, 0},
    {342, 35, 20}
};

BigFont font24 =
{
    24,
    10,
    dataFont24,
    symbolsFont24
};
