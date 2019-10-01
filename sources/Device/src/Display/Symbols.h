#pragma once


 // Font::Type::_5
#define SYMBOL_COUPLE_AC        '\x91'
#define SYMBOL_COUPLE_DC        '\x92'
#define SYMBOL_COUPLE_GND       '\x90'


// Font::Type::_8
#define SYMBOL_GOVERNOR_SHIFT_3 '\x0e'  ///< Символ регулятора
#define SYMBOL_TSHIFT_NORM_2    '\x84'  ///< 2-я часть символа смещения по времени нормального
#define SYMBOL_TSHIFT_NORM_1    '\x85'  ///< 1-я часть символа смещения по времени нормального
#define SYMBOL_TSHIFT_LEFT_2    '\x86'  ///< 2-я часть символа смещения по времени слева от экрана
#define SYMBOL_TSHIFT_RIGHT_2   '\x87'  ///< 2-я часть символа смещения по времени справа от экрана
#define SYMBOL_TPOS_1           '\x8a'  ///< 2-я часть символа TPos
#define SYMBOL_TPOS_2           '\x8b'  ///< 1-я часть символа TPos
#define SYMBOL_GOVERNOR_SHIFT_2 '\x8c'  ///< Символ регулятора
#define SYMBOL_TSHIFT_RIGHT_1   '\x93'  ///< 1-я часть символа смещения по времени справа от экрана
#define SYMBOL_RSHIFT_NORMAL    '\x93'  ///< Символ смещения по напряжению нормальный
#define SYMBOL_TSHIFT_LEFT_1    '\x94'  ///< 1-я часть символа смещения по времени слева от экрана
#define SYMBOL_TRIG_LEV_NORMAL  '\x94'  ///< Символ уровень синхронизации нормальный
#define SYMBOL_TRIG_LEV_LOWER   '\x95'  ///< Символ уровень синхронизации ниже сетки
#define SYMBOL_RSHIFT_LOWER     '\x95'  ///< Символ смещения по напряжению ниже сетки
#define SYMBOL_TRIG_LEV_ABOVE   '\x96'  ///< Символ уровень синхронизации выше сетки
#define SYMBOL_RSHIFT_ABOVE     '\x96'  ///< Символ смещения по напряжению выше сетки
#define SYMBOL_GOVERNOR_SHIFT_0 '\xaa'  ///< Символ регулятора
#define SYMBOL_GOVERNOR_SHIFT_1 '\xac'  ///< Символ регулятора
#define SYMBOL_PLAY             '\xae'  ///< Символ работы справа вверху экрана
#define SYMBOL_TPOS_1           '\x8a'  ///< 2-я часть символа TPos
#define SYMBOL_GOVERNOR_LEFT    '\x80'
#define SYMBOL_GOVERNOR_RIGHT   '\x81'


struct SymbolUGO2
{
    enum E
    {
        PEAK_DET_LEFT   = '\x12',  ///< Левая часть значка пикового детектора
        PEAK_DET_RIGHT  = '\x13',  ///< Правая часть значка пикового детектора
        ARROW_LEFT      = '\x20',
        DELETE          = '\x22',  ///< ВНЕШН ЗУ-МАСКА-УДАЛИТЬ
        INSERT          = '\x26',  ///< ВНЕШН ЗУ-ИМЯ-ВСТАВИТЬ
        SAVE_TO_MEM     = '\x2c',
        ROM             = '\x40',  ///< Микросхемка
        FLASH_DRIVE_BIG = '\x42',  ///< Символ флешки
        FOLDER          = '\x46',  ///< Каталог
        TRIANGLE_UP     = '\x4c',  ///< Стрелка треугольником вверх
        TRIANGLE_DOWN   = '\x4e',  ///< Стрекла треугольником вниз
        ARROW_RIGHT     = '\x64',
        TAB             = '\x6e',
        FLASH_DRIVE     = '\x80',  ///< Символ флешки внизу экрана
        ETHERNET        = '\x82',  ///< Символ Ethernet внизу экрана
        USB             = '\x84',  ///< Символ USB внизу экрана
        MATH_FUNC_MUL   = '\x8a',  ///< МАТЕМАТИКА-ФУНКЦИЯ-УМНОЖЕНИЕ
        TRIANGLE_LEFT   = '\x8c',  ///< Стрелка треугольником влево
        TRIANGLE_RIGHT  = '\x8e',  ///< Стрелка треугольником вправо
        SELECT          = '\xa0'   ///< Символ выбора
    };
};
