#pragma once


namespace Test
{
    void Run();

    namespace Display
    {
        void Init();

        void DeInit();
    }

    namespace FlashMemory
    {
        namespace Data
        {
            bool Test();
        }
    }
}
