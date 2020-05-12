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


class FDrive
{
public:
    static void Init();

    static void DeInit();
    
    static void Update();
    
    static void Mount();
    
    static bool IsConnected();
    
    static void GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles);
  
    static bool AppendStringToFile(const char *string);

    static bool GetNameDir(const char *fuulPath, int numDir, char *nameDirOut, StructForReadDir *sfrd);

    static bool GetNextNameDir(char *nameDirOut, StructForReadDir *sfrd);

    static void CloseCurrentDir(StructForReadDir *sfrd);

    static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *sfrd);

    static bool GetNextNameFile(char *nameFileOut, StructForReadDir *sfrd);

    // Функция создаст файл для записи. Если такой файл уже существует, сотрёт его, заменит новым нулевой длины и откроет его
    static bool OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite);

    static bool WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite);

    static bool CloseFile(StructForWrite *structForWrite);

    // Сохранить изображение экрана на флешку
    static void SaveScreen();

    static void *handle;
};
