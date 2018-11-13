#pragma once
#include "Menu/MenuItems.h"


class PageChannelA
{
public:

    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const PageBase *pointer;
};


class PageChannelB
{
public:

    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const PageBase *pointer;
};
