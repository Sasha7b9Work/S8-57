#pragma once
#include "defines.h"


#ifndef WIN32
#pragma anon_unions
#endif


union BitSet16
{
    uint16 halfWord;
    uint8  byte[2];
    struct
    {
        uint8 byte0;
        uint8 byte1;
    };
    explicit BitSet16(uint16 v) : halfWord(v) {};
    explicit BitSet16(uint8 v0, uint8 v1) : byte0(v0), byte1(v1) {};
};


union BitSet32
{
    uint    word;
    uint16  halfWord[2];
    struct
    {
        uint16 halfWord0;
        uint16 halfWord1;
    };
    uint8   byte[sizeof(uint)];
    struct
    {
        uint8 byte0;
        uint8 byte1;
        uint8 byte2;
        uint8 byte3;
    };

    explicit BitSet32(uint w = 0) : word(w) {}

    explicit BitSet32(uint8 _byte3, uint8 _byte2, uint8 _byte1, uint8 _byte0) : byte0(_byte0), byte1(_byte1), byte2(_byte2), byte3(_byte3)
    {
    }

    void Set(uint8 _byte3, uint8 _byte2, uint8 _byte1, uint8 _byte0)
    {
        byte[0] = _byte0;
        byte[1] = _byte1;
        byte[2] = _byte2;
        byte[3] = _byte3;
    }

    void Set(uint _word)
    {
        word = _word;
    }
};
