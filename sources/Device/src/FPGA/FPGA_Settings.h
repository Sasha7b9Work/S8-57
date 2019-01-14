#pragma once


namespace FPGA
{
    struct Settings
    {
    public:
        static void LoadTrigSource();
        /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
        static void LoadTrigSourceInput();

        static void LoadRanges();

        static void LoadRShift(Chan::E ch);

        static void LoadTrigLev();

        static void LoadTBase();

        static void LoadTShift();
        /// Включить/выключить калибратор.
        static void LoadCalibratorMode();

    private:
        static uint8 ValueForRange(Chan::E ch);
    };
}
