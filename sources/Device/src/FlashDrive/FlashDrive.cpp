#include "defines.h"
#include "Data/DataSettings.h"
#include "FlashDrive/FileManager.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Menu/Menu.h"
#include "usbh_diskio.h"


#define NEED_MOUNT (bf.needToMoundFlash)


static struct BitFieldFlashDrive
{
    uint needToMoundFlash : 1;  ///< Установленное в 1 значение означает, что подсоединена флешка. Надо её монтировать.
    uint notUsed          : 31;
} bf = {0, 0};


USBH_HandleTypeDef FDrive::hUSB_Host;
HCD_HandleTypeDef  FDrive::handleHCD;
static FATFS USBDISKFatFs;
static char USBDISKPath[4]; // -V112
static bool gFlashDriveIsConnected = false;



/// Устанавливает текущее время для файла nameFile
static void SetTimeForFile(const char *nameFile);



static void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch(id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            NEED_MOUNT = 1;

            /*
            if (f_mount(&USBDISKFatFs, (TCHAR const*)USBDISKPath, 1) != FR_OK)
            {
                display.ShowWarning(WrongFileSystem);
            }
            else
            {
                gFlashDriveIsConnected = true;
                FM_Init();
                Menu::ChangeStateFlashDrive();
            }
            */
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            f_mount(NULL, static_cast<TCHAR const*>(""), 0);
            break;

        case HOST_USER_DISCONNECTION:
            gFlashDriveIsConnected = false;
            Menu::ChangeStateFlashDrive();
            break;

        default:
            // ничего не надо делать
            break;
    }
}



void FDrive::Mount()
{
    FileManager::Init();
    Menu::ChangeStateFlashDrive();
    if (f_mount(&USBDISKFatFs, static_cast<TCHAR const*>(USBDISKPath), 0) != FR_OK)
    {
        LOG_ERROR("Не могу примонтировать диск");
    }
}


bool FDrive::IsConnected()
{
    return gFlashDriveIsConnected;
}



void FDrive::Init()
{
    if(FATFS_LinkDriver(&USBH_Driver, USBDISKPath) == FR_OK)    // -V2001
    {
        USBH_StatusTypeDef res = USBH_Init(&hUSB_Host, USBH_UserProcess, 0);
        res = USBH_RegisterClass(&hUSB_Host, USBH_MSC_CLASS);
        res = USBH_Start(&hUSB_Host);
    }
    else
    {
        LOG_ERROR("Can not %s", __FUNCTION__);
    }
}



void FDrive::Update()
{
    if (NEED_MOUNT)      // Если обнаружено физическое подключение внешнего диска
    {
        uint timeStart = TIME_MS;
        NEED_MOUNT = 0;

        Display::FuncOnWait::Start("Обнаружено запоминающее устройство", false);

        if (f_mount(&USBDISKFatFs, static_cast<TCHAR const*>(USBDISKPath), 1) != FR_OK)
        {
            Display::ShowWarning("Не могу прочитать флешку. Убедитесь, что на ней FAT32");
        }
        else
        {
            gFlashDriveIsConnected = true;
            FileManager::Init();
            Menu::ChangeStateFlashDrive();
        }
        while (TIME_MS - timeStart < 3000)
        {
        };

        Display::FuncOnWait::Stop();
    }
    else
    {
        USBH_Process(&hUSB_Host);
    }
}



bool FDrive::AppendStringToFile(const char *)
{
    return false;
}



/*
static void WriteToFile(FIL *file, char *string)
{
    //    uint bytesWritten;
    f_open(file, "list.txt", FA_OPEN_EXISTING);
    //f_write(file, string, strlen(string), (void *)&bytesWritten);
    f_puts(string, file);
    f_close(file);
}
*/


void FDrive::GetNumDirsAndFiles(const char *fullPath, int *numDirs, int *numFiles)
{
    FILINFO fno;
    DIR dir;

    *numDirs = 0;
    *numFiles = 0;
    

    char nameDir[_MAX_LFN + 1];
    std::memcpy(nameDir, fullPath, std::strlen(fullPath));
    nameDir[std::strlen(fullPath)] = '\0';

    if (f_opendir(&dir, nameDir) == FR_OK)
    {
        int numReadingElements = 0;
        bool alreadyNull = false;
        while (true) //-V2530
        {
            if (f_readdir(&dir, &fno) != FR_OK)
            {
                break;
            }
            if (fno.fname[0] == 0)
            {
                if(alreadyNull)
                {
                    break;
                }
                alreadyNull = true;
                continue;
            }
            numReadingElements++;
            if (fno.fname[0] != '.')
            {
                if (fno.fattrib & AM_DIR)
                {
                    (*numDirs)++;
                }
                else
                {
                    (*numFiles)++;
                }
            }
        }
        f_closedir(&dir);
    }
}



bool FDrive::GetNameDir(const char *fullPath, int numDir, char *nameDirOut, StructForReadDir *s) // -V2506
{
    std::memcpy(s->nameDir, fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numDirs = 0;
        FILINFO *pFNO = &s->fno;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameDirOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numDir == numDirs && (pFNO->fattrib & AM_DIR))
            {
                std::strcpy(nameDirOut, static_cast<const char *>(pFNO->fname));
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) && (pFNO->fname[0] != '.'))
            {
                numDirs++;
            }
        }
    }
    return false;
}



