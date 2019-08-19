#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsService Settings Service
 *  @{
 */

struct CalibratorMode
{
    enum E
    {
        Freq,
        DC,
        GND
    } value;
    explicit CalibratorMode(E v) : value(v) {};
};

 
#define RECORDER_MODE                   (set.serv_recorder)

#define CALIBRATOR_MODE                 (set.serv_calibratorMode)

#define SOUND_VOLUME                    (set.serv_soundVolume)
#define SOUND_ENABLED                   (SOUND_VOLUME == 0)

#define COLOR_SCHEME                    (set.serv_colorScheme)
#define COLOR_SCHEME_IS_WHITE_LETTERS   (COLOR_SCHEME == ColorScheme::WhiteLetters)

#define REC_PLACE_OF_SAVING             (set.rec_PlaceOfSaving)
#define REC_NUM_CURSOR                  (set.rec_NumCursor)


/** @}  @}
 */
