#pragma once


struct WarningStruct
{
    WarningStruct(const char *message = nullptr);
    const char *message;
    uint        timeStart;
    bool IsDead() const;
    int Height(int width) const;
    void Draw(int x, int y, int width) const;
};


class Warnings
{
public:

    static void AddWarning(const char *warning);

    static void Show(int left, int down, int width, int height);
};