bool FDrive::GetNextNameDir(char *nameDirOut, StructForReadDir *s) // -V2506
{
    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(pDir, pFNO) != FR_OK)
        {
            *nameDirOut = '\0';
            f_closedir(pDir);
            return false;
        }
        else if (pFNO->fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameDirOut = '\0';
                f_closedir(pDir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if (pFNO->fattrib & AM_DIR)
            {
                std::strcpy(nameDirOut, static_cast<const char *>(pFNO->fname));
                return true;
            }
        }
    }
}



void FDrive::CloseCurrentDir(StructForReadDir *s)
{
    f_closedir(&s->dir);
}



bool FDrive::GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s) // -V2506
{
    std::memcpy(s->nameDir, fullPath, std::strlen(fullPath));
    s->nameDir[std::strlen(fullPath)] = '\0';

    DIR *pDir = &s->dir;
    FILINFO *pFNO = &s->fno;
    if (f_opendir(pDir, s->nameDir) == FR_OK)
    {
        int numFiles = 0;
        bool alreadyNull = false;
        while (true)
        {
            if (f_readdir(pDir, pFNO) != FR_OK)
            {
                *nameFileOut = '\0';
                f_closedir(pDir);
                return false;
            }
            if (pFNO->fname[0] == 0)
            {
                if (alreadyNull)
                {
                    *nameFileOut = '\0';
                    f_closedir(pDir);
                    return false;
                }
                alreadyNull = true;
            }
            if (numFile == numFiles && (pFNO->fattrib & AM_DIR) == 0)
            {
                std::strcpy(nameFileOut, static_cast<const char *>(pFNO->fname));
                return true;
            }
            if ((pFNO->fattrib & AM_DIR) == 0 && (pFNO->fname[0] != '.'))
            {
                numFiles++;
            }
        }
    }
    return false;
}



bool FDrive::GetNextNameFile(char *nameFileOut, StructForReadDir *s) // -V2506
{
    FILINFO *pFNO = &s->fno;
    bool alreadyNull = false;
    while (true)
    {
        if (f_readdir(&s->dir, &s->fno) != FR_OK)
        {
            *nameFileOut = '\0';
            f_closedir(&s->dir);
            return false;
        }
        if (s->fno.fname[0] == 0)
        {
            if (alreadyNull)
            {
                *nameFileOut = '\0';
                f_closedir(&s->dir);
                return false;
            }
            alreadyNull = true;
        }
        else
        {
            if ((pFNO->fattrib & AM_DIR) == 0 && pFNO->fname[0] != '.')
            {
                std::strcpy(nameFileOut, static_cast<const char *>(pFNO->fname));
                return true;
            }
        }
    }
}



bool FDrive::OpenNewFileForWrite(const char *fullPathToFile, StructForWrite *structForWrite) // -V2506
{
    if (f_open(&structForWrite->fileObj, fullPathToFile, FA_CREATE_ALWAYS | FA_WRITE) != FR_OK)
    {
        return false;
    }
    std::strcpy(structForWrite->name, const_cast<char *>(fullPathToFile));
    structForWrite->sizeData = 0;
    return true;
}



bool FDrive::WriteToFile(uint8 *data, int sizeData, StructForWrite *structForWrite) // -V2506
{
    while (sizeData > 0)
    {
        int dataToCopy = sizeData;
        if (sizeData + structForWrite->sizeData > SIZE_FLASH_TEMP_BUFFER)
        {
            dataToCopy = SIZE_FLASH_TEMP_BUFFER - structForWrite->sizeData;
        }
        sizeData -= dataToCopy;
        std::memcpy(structForWrite->tempBuffer + structForWrite->sizeData, data, static_cast<uint>(dataToCopy));
        data += dataToCopy; // -V102
        structForWrite->sizeData += dataToCopy;
        if (structForWrite->sizeData == SIZE_FLASH_TEMP_BUFFER)
        {
            uint wr = 0;
            if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, static_cast<uint>(structForWrite->sizeData), &wr) != FR_OK || structForWrite->sizeData != static_cast<int>(wr))
            {
                return false;
            }
            structForWrite->sizeData = 0;
        }
    }

    return true;
}



bool FDrive::CloseFile(StructForWrite *structForWrite) // -V2506
{
    if (structForWrite->sizeData != 0)
    {
        uint wr = 0;
        if (f_write(&structForWrite->fileObj, structForWrite->tempBuffer, static_cast<uint>(structForWrite->sizeData), &wr) != FR_OK || structForWrite->sizeData != static_cast<int>(wr))
        {
            f_close(&structForWrite->fileObj);
            return false;
        }
    }
    f_close(&structForWrite->fileObj);

    SetTimeForFile(structForWrite->name);

    return true;
}


static void SetTimeForFile(const char *name)
{
    FILINFO info;

    PackedTime time = HAL_RTC::GetPackedTime();

    info.fdate = static_cast<WORD>(((time.year + 2000 - 1980) * 512) | time.month * 32 | time.day);        // -V112
    info.ftime = static_cast<WORD>(time.hours * 2048 | time.minutes * 32 | time.seconds / 2);              // -V112

    f_utime(name, &info);
}
