#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsDegug Settings Debug
 *  @{
 */

#define CONSOLE_NUM_STRINGS     (set.dbg_numStrings)
#define CONSOLE_SIZE_FONT       (set.dbg_sizeFont ? 8 : 5)
/// Если truе, то включён режим остновки консоли, в котором нажатие ПУСК/СТОП приостанавливает вывод в консоль.
#define MODE_PAUSE_CONSOLE      (set.dbg_modePauseConsole)
#define NUM_MEASURES_FOR_GATES  (set.dbg_numMeasuresForGates)
#define TIME_COMPENSATION       (set.dbg_timeCompensation)
#define SHOW_RAND_INFO          (set.dbg_showRandInfo)
#define SHOW_RAND_STAT          (set.dbg_showRandStat)
#define SHOW_STAT               (set.dbg_ShowStats)
#define PRETRIGGERED            (set.dbg_pretriggered)
#define BANDWIDTH_DEBUG(ch)     (set.dbg_bandwidth[ch])

#define DBG_SHOW_ALL            (set.dbg_showAll)
#define DBG_SHOW_FLAG           (set.dbg_showFlag)


float   GetStretchADC(Chan ch);

void    SetStretchADC(Chan ch, float kStretch);

/** @}  @}
 */
