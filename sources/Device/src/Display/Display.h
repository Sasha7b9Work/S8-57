#pragma once
#include "Keyboard/Keyboard.h"



#define DISPLAY_SHOW_WARNING(warn)  Display::ShowWarning(warn)

#define MOSI_HEIGHT         9
#define MOI_HEIGHT_TITLE    19
#define MOI_WIDTH_D_IP      34      /* Увеличение ширины открытого ip-адреса в случае наличия порта */
#define GRID_DELTA          20      /* Количество точек в клетке */

#define TIME_MESSAGES               5


#define DISPLAY_ORIENTATION         (set.dbg_Orientation)
#define DISPLAY_ORIENTATION_IS_NORMAL (DISPLAY_ORIENTATION == Display::Orientation::Normal)

#define SET_NUM_BYTES_ON_DISPLAY    (SET_PEAKDET_EN ? 281 * 2 : 281)

#define SHIFT_IN_MEMORY_IN_POINTS   Display::ShiftInMemoryInPoints()


struct DrawMode
{
    enum E
    {
        Auto,
        Hand
    } value;
};

struct Display
{
    static const int HEIGHT = 240;

    static const int WIDTH = 320;

    static void Init();
    /// Здесь происходит вся отрисовка
    static void Update();
    /// Возвращаемое значение true означает, что дисплей находится в состоянии отрисовки
    static bool InProcess();
    /// Устанавливает функцию, которая выполится после отрисовки кадра однократно
    static void SetFuncAfterUpadteOnce(pFuncVV func);

	static void ShowWarning(const char* warning);
    /// @brief Установить функцию и режим отрисовки экрана.
    /// @details Возможны три варианта.
    /// 1. DrawMode_Hand - в этом случае будет вызываться функция func(), определяемая пользователем, с частотой 25 раз в секунду.
    /// Используется в случаях, когда не выполняется главный цикл.
    /// 2. DrawMode::Auto и func == 0 - в этом случае будет выполняться функция Update() в главном цикле.
    /// 3. DrawMode::Auto и func != 0 - в этом случае будет выполняться функция func(), определяемая пользователем, но в теле
    /// главного цикла, будучи вызываемой из Update() вместо Update().
    static void SetDrawMode(DrawMode::E mode, pFuncVV func = nullptr);
    /// Устанавливает дополнительную функцию, которая будет отрисовываться каждый раз после обновления экрана
    /// timeRemove - время, по истечении которого дополнительная функция отрисовки будет удалена. Если его не указывать, фукнция удаляться самостоятельно не будет
    static void SetAddDrawFunction(pFuncVV func, uint timeRemove = 0);
    /// Удаляет дополнительую функцию отрисовки, установленную вызовом функции SetAddDrawFunction()
    static void RemoveAddDrawFunction();
    /// Возвращает время, через которое меню автоматически скрывается, если не было больше нажатий
    static int TimeMenuAutoHide();
    /// Если экран разделён на две части и основной сигнал выводится сверху - например, в режиме вывода спектра
    static bool IsSeparate();

    static void SaveScreenToDrive();

    static void SaveRow(int row);

    static void LoadBrightness();

    struct FuncOnWait
    {
        static void Start(const char *text, bool eraseBackground);

        static void Stop();
    private:
        static void Func();
    };
};
