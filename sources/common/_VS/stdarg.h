#pragma once


#define va_list char *
#define va_start(x, y) x = (char *)y;
int vsprintf(void *, const void *, void *);
#define va_end(x)
