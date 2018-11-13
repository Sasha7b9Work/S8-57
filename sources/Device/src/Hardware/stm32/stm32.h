#pragma once

#define ERROR_HANDLER() STM32::ErrorHandler(__FILE__, __LINE__)


class STM32
{
public:
    static void ErrorHandler(const char *, int);

protected:
};
