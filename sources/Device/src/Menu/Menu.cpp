#include "defines.h"
#include "device.h"
#include "Transceiver.h"
#include "Display/Primitives.h"
#include "FlashDrive/FlashDrive.h"
#include "Hardware/Timer.h"
#include "Keyboard/BufferButtons.h"
#include "Keyboard/HandlersKeys.h"
#include "Menu/Menu.h"
#include "Settings/Settings.h"
#include <cstdio>


const Page    *Menu::mainPage = nullptr;

/// Итем, для которого нужно выводить подсказку
static Item   *itemHint = nullptr;
/// Строка подсказки, которую надо выводить в случае включённого режима подсказок.
const char    *stringForHint = nullptr;
/// Нужно для того, чтобы периодически сохранять настройки
static uint timeLastKeyboardEvent = MAX_UINT;
/// Последний открытый контрол на дереве странице page
static Item *LastOpened(Page *page);
/// Обработка события таймера автоматического сокрытия меню
static void OnTimerAutoHide();

static void ProcessButtonForHint(Key::E button);
/// Написать подсказку
static void DrawHint();



void Menu::ProcessingAllKeyboardEvents()
{
    while(!BufferButtons::IsEmpty())                // Если есть события клавиатуры
    {
        timeLastKeyboardEvent = TIME_MS;            // то сохраняем время последнего нажатия, чтобы знать, когда сохранить настройки

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
    }
}


static void ProcessButtonForHint(Key::E button)
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
    else if (button == Key::Trig)
    {
        stringForHint =
            "1. Кнопка СИНХР открывает меню настроек синхронизации.\n"
            "2. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\" производит автоматическую "
            "настройку уровня синхронизации.\n"
            "3. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Сброс уровня\" устанавливает уровень "
            "синхронизации 0В.";
    }
    else
    {
        // здесь ничего не делаем
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


static void OnTimerAutoHide()
{
    Menu::Hide();
}


void Menu::ChangeStateFlashDrive()
{
    if(!FDrive::IsConnected())
    {
        if(Menu::OpenedItem() == PageDrive::PageManager::self)
        {
            OpenedItem()->HandlerFX(TypePress::Release);
        }
    }
    else if(set.mem.flashAutoConnect)
    {
        PageMemory::OnOpenClose_Drive_Manager();
    }
    else
    {
        // остальные ситуации не обрабатываются
    }
}


void Menu::Show()
{
    set.menu.show = true;

    Menu::SetAutoHide(true);
}


void Menu::Hide()
{
    set.menu.show = false;

    Menu::SetAutoHide(true);
}


bool Menu::IsShown()
{
    return set.menu.show && mainPage != nullptr;
}


static void ClosePage(Page *page)
{
    page->OwnData()->funcOnOpenClose(false);

    Page *keeper = const_cast<Page *>(page->Keeper());

    if (keeper)
    {
        keeper->SetPosActItem(0x7f);
    }

    if (page == Menu::mainPage)    
    {
        Menu::Hide();
    }
}


// Закрыть parent, если он является хранителем page
static void CloseIfSubPage(Page *parent, Page *page)
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
            page = const_cast<Page *>(page->Keeper());
        }
    }
}


void Menu::Init()
{
    PageMultimeter::Init();
    PageFreqMeter::Init();

    if (static_cast<Page *>(LastOpened(const_cast<Page *>(PageFunction::self))) == PageMultimeter::self) //-V1027
    {
        ClosePage(const_cast<Page *>(PageMultimeter::self));
    }

    Page *opened = static_cast<Page *>(LastOpened(const_cast<Page *>(PageFunction::self))); //-V1027

    CloseIfSubPage(const_cast<Page *>(PageMultimeter::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageRecorder::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageTester::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageFreqMeter::self), opened);
    CloseIfSubPage(const_cast<Page *>(PageFFT::self), opened);
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


Item *Menu::OpenedItem()
{
    return LastOpened(const_cast<Page *>(Menu::mainPage));
}


static Item *LastOpened(Page *page)
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


static void DrawHintItem(int x, int y, int width)
{
    if (itemHint)
    {
        //DEF__STRUCT(StructName, pString) names[Item::Type::Number] =
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
    if (Menu::IsShown())
    {
        Item *item = OpenedItem();

        if (item)
        {
            if (!item->Is(TypeItem::Page))
            {
                item = const_cast<Item *>(static_cast<const Item *>(item->Keeper()));
            }

            if (item)
            {
                item->Draw(0, Y0(), true);
            }
        }
    }

    DrawHint();
}


static void DrawHint()
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
        else if (itemHint)
        {
            DrawHintItem(x, y, width);
        }
        else
        {
            // ничего не делаем
        }
    }
}


void Menu::SetItemForHint(const Item *item)
{
    stringForHint = nullptr;
    itemHint = const_cast<Item *>(item);
}


void Menu::SaveSettings()
{
    if((timeLastKeyboardEvent != MAX_UINT) && (TIME_MS - timeLastKeyboardEvent > 5000))
    {
        timeLastKeyboardEvent = MAX_UINT;
        if(!Device::State::InModeTester())
        {
            Settings::Save();
        }
    }
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
