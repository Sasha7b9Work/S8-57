#pragma once


class SmootherTester
{
public:
    static void Run(const uint16 *x, const uint8 *y, int smooth);

    static uint16 *X();

    static uint8 *Y();
};
