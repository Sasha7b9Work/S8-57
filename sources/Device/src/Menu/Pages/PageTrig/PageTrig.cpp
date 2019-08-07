#include "defines.h"
#include "FPGA/FPGA.h"
#include "Menu/Pages/Include/PageTrig.h"
#include "Settings/Settings.h"

#include "Osci/Osci.h"


using namespace Osci::Settings;


extern const PageBase pTrig;
extern const PageBase ppSearch;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
void PageTrig::OnChanged_TrigMode(bool)
{
    Osci::Stop(false);
    if(!START_MODE_IS_SINGLE)
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
        if (START_MODE_IS_SINGLE)
        {
            SAMPLE_TYPE_OLD = SAMPLE_TYPE;
            SAMPLE_TYPE = SampleType::Real;
        }
        else if(START_MODE_IS_AUTO)    // Иначе восстановим ранее сохранённый
        {
            SAMPLE_TYPE = SAMPLE_TYPE_OLD;
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
    START_MODE, pTrig, FuncActive, PageTrig::OnChanged_TrigMode, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool)
{
    Trig::Source::Load();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- СИНХР - Источник ---
    "Источник",
    "Выбор источника сигнала синхронизации."
    ,
    "Канал 1",
    "Канал 2",
    TRIG_SOURCE, pTrig, FuncActive, OnChanged_Source, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool)
{
    Trig::Polarity::Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- СИНХР - Полярность ---
    "Полярность"
    ,
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса."
    ,
    "Фронт",
    "Срез",
    TRIG_POLARITY, pTrig, FuncActive, OnChanged_Polarity, Choice::FuncDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Input(bool)
{
    Trig::Input::Load();
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
    TRIG_INPUT, pTrig, FuncActive, OnChanged_Input, Choice::FuncDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::pointer = &pTrig;

DEF_PAGE_6( pTrig, // -V641 // -V1027                                                                                                                                         //--- СИНХР ---
    "СИНХР",
    "Содержит настройки синхронизации.",
    &cMode,                         ///< СИНХР - Режим
    &cSource,                       ///< СИНХР - Источник
    &cPolarity,                     ///< СИНХР - Полярность
    &cFiltr,                        ///< СИНХР - Вход
    PageTrig::PageHoldOff::pointer, ///< СИНХР - УДЕРЖАНИЕ
    PageTrig::PageFind::pointer,    ///< СИНХР - ПОИСК
    Page::Name::Trig, nullptr, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
