#pragma once


namespace Test
{
    void Run();

    namespace Display
    {
        void Init();

        void DeInit();

        void Update();

        void StartTest(char *nameTest);

        int AddMessage(char *message, int num = -1);
    }

    namespace FlashMemory
    {
        namespace Data
        {
            bool Test();
        }
    }
}
