#pragma once


#define TIME_WAIT   3000    // Время работы заставки


struct State
{
    enum E
    {
        Start,
        Mount,            // Монтирование флешки
        WrongFlash,       // Флешка есть, но прочитать нельзя
        RequestAction,    // Что делать - апгрейдить или нет
        NotFile,          // Если диск примонтирован, но обновления на нём нету
        Upgrade,          // Процесс апгрейда
        Ok                // Обновление удачно завершено
    };
};


namespace FDrive
{
    void Init();

    // Сделать попытку обновления
    void AttemptUpdate();

    bool FileExist(const char *fileName);

    // Открывает файл для чтения. Возвращает размер файла
    int OpenFileForRead(const char *fileName);

    // Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
    int ReadFromFile(int numBytes, uint8 *buffer);

    void CloseOpenedFile();

    void *GetHandleHCD();

    void *GetHandleUSBH();

    float PercentsUpdated();

    State::E State();

    namespace LL_
    {
        void InitHCD(void *phost);

        void SetToggle(uint8 pipe, uint8 toggle);

        uint8 GetToggle(uint8 pipe);
    };
};
