#include "defines.h"
#include "Settings/Settings.h"
#include "Osci/Osci_Settings.h"
#include "Tester_Settings.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
pString Tester::Settings::Scale::ToString() const // -V2506
{
    if (Chan(ch).IsA())
    {
        return Osci::Settings::Range((Osci::Settings::Range::E)value).ToString(Divider::_1);
    }

    static const pString names[][Language::Size] =
    {
        "2לךְ",     "2uA",
        "5לךְ",     "5uְ",
        "10לךְ",    "10uְ",
        "20לךְ",    "20uְ",
        "50לךְ",    "50uְ",
        "100לךְ",   "100uְ",
        "200לךְ",   "200uְ",
        "500לךְ",   "500uְ",
        "1לְ",      "1mְ",
        "2לְ",      "2mְ",
        "5לְ",      "5mְ",
        "10לְ",     "10mְ",
        "20לְ",     "20mְ"
    };

    pString name = names[value][LANG];

    if (name)
    {
        return names[value][LANG];
    }
    else
    {
        LOG_ERROR("ּאססטג םו חאןמכםום");
    }

    return 0;
}
