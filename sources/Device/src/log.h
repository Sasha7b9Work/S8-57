#pragma once
#include "Utils/String.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class Log
{
public:
    static void MessageString(char *message);
    static void Message(char *format, ...);
    static void MessageTrace(char *file, int line, char *format, ...);
private:
};

/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
#define LOG_WRITE(...)      Log::MessageString(String(__VA_ARGS__).CString());
//#define LOG_WRITE(...)       Log::Message(__VA_ARGS__)
#define LOG_ERROR_TRACE(...) Log::Message(__VA_ARGS__)
#define LOG_ERROR(...)       Log::MessageTrace(__FILE__, __LINE__, __VA_ARGS__)
#define LOG_FUNC_ENTER()     Log::Message("%s %d enter", __FUNCTION__, __LINE__)
#define LOG_FUNC_LEAVE()     Log::Message("%s %d leave", __FUNCTION__, __LINE__)
