#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class PageMeasures
{
public:
    static const PageBase *pointer;

    static const PageBase *GetPageFrequencyCounter();

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageAuto
    {
    public:
        static const PageBase *pointer;

        //--------------------------------------------------------------------------------------------------------------------------------------------
        class PageTune
        {
        public:
            static const PageBase *pointer;
        };
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageCursors
    {
    public:
        static const PageBase *pointer;

        //--------------------------------------------------------------------------------------------------------------------------------------------
        class PageSet
        {
        public:
            static const PageBase *pointer;

            static void Draw_Set_Channel(int x, int y);

            static void OnPress_Set_Channel();
            /// Выбрать следующий курсор
            static void IncCursCntrlU(Chan::E ch);
            /// Выбрать следующий курсор
            static void IncCursCntrlT(Chan::E ch);

            static void OnPress_Set_U();

            static void OnPress_Set_T();

            static void SetCursPos100(Chan::E ch);
            /// Изменить значение позиции курсора напряжения на delta точек
            static void SetShiftCursPosU(Chan::E ch, int numCur, float delta);
            /// Изменить значение позиции курсора времени на delta точек
            static void SetShiftCursPosT(Chan::E ch, int numCur, float delta);

            static bool OnRegSet(int angle);

            static void UpdateCursorsForLook();
            /// Установить позицию курсора напряжения
            static void SetCursorU(Chan::E ch, int numCur, float pos);
            /// Установить позицию курсора времени
            static void SetCursorT(Chan::E ch, int numCur, float pos);
            /// Возвращает true, если вращение ручки УСТАНОВКА будет перемещать курсоры/курсоры
            static bool IsRegSetActiveOnCursors();

        private:
            static const float MAX_POS_U;
            static const float MAX_POS_T;

            /// Установить источник курсорных измерений
            static void SetCursSource(Chan::E ch);
        };
    };
};
