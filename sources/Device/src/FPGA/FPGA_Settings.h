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

        void SetTShift(int tShift);

        void LoadTShift();

        void TShiftChange(int delta);
        /// Включить/выключить калибратор.
        void LoadCalibratorMode();
        /// Установить значение удержания синхронизации
        void LoadHoldfOff();
    };
}
