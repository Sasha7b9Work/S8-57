// 2023/11/22 13:37:00 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include <ff.h>


#define SIZE_FLASH_TEMP_BUFFER 512
struct StructForWrite
{
    uint8   tempBuffer[SIZE_FLASH_TEMP_BUFFER];
    int     sizeData;
    FIL     fileObj;
    char    name[255];
    uint8   notUsed0;
};

struct StructForReadDir
{
    char nameDir[_MAX_LFN + 1];
    FILINFO fno;
    DIR dir;
};


namespace FDrive
{
    void Init();

    void DeInit();

    void Update();

    void Mount();

    bool IsConnected();

    void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);

    bool AppendStringToFile(const char *string);

    bool GetNameDir(const char *fuulPath, int numDir, char *nameDirOut, StructForReadDir *sfrd);

    bool GetNextNameDir(char *nameDirOut, StructForReadDir *sfrd);

    void CloseCurrentDir(StructForReadDir *sfrd);

    bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *sfrd);

    bool GetNextNameFile(char *nameFileOut, StructForReadDir *sfrd);

    bool ExistFile(const char *fullPath, const char *fileName);

    // Функция создаст файл для записи. Если такой файл уже существует, сотрёт его, заменит новым нулевой длины и откроет его
    bool OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite);

    bool WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite);

    bool CloseFile(StructForWrite *structForWrite);

    // Сохранить изображение экрана на флешку
    void SaveScreen();

    extern void *handle;
};
