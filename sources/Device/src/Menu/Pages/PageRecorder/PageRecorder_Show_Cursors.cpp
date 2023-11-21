// 2023/11/21 10:25:34 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#include "defines.h"
#include "Display/Symbols.h"
#include "Display/Primitives.h"
#include "Menu/Pages/Include/PageRecorder.h"
#include "Settings/Settings.h"


namespace PageRecorder
{
    namespace Show
    {
        namespace Cursors
        {
            DEF_CHOICE_3(cChoice,
                "Курсор",
                "Выбор курсора",
                "Не выбран",
                "1",
                "2",
                S_REC_CURSOR, &PageRecorder::Show::Cursors::self, Choice::Active, Choice::Changed, Choice::AfterDraw
            )


            static void OnPress_Right()
            {

            }

            static void Draw_Right(int x, int y)
            {
                Char(SymbolUGO2::TRIANGLE_RIGHT).Draw4SymbolsInRect(x + 6, y + 2);
            }

            DEF_GRAPH_BUTTON(bRight,
                "Вправо",
                "Перемещение курсора вправо",
                &PageRecorder::Show::Cursors::self, Item::Active, OnPress_Right, Draw_Right
            )


            static void OnPress_Left()
            {

            }

            static void Draw_Left(int x, int y)
            {
                Char(SymbolUGO2::TRIANGLE_LEFT).Draw4SymbolsInRect(x + 6, y + 2);
            }

            DEF_GRAPH_BUTTON(bLeft,
                "Влево",
                "Перемещение курсора влево",
                &PageRecorder::Show::Cursors::self, Item::Active, OnPress_Left, Draw_Left
            )


            DEF_PAGE_3(pCursors, //-V2567
                "КУРСОРЫ",
                "Работа курсорами",
                &cChoice,
                &bLeft,
                &bRight,
                PageName::Recorder_Show_Choice_Cursors,
                &PageRecorder::Show::self, Item::Active, Page::NormalTitle, Page::OpenClose, Page::BeforeDraw, Page::HandlerKeyEvent
            )
        }
    }
}

const Page *const PageRecorder::Show::Cursors::self = static_cast<const Page *>(&PageRecorder::Show::Cursors::pCursors);
