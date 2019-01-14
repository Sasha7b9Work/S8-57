#pragma once


namespace FPGA
{
    namespace Settings
    {
        void Load();

        void RangeChange(Chan::E ch, int delta);

        void RangesLoad();

        void RShiftChange(Chan::E ch, int delta);

        void SetRShift(Chan::E ch, uint16 rShift);

        void LoadRShift(Chan::E ch);

        void LoadTrigSource();
        /// Установить в соотвествующие положения выводы, отвечающие за источник и вход синхронизации
        void LoadTrigSourceInput();

        void TrigLevChange(int delta);

        void LoadTrigLev();

        void LoadTrigPolarity();

        void TBaseChange(int delta);

        void LoadTBase();

        void SetTShift(int tShift);

        void LoadTShift();

        void TShiftChange(int delta);

        void SetModeCouple(Chan::E ch, ModeCouple::E couple);
        /// Включить/выключить калибратор.
        void LoadCalibratorMode();
        /// Установить значение удержания синхронизации
        void LoadHoldfOff();
    };
}
