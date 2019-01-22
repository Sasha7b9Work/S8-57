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

#include "FPGA/AD9286.h"
#include "FPGA/FPGA.h"
#include "FPGA/FPGA_HAL.h"
#include "FPGA/FPGA_Math.h"
#include "FPGA/FPGA_Settings.h"

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

#include "Menu/BufferButtons.h"
#include "Menu/MenuTriggers.h"
#include "Menu/Pages/Include/Definition.h"
#include "Menu/Pages/Include/PageChannels.h"
#include "Menu/Pages/Include/PageDebug.h"
#include "Menu/Pages/Include/PageDisplay.h"
#include "Menu/Pages/Include/PageFunction.h"
#include "Menu/Pages/Include/PageHelp.h"
#include "Menu/Pages/Include/PageMeasures.h"
#include "Menu/Pages/Include/PageMemory.h"
#include "Menu/Pages/Include/PageService.h"
#include "Menu/Pages/Include/PageTime.h"
#include "Menu/Pages/Include/PageTrig.h"

#include "Multimeter/Multimeter.h"

#include "Osci/Osci_Display.h"
#include "Osci/Display/BottomPart.h"
#include "Osci/Display/HiPart.h"
#include "Osci/Display/MemoryWindow.h"
#include "Osci/Display/PainterData.h"

#include "Recorder/Recorder.h"
#include "Recorder/Recorder_Display.h"
#include "Recorder/Recorder_Storage.h"

#include "SCPI/SCPI.h"
#include "Settings/Settings.h"

#include "Utils/Buffer.h"
#include "Utils/Debug.h"
#include "Utils/Dictionary.h"
#include "Utils/Math.h"
#include "Utils/Values.h"
#include "Utils/String.h"
#include "Utils/StringUtils.h"

#include "libs/USBH/usbh_diskio.h"
#include <usbd_def.h>
#include <usbd_cdc.h>
#include <usbd_core.h>

#pragma warning(push)
#pragma warning(disable:4365 4710 4996)

#include <algorithm>
#include <climits>
#include <limits>
#include <cmath>
#include <cstring>
#include <cstdarg>
#include <cctype>
#include <cstdlib>
#include <stdio.h>

#pragma warning(pop)

#endif
