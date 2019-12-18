#include "defines.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"
#include "Osci/Osci.h"


void PageTrig::OnChanged_Mode(bool)
{
    Osci::Stop();
    if(!TrigStartMode::IsSingle())
    {
        Osci::OnPressStart();
    }
    
    // Елси находимся в режиме рандомизатора
    if(Osci::InModeRandomizer())
    /// \todo Это вместо функции sTime_RandomizeModeEnabled() было сделано потому, что с функцией экран периодически отваливался
    //if (SET_TBASE < TBase_50ns)
    {
        // и переключаемся на одиночный режим запуска, то надо сохранить имеющийся тип выборки, чтобы восстановить при возвращении в режим 
        // рандомизатора автоматический или ждущий
        if (TrigStartMode::IsSingle())
        {
            set.time.sampleTypeOld = SampleType();
            SampleType().Set(SampleType::Real);
        }
        else if(TrigStartMode::IsAuto())    // Иначе восстановим ранее сохранённый
        {
            SampleType().Set(set.time.sampleTypeOld);
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
    TrigStartMode::Ref(), &PageTrig::self, Item::Active, PageTrig::OnChanged_Mode, Choice::AfterDraw
)


static void OnChanged_Source(bool)
{
    TrigSource::Set();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- СИНХР - Источник ---
    "Источник",
    "Выбор источника сигнала синхронизации."
    ,
    "Канал 1",
    "Канал 2",
    TrigSource::Ref(), &PageTrig::self, Item::Active, OnChanged_Source, Choice::AfterDraw
)


static void OnChanged_Polarity(bool)
{
    TrigPolarity().Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- СИНХР - Полярность ---
    "Полярность"
    ,
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса."
    ,
    "Фронт",
    "Срез",
    TrigPolarity::Ref(), &PageTrig::self, Item::Active, OnChanged_Polarity, Choice::AfterDraw
)


static void OnChanged_Filtr(bool)
{
    TrigInput().Load();
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
    TrigInput::Ref(), &PageTrig::self, Item::Active, OnChanged_Filtr, Choice::AfterDraw
)

DEF_PAGE_6( pTrig,                                                                                                                                                            //--- СИНХР ---
    "СИНХР",
    "Содержит настройки синхронизации.",
    &cMode,
    &cSource,
    &cPolarity,
    &cFiltr,
    PageTrig::PageHoldOff::self,
    PageTrig::PageFind::self,
    PageName::Trig, nullptr, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageTrig::self = static_cast<const Page *>(&pTrig);
