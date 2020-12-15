#include "defines.h"
#include "device.h"
#include "Display/Primitives.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Hardware/HAL/HAL.h"
#include "Hardware/Memory/RAM.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/HandlersKeys.h"
#include "Menu/Menu.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Osci/Display/DisplayOsci.h"
#include "Settings/Settings.h"
#include "Settings/SettingsNRST.h"
#include <cstdio>


Item *Menu::itemHint = nullptr;
const char *Menu::stringForHint = nullptr;


static const Page *const pages[] =
{
    PageFunction::self,
    PageMeasures::self,
    PageMemory::self,
    PageDisplay::self,
    PageService::self,
    PageTime::self,
    PageTrig::self,
    PageChannelA::self,
    PageChannelB::self,
    nullptr
};


void Menu::Update()
{
    static uint lastModified = 0;
    static bool saved = true;

    while(!BufferButtons::IsEmpty())                // Если есть события клавиатуры
    {
        KeyEvent event = BufferButtons::Extract();  // Извлекаем очередное событие

        if (!Keyboard::KeyIsActive(event.key))      // Если кнопка не разрешена для обработки сейчас:
        {
            continue;                               // Перехдим к следующей
        }

        if (HINT_MODE_ENABLED)                      // Если всклюён режим подсказок
        {
            ProcessButtonForHint(event.key);        // то выводим подсказку для соответствующей кнопки
            continue;                               // и переходим к следующей кнопке
        }

        Handlers::Process(event);                   // То обрабатываем его

        if(event.key != Key::Start)
        {
            DisplayOsci::SetFlagRedraw();
        }

        lastModified = TIME_MS;
        saved = false;
    }

    HAL_BUS::ConfigureToFSMC();

    if (!saved && TIME_MS - lastModified > 5000)
    {
        if(!Device::InModeTester())
        {
            Settings::Save();
            setNRST.Save();
            saved = true;
        }
    }
}


void Menu::ProcessButtonForHint(Key::E button)
{
    if (button == Key::Enter)
    {
        stringForHint =
            "Кнопка МЕНЮ выполняет следующие функции:\n"
            "1. При закрытом меню нажатие либо нажатие с удержанием в течение 0.5с открывает меню.\n"
            "2. При открытом меню удержание кнопки в течение 0.5с закрывает меню.\n"
            "3. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Закрывать\" текущей становится страница предыдущего уровня меню. Если текущей является "
            "корневая страница, меню закрывается.\n"
            "4. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Переключать\" текущей становится страница текущего уровня меню. Если текущая страница "
            "последняя в текущем уровне, происходит переход на предыдущий уровень меню.\n"
            "5. Если меню находится в режиме малых кнопок, то нажатие закрывает страницу.";
    } 
    else if (button == Key::Display)
    {
        stringForHint = "Кнопка ДИСПЛЕЙ открывает меню настроек дисплея.";
    }
    else if (button == Key::Memory)
    {
        stringForHint =
            "1. При настройке \"ПАМЯТЬ\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Меню\" открывает меню работы с памятью.\n"
            "2. При настройке \"ПАМЯТь\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Сохранение\" сохраняет сигнал на флеш-диск.";
    }
    else if (button == Key::Measure)
    {
        stringForHint = "Кнопка ИЗМЕР открывает меню автоматических измерений.";
    }
    else if (button == Key::Service)
    {
        stringForHint = "Кнопка СЕРВИС открывает меню сервисных возможностей.";
    }
    else if (button == Key::Start)
    {
        stringForHint = "Кнопка ПУСК/СTOП запускает и останавливает процесс сбора информации.";
    }
    else if (button == Key::ChannelA)
    {
        stringForHint =
            "1. Кнопка КАНАЛ1 открывает меню настроек канала 1.\n"
            "2. Нажатие и удержание кнопки КАНАЛ1 в течение 0.5с устанавливает смещение канала 1 по вертикали 0В.";
    }
    else if (button == Key::ChannelB)
    {
        stringForHint =
            "1. Кнопка КАНАЛ2 открывает меню настроек канала 2.\n"
            "2. Нажатие и удержание кнопки КАНАЛ2 в течение 0.5с устанавливает смещение канала 2 по вертикали 0В.";
    }
    else if (button == Key::Time)
    {
        stringForHint =
            "1. Кнопка РАЗВ открывает меню настроек развертки.\n"
            "2. Нажатие и удержание кнопки РАЗВ в течение 0.5с устанавливает смещение по горизонтали 0с.";
    }
    else if (button == Key::Trig) //-V2516
    {
        stringForHint =
            "1. Кнопка СИНХР открывает меню настроек синхронизации.\n"
            "2. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\" производит автоматическую "
            "настройку уровня синхронизации.\n"
            "3. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Сброс уровня\" устанавливает уровень "
            "синхронизации 0В.";
    }
}


