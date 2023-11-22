// 2023/11/22 13:37:09 (c) Aleksandr Shevchenko e-mail : Sasha7b9@tut.by
#pragma once
#include "Utils/String.h"


struct WarningStruct
{
    WarningStruct(const char *msg = nullptr);
    WarningStruct(const WarningStruct &s);
    ~WarningStruct();
    String      message;
    uint        timeStart;
    bool IsDead() const;
    int Height(int width) const;
    void Draw(int x, int y) const;
    WarningStruct &operator=(const WarningStruct &s);
};


struct Warnings
{
    static void AddWarning(const char *warning);

    static void Draw();

    static bool IsShown();
};
