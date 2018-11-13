#pragma once
#include "Menu/MenuItems.h"


class PageDebug
{
public:
    static void OnChanged_ADC_Stretch_Mode(bool);
    static void OnChanged_DisplayOrientation(bool);

    static const PageBase *pointer;
};
