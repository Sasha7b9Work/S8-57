#include "defines.h"
#include "Menu/Menu.h"
#include "Menu/BufferButtons.h"
#include "Hardware/Timer.h"
#include "Menu/MenuTriggers.h"
#include "Keyboard/HandlersKeys.h"
#include "device.h"
#include "FlashDrive/FlashDrive.h"
#include "Settings/SettingsMemory.h"
#include "Settings/Settings.h"
#include "Pages/Include/PageMemory.h"
#include "Pages/Include/PageChannels.h"
#include "Pages/Include/PageHelp.h"
#include "Pages/Include/PageMeasures.h"
#include "Pages/Include/PageDisplay.h"
#include "Pages/Include/PageFunction.h"
#include "Hardware/Beeper.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_Settings.h"
#include "Utils/Math.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include <cstdio>

#include "Utils/Debug.h"
#include "Hardware/HAL/HAL.h"
#include "Keyboard/DecoderDevice.h"
#include "Utils/Dictionary.h"


using namespace Display::Primitives;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
   Control    *Menu::itemUnderKey = 0;
const char    *Menu::stringForHint = 0;
   Control    *Menu::itemHint = 0;
/// true, если нужно сохранять копию экрана на флешку
static bool needSaveScreen = false;

/// Элементы управления, назначенные в данный момент соответствующим кнопкам
static Control *underButton[Key::Number];

/// Последний открытый контрол на дереве странице page
static Control *LastOpened(Page *page);
/// Обработка события таймера автоматического сокрытия меню
static void OnTimerAutoHide();
/// Функция, которая отключит вывод строки навигации меню
static void OnTimerStrNaviAutoHide();

static void ProcessButtonForHint(Key::E button);

static void ResetItemsUnderButton();
/// Возвращает true, если данная кнопка обрабатыватся в данном режиме
static bool IsProcessed(const KeyEvent *event);
/// Время последнего нажатия кнопки. Нужно для того, чтобы периодически сохранять настройки
static uint timeLastPressedButton = MAX_UINT;
/// Текущая главная страница
static PageBase *mainPage = nullptr;

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
static void CreateFileName(char name[256])
{
    std::strcpy(name, "screen.bmp");
}

