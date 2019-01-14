#pragma once


namespace FPGA
{
    namespace Settings
    {
        void Load();

        void LoadTrigSource();
        /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
        void LoadTrigSourceInput();

        void ChangeRange(Chan::E ch, int delta);

        void LoadRanges();

        void RShiftChange(Chan::E ch, int delta);

        void SetRShift(Chan::E ch, uint16 rShift);

        void LoadRShift(Chan::E ch);

        void TrigLevChange(int delta);

        void LoadTrigLev();

        void LoadTrigPolarity();

        void TBaseChange(int delta);

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
