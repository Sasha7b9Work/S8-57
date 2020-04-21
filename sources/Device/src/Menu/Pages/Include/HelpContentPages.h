#pragma once



typedef enum
{
    TypePage_Content,       // На этой странице расположены ссылки на другие страницы
    TypePage_Description    // На этой странице нет ссылок, только описание
} TypePageHelp;


#define MAX_PAGES 3


struct PageHelpContent
{
    TypePageHelp       type;
    uint8              notUsed[3];
    void              *parent;              // Адрес родительской страницы
    // \todo избавиться от этого рудимента
    //pFuncBV            funcNotUsed;         // Оставлено для соместимости с типом Page - нужно для отрисовки.
    const char * const titleHint[2];        // \brief Название страницы на русском и английском языке, а затем содержимое на русском и английском 
                                            // (для случая TypePage_Description)
    void              *pages[MAX_PAGES];    // Массив содержит адреса ссылаемых страниц в случае TypePage_Content
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
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMenu)),
    {
        "Общее описание принципов меню",
        "Кнопки на панели управления имеют два типа нажатия - короткое, длительностью менее 0.5 сек и длинное, длительностьи более 0.5 сек. "
        "Меню представляет собой древовидную структуру элементов. "
        "Главная страница меню открывается коротким либо длинным нажатием кнопки МЕНЮ. "
        "Ко всем элементам главной страницы имеется непосрдественный доступ с помощью нажатия одной из кнопок на панели управления:\n"
        "\"ДИСПЛЕЙ\" - ДИСПЛЕЙ\n"
        "\"КАНАЛ 1\" - КАНАЛ 1\n"
        "\"КАНАЛ 2\" - КАНАЛ 2\n"
        "\"СИНХР.\" - СИНХР\n"
        "\"РАЗВEРТКА\" - РАЗВ\n"
        "\"КУРСОРЫ\" - КУРСОРЫ\n"
        "\"ПАМЯТЬ\" - ПАМЯТЬ\n"
        "\"ИЗМЕРЕНИЯ\" - ИЗМЕР\n"
        "\"СЕРВИС\" - СЕРВИС\n"
        "\"ПОМОЩЬ\" - ПОМОЩЬ\n"
        "В каждом элементе главной страницы собраны настройки соответствующей категории"
    },
    {}
};

static const PageHelpContent helpMenuControls =
{
    TypePage_Description, {},
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMenu)),
    {
        "Описание органов управлениея",
        ""
    },
    {}
};

extern const PageHelpContent helpMain;

static const PageHelpContent helpSCPI =
{
    TypePage_Description, {},
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMain)),
    {
        "Работа с SCPI",
        ""
    },
    {}
};

const PageHelpContent helpMenu =
{
    TypePage_Content, {},
    static_cast<void *>(const_cast<PageHelpContent *>(&helpMain)),
    {
        "Работа с меню",
        "Working with menus"
    },
    {
        static_cast<void *>(const_cast<PageHelpContent *>(&helpMenuCommon)),
        static_cast<void *>(const_cast<PageHelpContent *>(&helpMenuControls))
    }
};

const PageHelpContent helpMain =
{
    TypePage_Content, {},
    0,
    {
        "ПОМОЩЬ",
        "HELP"
    },
    {
        static_cast<void *>(const_cast<PageHelpContent *>(&helpMenu)),
        static_cast<void *>(const_cast<PageHelpContent *>(&helpSCPI))
    }
};