void Menu::SetAutoHide(bool)
{
    if (Menu::IsShown())
    {
        if (Display::TimeMenuAutoHide() == 0)
        {
            Timer::Disable(TypeTimer::MenuAutoHide);
        }
        else
        {
            Timer::SetAndStartOnce(TypeTimer::MenuAutoHide, OnTimerAutoHide, static_cast<uint>(Display::TimeMenuAutoHide()));
        }
    }
}


void Menu::OnTimerAutoHide()
{
    Menu::Hide();
}


void Menu::ChangeStateFlashDrive()
{
    if(!FDrive::IsConnected())
    {
        if(Menu::OpenedItem() == PageDrive::Manager::self)
        {
            OpenedItem()->HandlerFX(TypePress::Release);
        }
    }
    else if(S_MEM_FLASH_AUTO_CONNECT) //-V2516
    {
        PageMemory::OnOpenClose_Drive_Manager();
    }
}


void Menu::Show()
{
    S_MENU_SHOW = true;

    Menu::SetAutoHide(true);
}


void Menu::Hide()
{
    S_MENU_SHOW = false;

    Menu::SetAutoHide(true);
}


bool Menu::IsShown()
{
    return S_MENU_SHOW && GetMainPage() != nullptr;
}


void Menu::ClosePage(Page *page)
{
    page->OwnData()->funcOnOpenClose(false);

    Page *keeper = const_cast<Page *>(page->Keeper()); //-V2567

    if (keeper)
    {
        keeper->SetPosActItem(0x7f);
    }

    if (page == GetMainPage())
    {
        Menu::Hide();
    }
}


void Menu::CloseIfSubPage(Page *parent, Page *page)
{
    if (page == parent)
    {
        ClosePage(parent);
    }

    if (page->IsSubPage(parent))
    {
        while (page)
        {
            ClosePage(page);
            page = const_cast<Page *>(page->Keeper()); //-V2567
        }
    }
}


Item *Menu::OpenedItem()
{
    return LastOpened(const_cast<Page *>(GetMainPage())); //-V2567
}


static void CloseDebugPages()
{
    const Page *mainPage = Menu::GetMainPage();

    Menu::SetMainPage(PageFunction::self);

    PageMultimeter::EnablePageCalibrate();

    if(Menu::OpenedItem()->ExistKeeper(PageMultimeter::self))
    {
        while (Menu::OpenedPage() != PageMultimeter::self)
        {
            Menu::CloseOpenedItem();
        }
    }

    PageMultimeter::DisablePageCalibrate();

    Menu::SetMainPage(PageService::self);

    PageService::EnablePageDebug();

    if (Menu::OpenedItem()->ExistKeeper(PageDebug::self))
    {
        while (Menu::OpenedPage() != PageService::self)
        {
            Menu::CloseOpenedItem();
        }
    }

    PageService::DisablePageDebug();

    Menu::SetMainPage(mainPage);
}


void Menu::Init()
{
    PageMultimeter::Init();
    PageFreqMeter::Init();
    PageTester::Init();
    PageDisplay::Settings::Colors::Init();

    if (static_cast<Page *>(LastOpened(const_cast<Page *>(PageFunction::self))) == PageMultimeter::self) //-V1027 //-V2567
    {
        ClosePage(const_cast<Page *>(PageMultimeter::self)); //-V2567
    }

    CloseAllBadOpenedPages();

    CloseDebugPages();
}


void Menu::CloseOpenedItem()
{
    Item *item = OpenedItem();

    if (item->Is(TypeItem::Page))
    {
        ClosePage(static_cast<Page *>(item)); //-V1027
    }
    else
    {
        item->Open(false);
    }
}


Page *Menu::OpenedPage()
{
    Item *item = OpenedItem();

    return (item->IsPage()) ? static_cast<Page *>(item) : nullptr;
}

Item *Menu::LastOpened(Page *page)
{
    Item *result = &Item::empty;

    if (page)
    {
        if (page->CurrentItemIsOpened())
        {
            int8 posActItem = page->PosCurrentItem();
            Item *item = page->GetItem(posActItem);

            if (page->GetItem(posActItem)->Is(TypeItem::Page))
            {
                result = LastOpened(static_cast<Page *>(item));
            }
            else
            {
                result = item;
            }
        }
        else
        {
            result = page;
        }
    }

    return result;
}


Item *Menu::CurrentItem()
{
    Item *result = OpenedItem();

    int8 pos = static_cast<Page *>(result)->PosCurrentItem();

    if (result->Is(TypeItem::Page) && pos != 0x7f)
    {
        result = static_cast<Page *>(result)->GetItem(pos);
    }

    return result;
}


