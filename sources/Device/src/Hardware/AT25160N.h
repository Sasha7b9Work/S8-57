#pragma once
#include "Settings/Settings.h"


/*
    Работа с микросхемой EEPROM AT25160N, предназначенной для хранения настроек.
*/
class AT25160N
{
friend class Settings;
public:

    static void Init();

    static void Test();
private:

    static void Save(Settings &set);

    static void Load(Settings &set);

    // Разрешить запись
    static void SetWriteLatch();
    
    // Запретить запись
    static void ResetWriteLatch();
    
    // Читать регистр статуса
    static uint8 ReadStatusRegister();
    
    // Записать регистр статуса
    static void WriteStatusRegister(uint8 data);
    
    // Записывает size байт, начиная с адреса address
    static void WriteData(uint address, uint8 *data, int size);
    
    // Посылает порцию буфера по данному адресу. Порция не может быть больше 32 байт
    static void Write32BytesOrLess(uint address, const uint8 *data, int size);
    
    // Читает size байт, начиная с адреса address
    static void ReadData(uint address, uint8 *data, int size);

    // Записывает байт в микросхему
    static void WriteByte(uint8 byte);
    
    // Читает байт из микросхемы
    static uint8 ReadByte();

    // Ожидает, пока не закончится внутреннй цикл записи
    static void WaitFinishWrite();
};
