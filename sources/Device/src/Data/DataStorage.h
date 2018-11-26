#pragma once
#include "DataSettings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
class DataStorage
{
public:

    static void Init();

    static void Push(DataSettings *ds);

    static bool GetData(DataSettings *ds);

    static uint NumElementsInStorage();

    static DataSettings *DataSettingsFromEnd(int fromEnd);

    static uint8 *GetAverageData(Chan::E ch);

    static void GetDataFromEnd(int fromEnd, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    static bool GetLimitation(Chan::E ch, uint8 *data, int direction);
};
