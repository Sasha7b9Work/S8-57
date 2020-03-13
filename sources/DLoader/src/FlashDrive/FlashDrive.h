#pragma once

/*
class FDrive
{
    friend class CPU;

public:
    static void Init();

public:
    static bool Update();

    static bool FileExist(const char *fileName);
    
    // Открывает файл для чтения. Возвращает размер файла
    static int OpenFileForRead(const char *fileName);
    
    // Считывает из открытого файла numBytes байт. Возвращает число реально считанных байт
    static int ReadFromFile(int numBytes, uint8 *buffer);

    static void CloseOpenedFile();

    static void HCD_IRQHandler();

    class LL_
    {
    public:
        static void InitHCD(USBH_HandleTypeDef *phost);

        static void SetToggle(uint8 pipe, uint8 toggle);

        static uint8 GetToggle(uint8 pipe);
    };

    static HCD_HandleTypeDef handleHCD;

    static USBH_HandleTypeDef  handleUSBH;
};
*/
