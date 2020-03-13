#include "defines.h"
#include <ctype.h>
#include <usbh_def.h>
#include <ff_gen_drv.h>
#include <usbh_diskio.h>
#include <usbh_core.h>
#include <usbh_msc.h>
#include "defines.h"
#include "CPU.h"
#include "ffconf.h"
#include "main.h"
#include "Hardware/HAL/HAL_PIO.h"



typedef struct
{
    char nameDir[_MAX_LFN + 1];
    char lfn[(_MAX_LFN + 1)];
    FILINFO fno;
    DIR dir;
} StructForReadDir;

static HCD_HandleTypeDef handleHCD;

static USBH_HandleTypeDef handleUSBH;



static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s);
static bool GetNextNameFile(char *nameFileOut, StructForReadDir *s);
static void USBH_UserProcess(USBH_HandleTypeDef *phost, uint8 id);


void *CPU::FDrive::GetHandleHCD()
{
    return &handleHCD;
}


void *CPU::FDrive::GetHandleUSBH()
{
    return &handleUSBH;
}


void CPU::FDrive::Init()
{
    __GPIOB_CLK_ENABLE();
    __USB_OTG_HS_CLK_ENABLE(); //-V760
    __HAL_RCC_USB_OTG_HS_CLK_ENABLE();
    __SYSCFG_CLK_ENABLE();

    HAL_PIO::Init(PIN_HCD_DM, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF12_OTG_HS_FS);
    HAL_PIO::Init(PIN_HCD_DP, HMode::AF_PP, HPull::No, HSpeed::High, HAlternate::AF12_OTG_HS_FS);

    HAL_NVIC_SetPriority(OTG_HS_IRQn, 5, 1);

    HAL_NVIC_EnableIRQ(OTG_HS_IRQn);

    ms->drive.state = StateDisk_Idle;
    ms->drive.connection = 0;
    ms->drive.active = 0;

    if (FATFS_LinkDriver(&USBH_Driver, ms->drive.USBDISKPath) == FR_OK) //-V2001
    {
        USBH_StatusTypeDef res = USBH_Init(&handleUSBH, USBH_UserProcess, 0);
        res = USBH_RegisterClass(&handleUSBH, USBH_MSC_CLASS);
        res = USBH_Start(&handleUSBH);
    }
}



void USBH_UserProcess(USBH_HandleTypeDef *, uint8 id)
{
    switch (id)
    {
        case HOST_USER_SELECT_CONFIGURATION:
            break;

        case HOST_USER_CLASS_ACTIVE:
            ms->drive.active++;
            ms->drive.state = StateDisk_Start;
            break;

        case HOST_USER_CLASS_SELECTED:
            break;

        case HOST_USER_CONNECTION:
            ms->drive.connection++;
            ms->state = State_Mount;
            f_mount(NULL, static_cast<TCHAR const*>(""), 0);
            break;

        case HOST_USER_DISCONNECTION:
            break;

        default:
            // ничего не делаем
            break;
    }
}



bool CPU::FDrive::Update()
{
    USBH_Process(&handleUSBH);
    if (ms->drive.state == StateDisk_Start)
    {
        if (f_mount(&(ms->drive.USBDISKFatFS), static_cast<TCHAR const*>(ms->drive.USBDISKPath), 0) == FR_OK)
        {
            return true;
        }
        else
        {
            ms->state = State_WrongFlash;
            return false;
        }
    }
    return false;
}



static void ToLower(char *str)
{
    while (*str)
    {
        *str = static_cast<char>(tolower(*str));
        str++;
    }
}



bool CPU::FDrive::FileExist(const char *fileName)
{
    char nameFile[255];
    char file[255];
    strcpy(file, fileName);
    ToLower(file);
    StructForReadDir strd;

    if (GetNameFile("\\", 0, nameFile, &strd))
    {
        ToLower(nameFile);
        if (strcmp(file, nameFile) == 0)
        {
            return true;
        }
        while (GetNextNameFile(nameFile, &strd))
        {
            ToLower(nameFile);
            if (strcmp(file, nameFile) == 0)
            {
                return true;
            }
        }
    }

    return false;
}



static bool GetNameFile(const char *fullPath, int numFile, char *nameFileOut, StructForReadDir *s)
{
    memcpy(reinterpret_cast<uint8 *>(s->nameDir), const_cast<char *>(fullPath), strlen(fullPath));
    s->nameDir[strlen(fullPath)] = '\0';

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
                strcpy(nameFileOut, pFNO->fname);
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



static bool GetNextNameFile(char *nameFileOut, StructForReadDir *s)
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
                strcpy(nameFileOut, pFNO->fname);
                return true;
            }
        }
    }
}


int CPU::FDrive::OpenFileForRead(const char *fileName)
{
    if (f_open(&ms->drive.file, fileName, FA_READ) == FR_OK)
    {
        return (int)f_size(&ms->drive.file);
    }
    return -1;
}


int CPU::FDrive::ReadFromFile(int numBytes, uint8 *buffer)
{
    uint readed = 0;
    if (f_read(&ms->drive.file, buffer, static_cast<UINT>(numBytes), &readed) == FR_OK)
    {
        return static_cast<int>(readed);
    }
    return -1;
}


void CPU::FDrive::CloseOpenedFile()
{
    f_close(&ms->drive.file);
}


void CPU::FDrive::LL_::InitHCD(void *host)
{
    USBH_HandleTypeDef *phost = static_cast<USBH_HandleTypeDef *>(host);

    handleHCD.Instance = USB_OTG_HS;
    handleHCD.Init.speed = HCD_SPEED_HIGH;
    handleHCD.Init.Host_channels = 12;
    handleHCD.Init.dma_enable = 0;
    handleHCD.Init.low_power_enable = 0;
    handleHCD.Init.phy_itface = HCD_PHY_EMBEDDED;
    handleHCD.Init.Sof_enable = 0;
    handleHCD.Init.vbus_sensing_enable = 0;
    handleHCD.Init.use_external_vbus = 0;

    handleHCD.pData = phost;
    phost->pData = &handleHCD;

    HAL_HCD_Init(&handleHCD);

    USBH_LL_SetTimer(phost, HAL_HCD_GetCurrentFrame(&handleHCD));
}


void CPU::FDrive::LL_::SetToggle(uint8 pipe, uint8 toggle)
{
    if (handleHCD.hc[pipe].ep_is_in)
    {
        handleHCD.hc[pipe].toggle_in = toggle;
    }
    else
    {
        handleHCD.hc[pipe].toggle_out = toggle;
    }
}


uint8 CPU::FDrive::LL_::GetToggle(uint8 pipe)
{
    uint8 toggle = 0;

    if (handleHCD.hc[pipe].ep_is_in)
    {
        toggle = handleHCD.hc[pipe].toggle_in;
    }
    else
    {
        toggle = handleHCD.hc[pipe].toggle_out;
    }
    return toggle;
}


void CPU::FDrive::HCD_IRQHandler()
{

}
