#pragma once


struct WarningStruct
{
    WarningStruct(const char *message = nullptr);
    const char *message;
    uint         timeStart;
};


class Warnings
{
public:

    static void AddWarning(const char *warning);

    static void Show(int left, int down, int width, int height);
};