static int numRow = -1;

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SaveRow(int row)
{
    numRow = row;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ReadRow(uint8 row)
{
    numRow = -1;

    uint8 buffer[] = { Command::Screen, row };

    HAL::FSMC::WriteToPanel(buffer, 2);

    while (numRow == -1)
    {
        uint8 data = 0;
        HAL::FSMC::WriteToPanel(&data, 1);
        Decoder::Update();
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void SaveScreenToFlash()
{
    if (!FDrive::IsConnected())
    {
        return;
    }

#pragma pack(1)
    struct BITMAPFILEHEADER
    {
        char    type0;      // 0
        char    type1;      // 1
        uint    size;       // 2
        uint16  res1;       // 6
        uint16  res2;       // 8
        uint    offBits;    // 10
    }
    bmFH =
    {
        0x42,
        0x4d,
        14 + 40 + 1024 + 320 * 240,
        0,
        0,
        14 + 40 + 1024
    };

    // 14

    struct BITMAPINFOHEADER
    {
        uint    size;           // 14
        int     width;          // 18
        int     height;         // 22
        uint16  planes;         // 26
        uint16  bitCount;       // 28
        uint    compression;    // 30
        uint    sizeImage;      // 34
        int     xPelsPerMeter;  // 38
        int     yPelsPerMeter;  // 42
        uint    clrUsed;        // 46
        uint    clrImportant;   // 50
        //uint    notUsed[15];
    }
    bmIH =
    {
        40, // size;
        320,// width;
        240,// height;
        1,  // planes;
        8,  // bitCount;
        0,  // compression;
        0,  // sizeImage;
        0,  // xPelsPerMeter;
        0,  // yPelsPerMeter;
        0,  // clrUsed;
        0   // clrImportant;
    };

    // 54
#pragma pack(4)

    StructForWrite structForWrite;

    char fileName[255];

    CreateFileName(fileName);

    FDrive::OpenNewFileForWrite(fileName, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmFH), 14, &structForWrite);

    FDrive::WriteToFile((uint8 *)(&bmIH), 40, &structForWrite);

    uint8 buffer[320 * 3] = { 0 };

    typedef struct tagRGBQUAD
    {
        uint8    blue;
        uint8    green;
        uint8    red;
        uint8    rgbReserved;
    } RGBQUAD;

    RGBQUAD colorStruct;

    for (int i = 0; i < 32; i++)
    {
        uint color = COLOR(i);
        colorStruct.blue = (uint8)((float)B_FROM_COLOR(color));
        colorStruct.green = (uint8)((float)G_FROM_COLOR(color));
        colorStruct.red = (uint8)((float)R_FROM_COLOR(color));
        colorStruct.rgbReserved = 0;
        ((RGBQUAD*)(buffer))[i] = colorStruct;
    }

    for (int i = 0; i < 4; i++)
    {
        FDrive::WriteToFile(buffer, 256, &structForWrite);
    }

    uint8 pixels[320];

    Decoder::SetBufferForScreenRow(pixels);

    for (int row = 239; row >= 0; row--)
    {
        ReadRow((uint8)row);

        FDrive::WriteToFile(pixels, 320, &structForWrite);
    }

    FDrive::CloseFile(&structForWrite);

    Display::FuncOnWaitStart(DICT(DFileSaved), false);

    uint timeStart = TIME_MS;

    while (TIME_MS - timeStart < 1500)
    {

    };
    
    Display::FuncOnWaitStop();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Update()
{
    while(!BufferButtons::IsEmpty())
    {
        timeLastPressedButton = TIME_MS;

        KeyEvent event = BufferButtons::Extract();

        if (HINT_MODE_ENABLED)
        {
            ProcessButtonForHint(event.key);
            continue;
        }

        if (!Menu::IsShown())
        {
            if(event.type == TypePress::Release && TriggerDebugConsole::Update(event.key))
            {
                continue;
            }
        }
        if(IsProcessed(&event))
        {
            Handlers::Process(event);
        }
    }

    if (needSaveScreen)
    {
        SaveScreenToFlash();

        needSaveScreen = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SaveScreenToDrive()
{
    needSaveScreen = true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static bool IsProcessed(const KeyEvent *event)
{
    Key::E key = event->key;
    TypePress::E type = event->type;

    if (Device::State::InModeTester())
    {
        if (Key(key).IsControlSignal() || Key(key).IsFunctional() || (key == Key::Enter && !TypePress(type).IsLong()))
        {
            return true;
        }
        return false;
    }
    
    if (Device::State::InModeMultimeter())
    {
        if (Key(key).IsFunctional() ||          // мультиметр реагирует на функциональные кнопки
            Key(key).IsArrow() ||               // на стрелки
            (key == Key::Enter && !TypePress(type).IsLong()))
        {
            return true;
        }
        return false;
    }

    return true;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ProcessButtonForHint(Key::E button)
{
    if (button == Key::Enter)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка МЕНЮ выполняет следующие функции:\n"
            "1. При закрытом меню нажатие либо нажатие с удержанием в течение 0.5с открывает меню.\n"
            "2. При открытом меню удержание кнопки в течение 0.5с закрывает меню.\n"
            "3. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Закрывать\" текущей становится страница предыдущего уровня меню. Если текущей является "
            "корневая страница, меню закрывается.\n"
            "4. При настройке \"СЕРВИС\x99Режим кн МЕНЮ\x99Переключать\" текущей становится страница текущего уровня меню. Если текущая страница "
            "последняя в текущем уровне, происходит переход на предыдущий уровень меню.\n"
            "5. Если меню находится в режиме малых кнопок, то нажатие закрывает страницу."
            :
        "МЕНЮ button performs the following functions:\n"
            "1. At the closed menu pressing or pressing with deduction during 0.5s opens the Menu::\n"
            "2. At the open menu deduction of the button during 0.5s closes the Menu::\n"
            "3. At control \"SERVICE\x99Mode btn MENU\x99\x43lose\" current becomes the page of the previous level of the Menu:: If the root page is "
            "current, the menu is closed.\n"
            "4. At control \"SERVICE\x99Mode btn MENU\x99Toggle\" current becomes the page of the current level of the Menu:: If the current page the "
            "last in the current level, happens transition to the previous level of the Menu::\n"
            "5. If the menu is in the mode of small buttons, pressing closes the page.";

    } 
    else if (button == Key::Display)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка ДИСПЛЕЙ открывает меню настроек дисплея."
            :
            "ДИСПЛЕЙ button opens the display settings Menu::";
    }
    else if (button == Key::Memory)
    {
        Menu::stringForHint = LANG_RU ?
            "1. При настройке \"ПАМЯТЬ\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Меню\" открывает меню работы с памятью.\n"
            "2. При настройке \"ПАМЯТь\x99ВНЕШН ЗУ\x99Реж кн ПАМЯТЬ\x99Сохранение\" сохраняет сигнал на флеш-диск."
            :
            "1. When setting \"ПАМЯТЬ-EXT\x99STORAGE\x99Mode btn MEMORY\x99Menu\" opens a menu of memory\n"
            "2. When setting \"ПАМЯТЬ-EXT\x99STORAGE\x99Mode btn MEMORY\x99Save\" saves the signal to the flash drive";
    }
    else if (button == Key::Measure)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка ИЗМЕР открывает меню автоматических измерений."
            :
            "ИЗМЕР button opens a menu of automatic measurements.";
    }
    else if (button == Key::Service)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка СЕРВИС открывает меню сервисных возможностей."
            :
            "СЕРВИС button opens a menu of service options.";
    }
    else if (button == Key::Start)
    {
        Menu::stringForHint = LANG_RU ?
            "Кнопка ПУСК/СTOП запускает и останавливает процесс сбора информации."
            :
            "ПУСК/СTOП button starts and stops the process of gathering information.";
    }
    else if (button == Key::ChannelA)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ1 открывает меню настроек канала 1.\n"
            "2. Нажатие и удержание кнопки КАНАЛ1 в течение 0.5с устанавливает смещение канала 1 по вертикали 0В."
            :
            "1. КАНАЛ1 button opens the settings menu of the channel 1.\n"
            "2. Pressing and holding the button КАНАЛ1 for 0.5c for the offset of the vertical channel 1 0V.";
    }
    else if (button == Key::ChannelB)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка КАНАЛ2 открывает меню настроек канала 2.\n"
            "2. Нажатие и удержание кнопки КАНАЛ2 в течение 0.5с устанавливает смещение канала 2 по вертикали 0В."
            :
            "1. КАНАЛ2 button opens the settings menu of the channel 2.\n"
            "2. Pressing and holding the button КАНАЛ2 for 0.5c for the offset of the vertical channel 2 0V.";
    }
    else if (button == Key::Time)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка РАЗВ открывает меню настроек развертки.\n"
            "2. Нажатие и удержание кнопки РАЗВ в течение 0.5с устанавливает смещение по горизонтали 0с."
            :
            "1. РАЗВ button open the settings menu sweep.\n"
            "2. Pressing and holding the button РАЗВ for 0.5s Is the offset horizontal 0s.";
    }
    else if (button == Key::Trig)
    {
        Menu::stringForHint = LANG_RU ?
            "1. Кнопка СИНХР открывает меню настроек синхронизации.\n"
            "2. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Автоуровень\" производит автоматическую "
            "настройку уровня синхронизации.\n"
            "3. Нажатие и удержание в течение 0.5с кнопки СИНХР при настройке \"СЕРВИС\x99Реж длит СИНХР\x99Сброс уровня\" устанавливает уровень "
            "синхронизации 0В."
            :
            "1. СИНХР button opens a menu settings synchronization.\n"
            "2. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99\x41utolevel\" automatically adjust the "
            "trigger level.\n"
            "3. Pressing and holding the button СИНХР for 0.5s when setting \"SERVICE\x99Mode long TRIG\x99SReset trig level\" sets the trigger "
            "level 0V.";
    }
    else
    {
        // здесь ничего не делаем
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetAutoHide(bool)
{
    if(!Menu::IsShown())
    {
        return;
    }
    if(Display::TimeMenuAutoHide() == 0)
    {
        Timer::Disable(Timer::Type::MenuAutoHide);
    }
    else
    {
        Timer::SetAndStartOnce(Timer::Type::MenuAutoHide, OnTimerAutoHide, (uint)Display::TimeMenuAutoHide());
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerAutoHide()
{
    Menu::Show(false);
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::TemporaryEnableStrNavi()
{
    if (SHOW_STRING_NAVI_TEMP)
    {
        SHOW_STRING_NAVIGATION = 1;                                            // Устанавливаем признак того, что надо выводить строку навигации меню
        Timer::SetAndStartOnce(Timer::Type::StrNaviAutoHide, OnTimerStrNaviAutoHide, 3000); // и запускаем таймер, который его отключит
    }
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
static void OnTimerStrNaviAutoHide()
{
    SHOW_STRING_NAVIGATION = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ChangeStateFlashDrive()
{
    if(!FDrive::IsConnected())
    {
        if(GetNameOpenedPage() == Page::Name::SB_Memory_Drive_Manager)
        {
            ((Page *)OpenedItem())->ShortPressOnItem(0);
        }
    }
    else if(FLASH_AUTOCONNECT)
    {
        PageMemory::OnPress_Drive_Manager();
    }
    else
    {
        // остальные ситуации не обрабатываются
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Show(bool show)
{
    set.menu_show = show;

    if (show)
    {
        Menu::TemporaryEnableStrNavi();
    }
    Menu::SetAutoHide(true);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsShown()
{
    return set.menu_show && MainPage() != nullptr;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ClosePage(PageBase *page)
{
    if (IS_PAGE_SB(page))
    {
        SMALL_BUTTON_FROM_PAGE(page, 0)->funcOnPress();
    }
    else
    {
        ((Page *)page)->funcOnEnterExit(false);
    }

    Page *keeper = (Page *)KEEPER(page);

    if (keeper)
    {
        keeper->SetPosActItem(0x7f);
    }

    if (page == (PageBase *)Menu::MainPage())    // -V1027
    {
        Menu::Show(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
// Закрыть parent, если он является хранителем page
static void CloseIfSubPage(PageBase *parent, PageBase *page)
{
    if (page == parent)
    {
        ClosePage(parent);
    }

    while (KEEPER(page))
    {
        if (KEEPER(page) == parent)
        {
            ClosePage(parent);
        }

        page = KEEPER(page);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Init()
{
    PageDisplay::Init();
    PageFunction::PageMultimeter::Init();

    if ((PageBase *)LastOpened((Page *)PageFunction::pointer) == PageFunction::PageMultimeter::pointer)
    {
        ClosePage((PageBase *)PageFunction::PageMultimeter::pointer);
    }

    PageBase *opened = (PageBase *)LastOpened((Page *)PageFunction::pointer);

    CloseIfSubPage((PageBase *)PageFunction::PageMultimeter::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageRecorder::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageTester::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageFrequencyCounter::pointer, opened);
    CloseIfSubPage((PageBase *)PageFunction::PageFFT::pointer, opened);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::CloseOpenedItem()
{
    Control *item = OpenedItem();

    if (IS_PAGE(item))
    {
        ClosePage((PageBase *)item);
    }
    else
    {
        item->Open(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    return LastOpened((Page *)MainPage());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Page::Name::E Menu::GetNameOpenedPage()
{
    const Page *opened = (const Page *)OpenedItem();

    if (opened == nullptr)
    {
        return Page::Name::NoPage;
    }

    return opened->GetName();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Menu::OpenedPage()
{
    return (PageBase *)OpenedItem(); //-V1027
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static Control *LastOpened(Page *page)
{
    if (page == nullptr)
    {
        return nullptr;
    }

    if (page->CurrentItemIsOpened())
    {
        int8 posActItem = page->PosCurrentItem();
        Control *item = page->Item(posActItem);

        if (page->Item(posActItem)->IsPage())
        {
            return LastOpened((Page *)item);
        }
        else
        {
            return item;
        }
    }
    return page;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::CurrentItem()
{
    Control *opened = OpenedItem();

    int8 pos = ((const Page *)opened)->PosCurrentItem();

    if (opened->IsPage() && pos != 0x7f)
    {
        return ((const Page *)opened)->Item(pos);
    }

    return (Control *)opened;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void DrawHintItem(int x, int y, int width)
{
    if (!Menu::itemHint)
    {
        return;
    }

    DEF_STRUCT(StructName, pString) names[Control::Type::Number][2] =
    {
        {"",                ""},                    // Item_None
        {"",                ""},                    // Control::Type::Choice
        {"Кнопка",          "Button"},              // Control::Type::Button
        {"Страница",        "Page"},                // Control::Type::Page
        {"Регулятор",       "Governor"},            // Control::Type::Governor
        {"",                ""},                    // Control::Type::Time
        {"",                ""},                    // Control::Type::GovernorColor
        {"",                ""},                    // Control::Type::ChoiceReg
        {"Кнопка",          "Button"},              // Control::Type::DrawButton
        {"Выбор параметра", "Choice parameter"},    // Control::Type::ChoiceParameter
    };

    Language::E lang = LANG;
    Page *item = (Page *)Menu::itemHint;

    const int SIZE = 100;
    char title[SIZE];
    std::snprintf(title, SIZE, "%s \"%s\"", names[Menu::itemHint->type][lang].val, item->titleHint[lang]);

    if (item->type == Control::Type::DrawButton)
    {
        y -= 9;
    }

    Text(title).DrawInCenterRectAndBoundIt(x, y, width, 15, Color::BACK, Color::FILL);

    y = Text(item->titleHint[2 + lang]).DrawInBoundedRectWithTransfers(x, y + 15, width, Color::BACK, Color::FILL);

    if (item->type == Control::Type::DrawButton)
    {
        ((SButton*)item)->DrawHints(x, y, width);   // -V1027
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
int Menu::Y()
{
    return Display::HEIGHT - Item::HEIGHT - 2;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Draw()
{
    if (Menu::IsShown())
    {
        ResetItemsUnderButton();
        Control *item = OpenedItem();
        if (Menu::IsShown())
        {
            if (IS_PAGE(item))
            {
                item->Draw(0, Y(), true);
            }
            else
            {
                ((Page *)KEEPER(item))->Draw(0, Y(), true);
            }
        }
    }

    if (HINT_MODE_ENABLED)
    {
        int x = 1;
        int y = 0;
        int width = 318;
        if (Menu::IsShown())
        {
            width = Menu::IsMinimize() ? 289 : 220;
        }

        //Painter::DrawTextInBoundedRectWithTransfers(x, y, width,
        //       LANG_RU ? "Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о её назначении. "
        //       "Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с." :
        //       "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
        //       "To disable this mode, press the button HELP and hold it for 0.5s.",
        //       Color::BACK, Color::FILL);

        Text(
            LANG_RU ? 
            "Включён режим подсказок. В этом режиме при нажатии на кнопку на экран выводится информация о её назначении. "
            "Чтобы выключить этот режим, нажмите кнопку ПОМОЩЬ и удерживайте её в течение 0.5с."
            :
            "Mode is activated hints. In this mode, pressing the button displays the information on its purpose. "
            "To disable this mode, press the button HELP and hold it for 0.5s."
        ).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::FILL);

        y += LANG_RU ? 49 : 40;
        if (Menu::stringForHint)
        {
            Text(Menu::stringForHint).DrawInBoundedRectWithTransfers(x, y, width, Color::BACK, Color::WHITE);
        }
        else if (Menu::itemHint)
        {
            DrawHintItem(x, y, width);
        }
        else
        {
            // ничего не делаем
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void ResetItemsUnderButton()
{
    for (int i = 0; i < Key::Number; i++)
    {
        Menu::SetItemUnderButton((Key::E)i, 0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsMinimize()
{
    return IS_PAGE_SB(Menu::OpenedItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetItemForHint(void *item)
{
    Menu::stringForHint = 0;
    Menu::itemHint = (Control *)item;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ButtonEvent(KeyEvent event)
{
    /// \todo На звуке иногда виснет при длительном удержании кнопки смещения. Вызвано ошибками при передаче данных
    Beeper::Beep(event.type);
    BufferButtons::Push(event);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SaveSettings()
{
    if((timeLastPressedButton != MAX_UINT) && (TIME_MS - timeLastPressedButton > 5000))
    {
        timeLastPressedButton = MAX_UINT;
        if(!Device::State::InModeTester())
        {
            Settings::Save();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ReleaseFunctionalButton(Key::E key)
{
    if(Menu::IsShown())
    {
        Control *control = Menu::ItemUnderButton(key);
        if(control)
        {
            control->ShortPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::LongFunctionalButton(Key::E key)
{
    if(Menu::IsShown())
    {
        Control *control = Menu::ItemUnderButton(key);
        if(control)
        {
            control->LongPress();
        }
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::ItemUnderButton(Key::E button)
{
    return underButton[button];
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetItemUnderButton(Key::E button, Control *control)
{
    underButton[button] = control;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Menu::MainPage()
{
    return mainPage;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::SetMainPage(PageBase *page)
{
    mainPage = page;
}
