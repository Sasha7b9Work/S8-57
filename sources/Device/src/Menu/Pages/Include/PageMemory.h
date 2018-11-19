#pragma once
#include "Menu/MenuItems.h"


/////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////

/** @addtogroup Menu
 *  @{
 *  @addtogroup Pages
 *  @{
 *  @defgroup PageMemory
 *  @{
 */

class PageMemory
{
public:

    static void OnChanged_Points(bool active);

    static void OnPress_Drive_Manager(bool enter = true);

    static void SaveSignalToFlashDrive();

    static const PageBase *pointer;

    class PageLast
    {
    public:
        static const PageBase *pointer;
    };

    class PageInternal
    {
    public:
        static const PageBase *pointer;
    };
};



/** @}  @}  @}
 */
