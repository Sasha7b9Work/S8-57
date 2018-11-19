#include "defines.h"
#include "MenuTriggers.h"
#include "Display/Display.h"
#include <cstring>


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
      Key TriggerDebugConsole::bufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {Key::None};
const Key TriggerDebugConsole::sampleBufferForButtons[SIZE_BUFFER_FOR_BUTTONS] = {Key::F5, Key::F5, Key::F4, Key::F4, Key::F3, Key::F3, Key::F2, 
                                                                                    Key::F2, Key::F1,  Key::F1};


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
bool TriggerDebugConsole::Update(Key button)
{
    if (button.IsFunctional())
    {
        for (int i = SIZE_BUFFER_FOR_BUTTONS - 1; i > 0; i--)
        {
            bufferForButtons[i] = bufferForButtons[i - 1];
        }
        bufferForButtons[0] = button;

        if (std::memcmp(bufferForButtons, sampleBufferForButtons, SIZE_BUFFER_FOR_BUTTONS) == 0)
        {
            SHOW_DEBUG_MENU = 1;
            Display::ShowWarning(Warning::MenuDebugEnabled);
            return true;
        }
    }
    else
    {
        bufferForButtons[0] = button;
    }

    return false;
}
