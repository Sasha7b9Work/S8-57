#include "defines.h"
#include "Settings/Settings.h"
#include "Display/Painter.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"


ScaleFFT::E &ScaleFFT::Ref()
{
    return set.fft.scale;
}


SourceFFT::E &SourceFFT::Ref()
{
    return set.fft.source;
}

WindowFFT::E &WindowFFT::Ref()
{
    return set.fft.window;
}


MaxDBFFT::E &MaxDBFFT::Ref()
{
    return set.fft.maxDB;
}


static bool IsActive_Parameter()
{
    return set.fft.enabled;
}


DEF_CHOICE_2( cView,                                                                                                                                 //--- ФУНКЦИЯ - СПЕКТР - Отображение ---
    "Отображение",
    "Включает и выключает отображение спектра",
    DISABLE_RU,
    ENABLE_RU,
    set.fft.enabled, &PageFFT::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cScale,                                                                                                                                      //--- ФУНКЦИЯ - СПЕКТР - Шкала ---
    "Шкала",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Логарифм",
    "Линейная",
    ScaleFFT::Ref(), &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_3( cSource,                                                                                                                                  //--- ФУНКЦИЯ - СПЕКТР - Источник ---
    "Источник",
    "Выбор источника для расчёта спектра",
    "Канал 1",
    "Канал 2",
    "Канал 1 + 2",
    SourceFFT::Ref(), &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_4( cWindow,                                                                                                                                      //--- ФУНКЦИЯ - СПЕКТР - Окно ---
    "Окно",
    "Задаёт окно для расчёта спектра",
    "Прямоугольн",
    "Хэмминга",
    "Блэкмена",
    "Ханна",
    WindowFFT::Ref(), &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Range()
{
    return set.fft.enabled && ScaleFFT::IsLog();
}

DEF_CHOICE_3( cRange,                                                                                                                                   //--- ФУНКЦИЯ - СПЕКТР - Диапазон ---
    "Диапазон",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "-40дБ",
    "-60дБ",
    "-80дБ",
    MaxDBFFT::Ref(), &PageFFT::self, IsActive_Range, Choice::Changed, Choice::AfterDraw
)


static void OnPress_Cursors_Source()
{
    set.fft.cursor = static_cast<uint8>((set.fft.cursor + 1) % 2);
}

static void Draw_Cursors_Source(int x, int y)
{
    String((set.fft.cursor == 0) ? "1" : "2").Draw(x + 7, y + 5);
}

DEF_GRAPH_BUTTON( bCursors_Source,                                                                                                            //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ - Источник ---
    "Источник",
    "Выбор источника для расчёта спектра",
    &PageFFT::Cursors::self, Item::Active, OnPress_Cursors_Source, Draw_Cursors_Source
)


static bool HandlerKey_FFT_Cursors(const KeyEvent &event)
{
    if (event.IsArrow())
    {
        if (event.IsPress() || event.IsRepeat())
        {
            int delta = (event.IsArrowUp() || event.IsArrowRight()) ? 1 : -1;

            set.fft.posCur[set.fft.cursor] += static_cast<uint8>(delta);
            Beeper::RegulatorShiftRotate();

            return true;
        }
    }

    return false;
}

DEF_PAGE_1( pCursors,                                                                                                                                    //--- ФУНКЦИЯ - СПЕКТР - КУРСОРЫ ---
    "КУРСОРЫ", 
    "Включает курсоры для измерения параметров спектра",
    &bCursors_Source,
    PageName::FFT_Cursors, &PageFFT::self, IsActive_Parameter, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, HandlerKey_FFT_Cursors
)

const Page * const PageFFT::Cursors::self = static_cast<const Page *>(&pCursors);

static bool IsActive_FFT()
{
    return FuncModeDraw::IsDisabled();
}

static void OnOpenClose_FFT(bool)
{
    if (!IsActive_FFT())
    {
        Display::ShowWarning("Отключите математическую функцию");
    }
}

DEF_PAGE_6( pFFT,                                                                                                                                                   //--- СЕРВИС - СПЕКТР ---
    "СПЕКТР",
    "Отображение спектра входного сигнала",
    &cView,
    &cScale,
    &cSource,
    &cWindow,
    &pCursors,
    &cRange,
    PageName::FFT, &PageFunction::self, IsActive_FFT, Page::NormalTitle, OnOpenClose_FFT, Page::BeforeDraw, Page::HandlerKeyEvent
)

const Page * const PageFFT::self = static_cast<const Page *>(&pFFT);
