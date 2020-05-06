#include "defines.h"
#include "Display/Painter.h"
#include "Display/Primitives.h"
#include "Hardware/Beeper.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Settings/Settings.h"



static bool IsActive_Parameter()
{
    return S_FFT_ENABLED;
}


DEF_CHOICE_2( cView,                                                                                                                                 //--- ФУНКЦИЯ - СПЕКТР - Отображение ---
    "Отображение",
    "Включает и выключает отображение спектра",
    DISABLE_RU,
    ENABLE_RU,
    S_FFT_ENABLED, &PageFFT::self, Item::Active, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_2( cScale,                                                                                                                                      //--- ФУНКЦИЯ - СПЕКТР - Шкала ---
    "Шкала",
    "Задаёт масштаб вывода спектра - линейный или логарифмический",
    "Логарифм",
    "Линейная",
    set.fft.scale, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_3( cSource,                                                                                                                                  //--- ФУНКЦИЯ - СПЕКТР - Источник ---
    "Источник",
    "Выбор источника для расчёта спектра",
    "Канал 1",
    "Канал 2",
    "Канал 1 + 2",
    set.fft.source, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


DEF_CHOICE_4( cWindow,                                                                                                                                      //--- ФУНКЦИЯ - СПЕКТР - Окно ---
    "Окно",
    "Задаёт окно для расчёта спектра",
    "Прямоугольн",
    "Хэмминга",
    "Блэкмена",
    "Ханна",
    set.fft.window, &PageFFT::self, IsActive_Parameter, Choice::Changed, Choice::AfterDraw
)


static bool IsActive_Range()
{
    return (S_FFT_ENABLED && (set.fft.scale == ScaleFFT::Log));
}

DEF_CHOICE_3( cRange,                                                                                                                                   //--- ФУНКЦИЯ - СПЕКТР - Диапазон ---
    "Диапазон",
    "Здесь можно задать предел наблюдения за мощностью спектра",
    "-40дБ",
    "-60дБ",
    "-80дБ",
    set.fft.maxDB, &PageFFT::self, IsActive_Range, Choice::Changed, Choice::AfterDraw
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
    else if(event.key == Key::F1 && event.IsRelease())
    {
        set.fft.cursor = static_cast<uint8>((set.fft.cursor + 1) % 2);
        return true;
    }

    return false;
}

static bool ShowTitle()
{
    return false;
}


static void OnCursors_BeforeDraw()
{
    Region(60, 20).DrawBounded(0, 219, Color::BACK, Color::FILL);

    Text((set.fft.cursor == 0) ? "Курсор 1" : "Курсор 2").Draw(12, 224, Color::FILL);
}


DEF_PAGE_0( pCursors,
    "КУРСОРЫ",
    PageName::FFT_Cursors, &PageFFT::self, ShowTitle, Page::OpenClose, OnCursors_BeforeDraw, HandlerKey_FFT_Cursors
)


const Page * const PageFFT::Cursors::self = static_cast<const Page *>(&pCursors);

static bool IsActive_FFT()
{
    return (set.math.modeDraw == FuncModeDraw::Disabled);
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
