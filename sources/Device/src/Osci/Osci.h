// 2023/11/22 13:46:27 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Settings/Settings.h"
#include "Utils/Buffer.h"


#define OSCI_IN_MODE_P2P        (S_TIME_BASE >= TBase::MIN_P2P)
#define OSCI_IN_MODE_RANDOMIZER (S_TIME_BASE <= TBase::_50ns)



namespace OsciStateWork
{
    enum E
    {
        Stopped,        // ����������� � ��������� ��������
        Awaiting,       // ����������� � ��������� �������� �������������
        Triggered       // ����������������
    };

    OsciStateWork::E Current();
};



namespace Osci
{
    void Init();

    void Update();

    void OnPressButtonStart();

    void DeInit();

    // (���� button == true) - ��� ������ �������
    void Start(bool button);

    void Stop();

    bool IsRunning();

    // ��� �������� � ������ ��������� ���������
    void Restart();

    // ��� ������� ���� �������� ��� ��������� ����� ������. �� ��� ������ �������� ���������
    void OnChangedLengthMemoryChannel();

    // ��� ������� ����� �������� ��� ��������� ������ �������
    void ChangedTrigStartMode();

    // ������� ������ ������������� ��� ������������ �������
    void ClearDataRand();

    void ReadData();

    uint16 ReadLastRecord(Chan::E ch);

    // ���������� ���� �����������
    void ProcessFlagPred();

    // ��������� ����� �������
    void RunAutoSearch();

    // ������� ������ � SCPI, ���� ��� ����������
    void SendDataToSCPI();

    // ���������� �������� ������
    struct InputController
    {
        static void Init();
        static void Write(HPort::E port, uint16 pin, uint16 value);
    };

    // ����� �������� �����, ������� � �������� ����� ������ ������ �� �������. ���� addrRead == 0xffff, �� ����� ������� ����� �������
    extern uint16 addrRead;

    // ������ ������ ������ � ������� data
    bool ReadDataChannel(Chan::E ch, uint8 *data);

    bool ReadDataChannelRand(uint8 *address, uint8 *data);
};


namespace Roller
{
    // ������� ����� �������� ����� ������ �������
    void Prepare();

    // ��� ������� ����� ��������� �������� �� ����� ���������� ��������� ��� ������ ����� ����������� ������, ���� �� ��������� � ���������� ������
    void ReadPoint();

    // ���������� true, ���� ����� �������� ���������� �����
    bool NeedDraw();

    // ���������� ��������� �� ������, ������� ����� ��������
    DataSettings *GetDS();

    // ��������� buffer ������� ��� ���������. width - ������ ���� ��� ���������. ���������� �������, � ������� ����� �������� �����������
    int FillScreenBuffer(Chan::E ch, Buffer &buffer, int width);
};


// 
struct ShiftPoint
{
    enum E
    {
        FAIL,   // �������� ���������� �� �������, ����� ��������� �� ����
        READED  // ��������� ��������� �����
    } type;

    int shift;

    ShiftPoint(E t = FAIL, int s = 0) : type(t), shift(s)
    {
    }
};
