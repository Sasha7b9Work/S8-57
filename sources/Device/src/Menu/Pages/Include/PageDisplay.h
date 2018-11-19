#pragma once
#include "Display/Colors.h"
#include "Menu/MenuItems.h"


//-------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------------
class PageDisplay
{
public:

    static void Init();

    static void OnChanged_RefreshFPS(bool);

    static const PageBase *pointer;

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageAccumulation
    {
    public:
        static const PageBase *pointer;

        static void OnPress_Accumulation_Clear();
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageAverage
    {
    public:
        static const PageBase *pointer;
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageGrid
    {
    public:
        static const PageBase *pointer;
    };

    //------------------------------------------------------------------------------------------------------------------------------------------------
    class PageSettings
    {
    public:
        static const PageBase *pointer;

        //--------------------------------------------------------------------------------------------------------------------------------------------
        class PageColors
        {
        public:
            static const PageBase *pointer;

            static void OnChanged_Settings_Colors_Background(bool);

            static ColorType colorTypeGrid;

            static ColorType colorTypeA;

            static ColorType colorTypeB;
        };
    };
};
