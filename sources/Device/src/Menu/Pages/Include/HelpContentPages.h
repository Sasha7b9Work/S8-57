// 2023/11/22 13:44:49 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once


typedef enum
{
    TypePage_Content,       // �� ���� �������� ����������� ������ �� ������ ��������
    TypePage_Description    // �� ���� �������� ��� ������, ������ ��������
} TypePageHelp;


#define MAX_PAGES 3


struct PageHelpContent
{
    TypePageHelp       type;
    uint8              notUsed[3];
    void              *parent;              // ����� ������������ ��������
    // \todo ���������� �� ����� ���������
    //pFuncBV            funcNotUsed;         // ��������� ��� ������������ � ����� Page - ����� ��� ���������.
    const char * const titleHint[2];        // \brief �������� �������� �� ������� � ���������� �����, � ����� ���������� �� ������� � ���������� 
                                            // (��� ������ TypePage_Description)
    void              *pages[MAX_PAGES];    // ������ �������� ������ ��������� ������� � ������ TypePage_Content
    pString Title() const
    {
        return titleHint[0];
    }
    pString Hint() const
    {
        return titleHint[1];
    }
};

extern const PageHelpContent helpMenu;

static const PageHelpContent helpMenuCommon =
{
    TypePage_Description, {},
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMenu)), //-V2567
    {
        "����� �������� ��������� ����",
        "������ �� ������ ���������� ����� ��� ���� ������� - ��������, ������������� ����� 0.5 ��� � �������, ������������� ����� 0.5 ���. "
        "���� ������������ ����� ����������� ��������� ���������. "
        "������� �������� ���� ����������� �������� ���� ������� �������� ������ ����. "
        "�� ���� ��������� ������� �������� ������� ���������������� ������ � ������� ������� ����� �� ������ �� ������ ����������:\n"
        "\"�������\" - �������\n"
        "\"����� 1\" - ����� 1\n"
        "\"����� 2\" - ����� 2\n"
        "\"�����.\" - �����\n"
        "\"����E����\" - ����\n"
        "\"�������\" - �������\n"
        "\"������\" - ������\n"
        "\"���������\" - �����\n"
        "\"������\" - ������\n"
        "\"������\" - ������\n"
        "� ������ �������� ������� �������� ������� ��������� ��������������� ���������"
    },
    {}
};

static const PageHelpContent helpMenuControls =
{
    TypePage_Description, {},
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMenu)), //-V2567
    {
        "�������� ������� �����������",
        ""
    },
    {}
};

extern const PageHelpContent helpMain;

static const PageHelpContent helpSCPI =
{
    TypePage_Description, {},
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMain)), //-V2567
    {
        "������ � SCPI",
        ""
    },
    {}
};

const PageHelpContent helpMenu =
{
    TypePage_Content, {},
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMain)), //-V2567
    {
        "������ � ����",
        "Working with menus"
    },
    {
        static_cast<void *>(const_cast<PageHelpContent *>(&helpMenuCommon)), //-V2567
        static_cast<void *>(const_cast<PageHelpContent *>(&helpMenuControls)) //-V2567
    }
};

const PageHelpContent helpMain =
{
    TypePage_Content, {},
    0,
    {
        "������",
        "HELP"
    },
    {
        static_cast<void *>(const_cast<PageHelpContent *>(&helpMenu)), //-V2567
        static_cast<void *>(const_cast<PageHelpContent *>(&helpSCPI)) //-V2567
    }
};
