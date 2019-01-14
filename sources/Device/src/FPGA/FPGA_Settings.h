#pragma once


namespace FPGA
{
    namespace Settings
    {
        void Load();

        void LoadTrigSource();
        /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
        void LoadTrigSourceInput();

        void LoadRanges();

        void LoadRShift(Chan::E ch);

        void LoadTrigLev();

        void LoadTBase();

        void LoadTShift();
        /// Включить/выключить калибратор.
        void LoadCalibratorMode();
        /// Установить значение удержания синхронизации
        void LoadHoldfOff();

        uint8 ValueForRange(Chan::E ch);
    };
}
