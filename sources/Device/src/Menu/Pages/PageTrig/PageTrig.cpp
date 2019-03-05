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
    "Режим", "Mode",
    "Задаёт режим запуска:\n"
    "1. \"Авто\" - запуск происходит автоматически.\n"
    "2. \"Ждущий\" - запуск происходит по уровню синхронизации.\n"
    "3. \"Однократный\" - запуск происходит по достижении уровня синхронизации один раз. Для следующего измерения нужно нажать кнопку ПУСК/СТОП.",
    "Sets the trigger mode:\n"
    "1. \"Auto\" - start automatically.\n"
    "2. \"Standby\" - the launch takes place at the level of synchronization.\n"
    "3. \"Single\" - the launch takes place on reaching the trigger levelonce. For the next measurement is necessary to press the START/STOP.",
    "Авто ",       "Auto",
    "Ждущий",      "Wait",
    "Однократный", "Single",
    START_MODE, pTrig, FuncActive, PageTrig::OnChanged_TrigMode, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Source(bool)
{
    Trig::Source::Load();
}

DEF_CHOICE_2( cSource, // -V206                                                                                                                                    //--- СИНХР - Источник ---
    "Источник", "Source",
    "Выбор источника сигнала синхронизации.",
    "Synchronization signal source choice.",
    "Канал 1", "Chan 1",
    "Канал 2", "Chan 2",
    TRIG_SOURCE, pTrig, FuncActive, OnChanged_Source, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Polarity(bool)
{
    Trig::Polarity::Load();
}

DEF_CHOICE_2( cPolarity, //-V206                                                                                                                                 //--- СИНХР - Полярность ---
    "Полярность", "Polarity",
    "1. \"Фронт\" - запуск происходит по фронту синхроимпульса.\n"
    "2. \"Срез\" - запуск происходит по срезу синхроимпульса.",
    "1. \"Front\" - start happens on the front of clock pulse.\n"
    "2. \"Back\" - start happens on a clock pulse cut.",
    "Фронт", "Front",
    "Срез",  "Back",
    TRIG_POLARITY, pTrig, FuncActive, OnChanged_Polarity, Choice::EmptyDraw
)

//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
static void OnChanged_Input(bool)
{
    Trig::Input::Load();
}

DEF_CHOICE_3( cInput, // -V206                                                                                                                                         //--- СИНХР - Вход ---
    "Вход", "Input",
    "Выбор связи с источником синхронизации:\n"
    "1. \"ПС\" - полный сигнал.\n"
    "2. \"ФНЧ\" - фильтр низких частот.\n"
    "3. \"ФВЧ\" - фильтр высоких частот.",
    "The choice of communication with the source of synchronization:\n"
    "1. \"SS\" - a full signal.\n"
    "2. \"LPF\" - low-pass filter.\n"
    "3. \"HPF\" - high-pass filter frequency.",
    "ПС", "Full",
    "НЧ", "LP",
    "ВЧ", "HP",
    TRIG_INPUT, pTrig, FuncActive, OnChanged_Input, Choice::EmptyDraw
)

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
const PageBase *PageTrig::pointer = &pTrig;

DEF_PAGE_6( pTrig, // -V641 // -V1027                                                                                                                                         //--- СИНХР ---
    "СИНХР", "TRIG",
    "Содержит настройки синхронизации.",
    "Contains synchronization settings.",
    &cMode,                         ///< СИНХР - Режим
    &cSource,                       ///< СИНХР - Источник
    &cPolarity,                     ///< СИНХР - Полярность
    &cInput,                        ///< СИНХР - Вход
    PageTrig::PageHoldOff::pointer, ///< СИНХР - УДЕРЖАНИЕ
    PageTrig::PageFind::pointer,    ///< СИНХР - ПОИСК
    Page::Name::Trig, nullptr, FuncActive, FuncPressPage, FuncDrawPage, FuncRegSetPage
)