void Menu::DrawHintItem(int x, int y, int width)
{
    if (itemHint)
    {
        static pString names[TypeItem::Count] =
        {
            "",                   // Item_None
            "",                   // Item::Type::Choice
            "Кнопка",             // Item::Type::Button
            "Страница",           // Item::Type::Page
            "Регулятор",          // Item::Type::Governor
            "",                   // Item::Type::GovernorColor
            "Кнопка"              // Item::Type::DrawButton
        };

        Page *item = static_cast<Page *>(itemHint);

        const int SIZE = 100;
        char title[SIZE];
        std::snprintf(title, SIZE, "%s \"%s\"", names[itemHint->data->type], item->data->title);

        Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);

        y = Text(item->data->hint).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);

        if (item->Is(TypeItem::GraphButton))
        {
            reinterpret_cast<GraphButton *>(item)->DrawHints(x, y, width);   
        }
    }
}


int Menu::Y0()
{
    return Display::HEIGHT - 25;
}


void Menu::Draw()
{
    DFont::Set(DTypeFont::_8);

    if (Menu::IsShown())
    {
        Item *item = OpenedItem();

        if (item)
        {
            if (!item->Is(TypeItem::Page))
            {
                item = const_cast<Item *>(static_cast<const Item *>(item->Keeper())); //-V2567
            }

            if (item)
            {
                item->Draw(0, Y0(), true);
            }
        }
    }

    DrawHint();
}


void Menu::DrawHint()
{
    if (HINT_MODE_ENABLED)
    {
        int x = 1;
        int y = 0;
        int width = 318;

        //Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
        //       LANG_RU ? "Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о её назначении. "
        //       "Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с." :
        //       "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
        //       "To disable this mode, press the button HELP and hold it for 0.5s.",
        //       Color::BACK, Color::FILL);

        Text("Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о её назначении. "
            "Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с."
        ).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::FILL);

        y += 49;
        if (stringForHint)
        {
            Text(stringForHint).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::WHITE);
        }
        else if (itemHint) //-V2516
        {
            DrawHintItem(x, y, width);
        }
    }
}


void Menu::SetItemForHint(const Item *item)
{
    stringForHint = nullptr;
    itemHint = const_cast<Item *>(item); //-V2567
}


const Item *Menu::ItemUnderFunctionalKey(Key::E key)
{
    const Item *result = &Item::empty;

    if (Key(key).IsFunctional())
    {
        Item *item = Menu::OpenedItem();

        if (item == nullptr)
        {
        }
        else if (item->Is(TypeItem::Page))
        {
            result = (static_cast<Page *>(item))->ItemForFuncKey(key);
        }
        else
        {
            const Page *parent = item->Keeper();
            if (parent && parent->ItemForFuncKey(key) == item && item->IsActive())
            {
                result = item;
            }
        }
    }

    return result;
}


void Menu::CloseAllBadOpenedPages()
{
    Page *opened = static_cast<Page *>(LastOpened(const_cast<Page *>(PageFunction::self))); //-V1027 //-V2567

    CloseIfSubPage(const_cast<Page *>(PageMultimeter::self), opened); //-V2567
    CloseIfSubPage(const_cast<Page *>(PageRecorder::self), opened); //-V2567
    CloseIfSubPage(const_cast<Page *>(PageTester::self), opened); //-V2567
    CloseIfSubPage(const_cast<Page *>(PageFreqMeter::self), opened); //-V2567
    CloseIfSubPage(const_cast<Page *>(PageFFT::self), opened); //-V2567
    CloseIfSubPage(const_cast<Page *>(PageMemory::self), opened); //-V2567

    opened = static_cast<Page *>(LastOpened(const_cast<Page *>(PageMemory::self))); //-V2567

    CloseIfSubPage(const_cast<Page *>(PageROM::self), opened); //-V2567
    CloseIfSubPage(const_cast<Page *>(PageRAM::self), opened); //-V2567
}


int8 &Menu::Position::ActItem(PageName::E name)
{
    return S_MENU_POS_ACT_ITEM(name);
}


Page *Menu::PageFromName(PageName::E name) //-V2506
{
    for(int i = 0; true; i++)
    {
        if(pages[i] == nullptr)
        {
            break;
        }
        else if(pages[i]->GetName() == name) //-V2516
        {
            return const_cast<Page *>(pages[i]); //-V2567
        }
    }

    return nullptr;
}


void Menu::SetMainPage(const Page *page)
{
    S_MENU_MAIN_PAGE = page->GetName();
}


const Page *Menu::GetMainPage()
{
    return PageFromName(S_MENU_MAIN_PAGE);
}
