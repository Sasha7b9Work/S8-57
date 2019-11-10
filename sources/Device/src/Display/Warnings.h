#pragma once


struct WarningStruct
{
    WarningStruct(const char *msg = nullptr);
    WarningStruct(const WarningStruct &s);
    ~WarningStruct();
    String      message;
    uint        timeStart;
    bool IsDead() const;
    int Height(int width) const;
    void Draw(int x, int y, int width) const;
    WarningStruct &operator=(const WarningStruct &s);
};


struct Warnings
{
    static void AddWarning(const char *warning);

    static void Draw();

    static bool IsShown();
};
