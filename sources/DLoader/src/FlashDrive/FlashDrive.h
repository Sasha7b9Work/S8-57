#pragma once


#define TIME_WAIT   5    // Время работы заставки


struct State
{
    enum E
    {
        Start,
        Mount,            // Монтирование флешки
        WrongFlash,       // Флешка есть, но прочитать нельзя
        RequestAction,    // Что делать - апгрейдить или нет
        Upgrade,          // Процесс апгрейда
        Ok                // Обновление удачно завершено
    };
};


class FDrive
{
    friend class CPU;

public:
    static void Init();

    // Сделать попытку обновления
    static void AttemptUpdate();

    static bool FileExist(const char *fileName);

    // Открывает файл для чтения. Возвращает размер файла
    static int OpenFileForRead(const char *fileName);

    // Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
    static int ReadFromFile(int numBytes, uint8 *buffer);

    static void CloseOpenedFile();

    static void *GetHandleHCD();

    static void *GetHandleUSBH();

    static float PercentsUpdated();

    static State::E State();

    class LL_
    {
    public:
        static void InitHCD(void *phost);

        static void SetToggle(uint8 pipe, uint8 toggle);

        static uint8 GetToggle(uint8 pipe);
    };
};
