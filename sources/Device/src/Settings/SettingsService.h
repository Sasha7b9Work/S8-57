#pragma once


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Settings
 *  @{
 *  @defgroup SettingsService Settings Service
 *  @{
 */

struct Language
{
    enum E
    {
        RU,
        EN,
        Number
    } value;
    explicit Language(E v) : value(v) {};
};

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

 
#define LANG                            (set.serv_lang)
#define LANG_RU                         (LANG == Language::RU)
#define LANG_EN                         (LANG == Language::EN)

#define RECORDER_MODE                   (set.serv_recorder)

#define TIME_SHOW_LEVELS                (set.disp_timeShowLevels)


#define CALIBRATOR_MODE                 (set.serv_calibratorMode)

#define SOUND_ENABLED                   (set.serv_soundEnable)
#define SOUND_VOLUME                    (set.serv_soundVolume)

#define COLOR_SCHEME                    (set.serv_colorScheme)
#define COLOR_SCHEME_IS_WHITE_LETTERS   (COLOR_SCHEME == ColorScheme::WhiteLetters)

#define REC_PLACE_OF_SAVING             (set.rec_PlaceOfSaving)
#define REC_NUM_CURSOR                  (set.rec_NumCursor)


/** @}  @}
 */
