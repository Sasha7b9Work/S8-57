#pragma once


struct WarningStruct
{
    WarningStruct(const char *message = nullptr);
    const char * message;
    uint         timeStart;
};
