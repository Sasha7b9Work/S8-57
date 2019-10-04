#pragma once


struct MessageMgr
{
    /// Вызывается при изменении настройки осциллографа, существенно влияющей на усредненеие
    static void OsciSettingsEffectOnAverageChanged();
};
