#pragma once


struct ContextTester
{
    static void OnPressStart();
    /// Запустить цикл чтения для тестер-компонента. В течение time секунд должно быть считано numPoints точек
    /// Если возвращает false - старт не прошёл
    static void Start();
    static bool Read(uint16 *dataA, uint8 *dataB);
};
