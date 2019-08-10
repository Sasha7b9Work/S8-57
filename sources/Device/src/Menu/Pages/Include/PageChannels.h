#pragma once
#include "Menu/MenuItems.h"


class PageChannelA
{
public:

    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};


class PageChannelB
{
public:

    static void OnChanged_Input(bool active);

    static void OnChanged_Couple(bool active);

    static const Page * const self;
};
