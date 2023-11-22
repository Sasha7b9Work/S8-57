#pragma once


#define TIME_WAIT   3000    // ����� ������ ��������


struct State
{
    enum E
    {
        Start,
        Mount,            // ������������ ������
        WrongFlash,       // ������ ����, �� ��������� ������
        RequestAction,    // ��� ������ - ���������� ��� ���
        NotFile,          // ���� ���� �������������, �� ���������� �� �� ����
        Upgrade,          // ������� ��������
        Ok                // ���������� ������ ���������
    };
};


namespace FDrive
{
    void Init();

    // ������� ������� ����������
    void AttemptUpdate();

    bool FileExist(const char *fileName);

    // ��������� ���� ��� ������. ���������� ������ �����
    int OpenFileForRead(const char *fileName);

    // ��������� �� ��������� ����� numBytes ����. ���������� ����� ������� ��������� ����
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
