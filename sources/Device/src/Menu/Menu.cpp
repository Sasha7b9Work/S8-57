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


using namespace Display::Primitives;
using namespace Osci::Settings;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
       uint    Menu::timeLastPressedButton = MAX_UINT;
        Key::E Menu::shortPressureButton = Key::None;
        Key::E Menu::longPressureButton = Key::None;
        Key::E Menu::pressButton = Key::None;
        Key::E Menu::releaseButton = Key::None;
   Control    *Menu::itemUnderKey = 0;
    pFuncVV    Menu::funcAterUpdate = 0;
const char    *Menu::stringForHint = 0;
   Control    *Menu::itemHint = 0;

/// Элементы управления, назначенные в данный момент соответствующим кнопкам
static Control *underButton[Key::Number];

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
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

    if (funcAterUpdate)
    {
        funcAterUpdate();
        funcAterUpdate = 0;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsProcessed(const KeyEvent *event)
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
void Menu::ShortPressureButton(Key::E button)
{
    if (!HINT_MODE_ENABLED)
    {
        if (button == Key::Memory && FDrive::IsConnected() && MODE_BTN_MEMORY_IS_SAVE)
        {
            NEED_SAVE_TO_FLASHDRIVE = 1;
        }

        shortPressureButton = button;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::LongPressureButton(Key::E button)
{
    if (!HINT_MODE_ENABLED)
    {
        longPressureButton = button;
        NEED_FINISH_DRAW = 1;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessButtonForHint(Key::E button)
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
        shortPressureButton = button;
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
char *Menu::StringNavigation(char buffer[100])
{
    buffer[0] = 0;
    const char * titles[10] = {0};
    int numTitle = 0;
    Control *item = OpenedItem();
    if(IsMainPage(item))
    {
        return 0;
    }
    while(!IsMainPage(item))
    {
        titles[numTitle++] = item->Title().CString();
        item = (Control *)item->keeper;                     // -V1027
    }
    for(int i = 9; i >= 0; i--)
    {
        if(titles[i])
        {
            std::strcat(buffer, titles[i]);
            if(i != 0)
            {
                std::strcat(buffer, " - ");
            }
        }
    }
    return buffer;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::OnTimerAutoHide()
{
    Menu::Show(false);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingShortPressureButton()
{
    if(shortPressureButton != Key::None)
    {
        if (shortPressureButton == Key::Memory && MODE_BTN_MEMORY_IS_SAVE && FDrive::IsConnected())
        {
            EXIT_FROM_SETNAME_TO = (uint)(Menu::IsShown() ? RETURN_TO_MAIN_MENU : RETURN_TO_DISABLE_MENU);
            PageMemory::SaveSignalToFlashDrive();
            shortPressureButton = Key::None;
            return;
        }
        NEED_FINISH_DRAW = 1;
        Menu::SetAutoHide(true);

        Key::E button = shortPressureButton;

        do
        {
            if(button == Key::Enter)                                   // Если нажата кнопка МЕНЮ и мы не находимся в режме настройки измерений.
            {
                if(!Menu::IsShown())
                {
                    Menu::Show(true);
                }
                else
                {
                    if (IS_PAGE(OpenedItem()))
                    {
                        Menu::TemporaryEnableStrNavi();
                    }
                    Menu::CloseOpenedItem();
                }
            }
            else if (Menu::IsShown() && Key(button).IsFunctional())       // Если меню показано и нажата функциональная клавиша
            {
                Control *item = Menu::ItemUnderButton(button);
                if (HINT_MODE_ENABLED)
                {
                    SetItemForHint(item);
                }
                else
                {
                    if(item)
                    {
                        ((Control *)item)->ShortPress();
                    }
                }
            }
            else                                                        // Если меню не показано.
            {
                Page::Name::E name = ((const Page *)OpenedItem())->GetName();
                if(button == Key::ChannelA && name == Page::Name::ChannelA && Menu::IsShown())
                {
                    SET_ENABLED_A = !SET_ENABLED_A;
                    PageChannelA::OnChanged_Input(true);
                    break;
                }
                if(button == Key::ChannelB && name == Page::Name::ChannelB && Menu::IsShown())
                {
                    SET_ENABLED_B = !SET_ENABLED_B;
                    PageChannelB::OnChanged_Input(true);
                    break;
                }

                Page *page = (Page *)PageForButton(button);
                if(page && page != (Page *)PageHelp::pointer)
                {
                    page->SetCurrent(true);
                    page->Open(true);
                    Menu::TemporaryEnableStrNavi();
                    Menu::Show(true);
                }
            }
        } while(false);

        shortPressureButton = Key::None;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingLongPressureButton()
{
    Key::E button = longPressureButton;

    if(button != Key::None)
    {
        Control *item = OpenedItem();
        
        Beeper::ButtonRelease();
        NEED_FINISH_DRAW = 1;
        SetAutoHide(true);

        if(button == Key::Time)
        {
            TShift::Set(0);
        }
        else if(button == Key::Trig)
        {
        }
        else if(button == Key::ChannelA)
        {
            RShift::Set(Chan::A, RShift::ZERO);
        }
        else if(button == Key::ChannelB)
        {
            RShift::Set(Chan::B, RShift::ZERO);
        }
        else if(button == (Key::Enter))
        {
            if (IS_PAGE_SB(OpenedItem()))
            {
                CloseOpenedItem();
            }
            else
            {
                Show(!Menu::IsShown());
                if (NOT_PAGE(item))
                {
                    TemporaryEnableStrNavi();
                }
            }
        }
        else if(Menu::IsShown() && Key(button).IsFunctional())
        {
            Control *control = Menu::ItemUnderButton(button);
            if(control)
            {
                control->LongPress();
                if (NOT_PAGE(control))
                {
                    TemporaryEnableStrNavi();
                }
            }
        }
        else
        {
            // остальные кнопки не обрабатываются
        }
        longPressureButton = Key::None;
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingPressButton()
{
    if ((pressButton >= Key::F1 && pressButton <= Key::F5) || pressButton == Key::Enter)
    {
        if (pressButton != Key::Enter)
        {
            itemUnderKey = Menu::ItemUnderButton(pressButton);
        }
    }
    if ((pressButton == Key::Start) && (MODE_WORK != ModeWork::RAM))
    {
        FPGA::OnPressStart();
    }
    pressButton = Key::None;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ProcessingReleaseButton()
{
    if((releaseButton >= Key::F1 && releaseButton <= Key::F5) || pressButton == Key::Enter)
    {
        itemUnderKey = 0;
        releaseButton = Key::None;
    }
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
void Menu::OnTimerStrNaviAutoHide()
{
    SHOW_STRING_NAVIGATION = 0;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ShortPress_ChoiceReg(void *choice_)
{
    Choice *choice = (Choice*)choice_;

    if(!choice->IsAcitve()) 
    {
        CHOICE_RUN_FUNC_CHANGED(choice, false);
    } 
    else if(OpenedItem() != choice) 
    {
        choice->SetCurrent(!choice->IsCurrentItem());
    }
    else
    {
        // остальные ситуации не обрабатываются
    }
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
void Menu::OpenItemTime()
{
    Display::ShowWarning(Warning::TimeNotSet);
    ShortPressureButton(Key::Service);
    Update();
    Display::Update();
    for (int i = 0; i < 2; i++)
    {
        Update();
        Display::Update();
    }
    ShortPressureButton(Key::F4);
    Update();
    Display::Update();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::NeedForFireSetLED()
{
    Control *item = OpenedItem();
    
    if (!Menu::IsShown())
    {
        return IS_CHOICE_REG(item) || IS_CHOICE(item) || IS_GOVERNOR(item);
    }

    Page::Name::E name = GetNameOpenedPage();
    if (
            name == Page::Name::SB_Debug_SerialNumber   ||
            name == Page::Name::SB_Service_FFT_Cursors  || 
            name == Page::Name::Measures_Auto_Tune      || 
            name == Page::Name::SB_Memory_Last          || 
            name == Page::Name::SB_Memory_Internal      ||
            PageMeasures::PageCursors::PageSet::IsRegSetActiveOnCursors() ||
            (name == Page::Name::SB_Service_Function && FUNC_MODE_DRAW_IS_ENABLED)
        )
    {
        return true;
    }
    
    item = CurrentItem();
    
    if (IS_GOVERNOR(item)       ||
        IS_CHOICE_REG(item)     ||
        IS_GOVERNOR_COLOR(item))
    {
        return true;
    }
    
    item = OpenedItem();

    if (IS_CHOICE(item)  ||
        (IS_PAGE(item) && ((const Page *)OpenedItem())->NumSubPages() > 1)
        )
    {
        return true;
    }

    return false;
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
    return set.menu_show;
}

//---------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::Init()
{
    PageDisplay::Init();
    PageFunction::PageMultimeter::Init();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::RunAfterUpdate(pFuncVV func)
{
    funcAterUpdate = func;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::OpenedItem()
{
    return LastOpened((Page *)pageMain);
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Page::Name::E Menu::GetNameOpenedPage()
{
    return ((const Page *)OpenedItem())->GetName();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
PageBase *Menu::OpenedPage()
{
    return (PageBase *)OpenedItem();
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
Control *Menu::LastOpened(Page *page)
{
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
    Control *opened = LastOpened((Page *)pageMain);
    int8 pos = ((const Page *)opened)->PosCurrentItem();

    if (opened->IsPage() && pos != 0x7f)
    {
        return ((const Page *)opened)->Item(pos);
    }

    return (Control *)opened;
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::CloseOpenedItem()
{
    Control *item = OpenedItem();

    if (IS_PAGE(item))
    {
        if (IS_PAGE_SB(item))
        {
            SMALL_BUTTON_FROM_PAGE(item, 0)->funcOnPress();
        }
        else
        {
            ((Page *)item)->funcOnEnterExit(false);
        }

        Page *keeper = (Page *)KEEPER(item);

        if (keeper)
        {
            keeper->SetPosActItem(0x7f);
        }

        if (item == (Control *)pageMain)    // -V1027
        {
            Menu::Show(false);
        }
    }
    else
    {
        item->Open(false);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void Menu::ChangeItem(Control *item, int delta)
{
    if (IS_CHOICE(item) || IS_CHOICE_REG(item))
    {
        ((Choice *)item)->StartChange(delta);
    }
    else if (IS_GOVERNOR(item))
    {
        Governor *governor = (Governor*)item;
        if (OpenedItem() != governor)
        {
            governor->StartChange(delta);
        }
        else
        {
            governor->ChangeValue(delta);
        }
    }
    else if (IS_GOVERNOR_COLOR(item))
    {
        ((GovernorColor *)item)->ChangeValue(delta);
    }
    else
    {
        // остальные контролы не обрабатываются
    }
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
    if (Menu::IsShown() || NOT_PAGE(OpenedItem()))
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
        else
        {
            if (IS_CHOICE(item) || IS_CHOICE_REG(item))
            {
                ((Choice *)item)->Draw(0, Grid::Top(), false);
                VLine(33).Draw(0, Grid::Top() + 1, Color::BorderMenu(false));
                VLine(33).Draw(1, Grid::Top() + 1);
                VLine(10).Draw(Grid::Right(), Grid::Top() + 30);
                VLine(34).Draw(-1, Grid::Top() + 1, Color::BACK);
                HLine(Grid::Right()).Draw(-1, Grid::Top() + 35);
            }
            else if (IS_GOVERNOR(item))
            {
                ((Governor *)item)->Draw(0, Grid::Top(), true);
                HLine(Grid::Right() + 2).Draw(-2, Grid::Top(), Color::FILL);
                VLine(40).Draw(Grid::Right(), Grid::Top());
            }
            else
            {
                // остальные контролы не обрабатываются
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
void Menu::ResetItemsUnderButton()
{
    for (int i = 0; i < Key::Number; i++)
    {
        SetItemUnderButton((Key::E)i, 0);
    }
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
bool Menu::IsMinimize()
{
    return IS_PAGE_SB(Menu::OpenedItem());
}

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
const SButton *Menu::GetDrawButton(Key::E button)
{
    if (Menu::IsMinimize() && button >= Key::Enter && button <= Key::F5)
    {
        Page *page = (Page *)OpenedItem();
        SButton *sb = (SButton *)page->items[button - Key::Enter];
        return sb;
    }
    return NULL;
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
