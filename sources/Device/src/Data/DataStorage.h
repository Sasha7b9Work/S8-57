#pragma once
#include "DataSettings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
namespace DataStorage
{
    void Init();

    void Push(DataSettings *ds);

    uint NumElementsInStorage();

    DataSettings *DataSettingsFromEnd(int fromEnd);

    uint8 *GetAverageData(Chan::E ch);

    void GetDataFromEnd(int fromEnd, DataSettings *ds, uint8 *dataA, uint8 *dataB);

    bool GetLimitation(Chan::E ch, uint8 *data, int direction);
};
