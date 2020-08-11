#pragma once

typedef char *va_list; //-V677

int vsprintf(char * s, const char * format, va_list arg);
int snprintf(char * s, unsigned int n, const char * format, ...);
