#pragma once


namespace Test
{
    void Run();

    namespace Display
    {
        void Init();

        void DeInit();

        void StartTest(const char *nameTest);

        int AddMessage(const char *message, int num = -1);
    }

    namespace ROM
    {
        namespace Data
        {
            bool Test();
        }
    }

    namespace RAM
    {
        bool Test();
    }
}
