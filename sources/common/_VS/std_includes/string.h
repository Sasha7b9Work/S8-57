#pragma once


unsigned int  strlen(const char *);
char         *strcat(char *, const char *);
char         *strcpy(char *, const char *);
int           strcmp(void *, const void *);
char          toupper(char);
char          tolower(char);
void          memcpy(void *dest, const void *src, unsigned int size);
void          memset(void *, int, unsigned int);
int           memcmp(void *, const void *, int);
void         *memmove(void *dest, const void *src, unsigned int size);
