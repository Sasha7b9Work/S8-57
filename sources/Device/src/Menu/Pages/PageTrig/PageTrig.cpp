#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


using namespace Osci::Settings;


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
void PageTrig::OnChanged_Mode(bool)
{
    Osci::Stop(false);
    if(set.trig.startMode != TrigStartMode::Single)
    {
        FPGA::OnPressStart();
    }
    
    // Елси находимся в режиме рандомизатора
    if(Osci::InModeRandomizer())
    /// \todo Это вместо функции sTime_RandomizeModeEnabled() было сделано потому, что с функцией экран периодически отваливался
    //if (SET_TBASE < TBase_50ns)
    {
        // и переключаемся на одиночный режим запуска, то надо сохранить имеющийся тип выборки, чтобы восстановить при возвращении в режим 
        // рандомизатора автоматический или ждущий
        if (set.trig.startMode == TrigStartMode::Single)
        {
            set.time.sampleTypeOld = set.time.sampleType;
            set.time.sampleType = SampleType::Real;
        }
        else if(set.trig.startMode == TrigStartMode::Auto)    // Иначе восстановим ранее сохранённый
        {
            set.time.sampleType = set.time.sampleTypeOld;
        }
		else
		{
			// нет действий
		}
    }
}

DEF_CHOICE_3( cMode, // -V206                                                                                                                                         //--- СИНХР - Режим ---
    "Режим"
    ,
    "Задаёт режим запуска:\n"
    "1. \"Авто\" - запуск происходит автоматически.\n"
    "2. \"Ждущий\" - запуск происходит по уровню синхронизации.\n"
    "3. \"Однократный\" - запуск происходит по достижении уровня синхронизации один раз. Для следующего измерения нужно нажать кнопку ПУСК/СТОП."
    ,
    "Авто ",
    "Ждущий",
    "Однократный",
    set.trig.startMode, &PageTrig::self, Item::Active, PageTrig::OnChanged_Mode, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool)
{
    TrigSource::Load();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- СИНХР - Источник ---
    "Источник",
    "Выбор источника сигнала синхронизации."
    ,
    "Канал 1",
    "Канал 2",
    set.trig.source, &PageTrig::self, Item::Active, OnChanged_Source, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool)
{
    TrigPolarity::Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- СИНХР - Полярность ---
    "Полярность"
    ,
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса."
    ,
    "Фронт",
    "Срез",
    set.trig.polarity, &PageTrig::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Filtr(bool)
{
    TrigInput::Load();
}

DEF_CHOICE_3( cFiltr, // -V206                                                                                                                                         //--- СИНХР - Вход ---
    "Фильтр"
    ,
    "Выбор фильтра на источнике синхронизации:\n"
    "1. \"ПС\" - полный сигнал.\n"
    "2. \"ФНЧ\" - фильтр низких частот.\n"
    "3. \"ФВЧ\" - фильтр высоких частот."
    ,
    "ПС",
    "НЧ",
    "ВЧ",
    set.trig.input, &PageTrig::self, Item::Active, OnChanged_Filtr, Choice::AfterDraw
)

DEF_PAGE_6( pTrig, // -V641 // -V1027                                                                                                                                         //--- СИНХР ---
    "СИНХР",
    "Содержит настройки синхронизации.",
    &cMode,                         ///< СИНХР - Режим
    &cSource,                       ///< СИНХР - Источник
    &cPolarity,                     ///< СИНХР - Полярность
    &cFiltr,                        ///< СИНХР - Вход
    PageTrig::PageHoldOff::self, ///< СИНХР - УДЕРЖАНИЕ
    PageTrig::PageFind::self,    ///< СИНХР - ПОИСК
    PageName::Trig, nullptr, Item::Active, Page::Changed, Page::BeforeDraw, E_BfKE
)

const Page * const PageTrig::self = (const Page *)&pTrig;
