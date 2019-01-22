#pragma once

#ifdef WIN32

#include "defines.h"
#include "device.h"
#include "log.h"
#include "tables.h"

#include "Command.h"
#include "Message.h"

#include "Data/DataBuffer.h"
#include "Data/DataSettings.h"
#include "Data/DataStorage.h"
#include "Data/Reader.h"

#include "Display/Console.h"
#include "Display/Display_Primitives.h"
#include "Display/Grid.h"
#include "Display/Painter.h"
#include "Display/PainterMem.h"
#include "Display/Symbols.h"
#include "Display/font/Font.h"

#include "FlashDrive/FileManager.h"
#include "FlashDrive/FlashDrive.h"

#include "Keyboard/DecoderDevice.h"
#include "Keyboard/HandlersKeys.h"

#include "Data/Heap.h"

#include "Multimeter/Multimeter.h"

#include "libs/USBH/usbh_diskio.h"
#include <usbd_def.h>
#include <usbd_cdc.h>
#include <usbd_core.h>

#endif
