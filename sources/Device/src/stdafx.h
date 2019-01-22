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

#include "Hardware/AT25160N.h"
#include "Hardware/Battery.h"
#include "Hardware/CPU.h"
#include "Hardware/EEPROM.h"
#include "Hardware/FSMC.h"
#include "Hardware/GPIO.h"
#include "Hardware/Sound.h"
#include "Hardware/Timer.h"
#include "Hardware/VCP.h"
#include "Hardware/stm32/Timer4XX.h"

#include "Data/Heap.h"

#include "Multimeter/Multimeter.h"

#include "Osci/Osci_Display.h"
#include "Osci/Display/BottomPart.h"
#include "Osci/Display/HiPart.h"
#include "Osci/Display/MemoryWindow.h"
#include "Osci/Display/PainterData.h"
#include "Osci/Processing/Processing.h"

#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Storage.h"

#include "libs/USBH/usbh_diskio.h"
#include <usbd_def.h>
#include <usbd_cdc.h>
#include <usbd_core.h>

#endif
