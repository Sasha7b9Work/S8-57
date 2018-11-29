#pragma once


class String
{
public:
    String(char *format, ...);
    ~String();

    char *CString() const;

private:
    char *buffer;
};
