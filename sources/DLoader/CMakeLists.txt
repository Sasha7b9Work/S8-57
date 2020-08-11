cmake_minimum_required(VERSION 3.0)

#if (COMMAND cmake_policy)
#    cmake_policy(SET CMP0026 OLD)
#endif()

project (DLoader)

include_directories(../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ..
                    ../common/_VS
                    ../common/_VS/std_includes
                    src
                    src/Hardware/HAL
                    src/Hardware/FatFS
                    src/Hardware/USBH)
                    
add_definitions(-DSTM32F437xx -DLOADER)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4159)    # #pragma pack(pop,...): has popped previously pushed identifier xxx
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd26812)   # The enum type %type% is unscoped. Prefer 'enum class' over 'enum' (Enum.3)

file(GLOB LIBS_FATFS         ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff.*
                             ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/diskio.*
                             ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff_gen_drv.*
                             ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/ff_gen_drv.*
                             ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/option/ccsbcs.c
                             ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src/option/syscall.c)
file(GLOB LIBS_HAL           ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_cortex.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_cortex.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_dma.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_dma.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_flash_ex.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_flash_ex.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_gpio.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_gpio.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_hcd.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_hcd.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rcc.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_rcc.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_spi.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_spi.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_sram.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_sram.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_tim_ex.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_hal_tim_ex.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_fmc.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_fmc.c
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_ll_usb.h
                             ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Src/stm32f4xx_ll_usb.c)
file(GLOB LIBS_USBH          ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_core.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_core.c
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ctlreq.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ctlreq.c
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_ioreq.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_ioreq.c
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc/usbh_pipes.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Src/usbh_pipes.c
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc.c
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_bot.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_bot.c
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc/usbh_msc_scsi.h
                             ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Src/usbh_msc_scsi.c)
file(GLOB SRC                src/*.h src/*.cpp)
file(GLOB COMMON             ../common/Command.h
                             ../common/Decoder_d.h
                             ../common/Decoder_d.cpp)
file(GLOB DISPLAY            src/Display/*)
file(GLOB DISPLAY_FONT       ../common/Display/Font/Font_d.h
                             ../common/Display/Font/Font_d.cpp
                             ../common/Display/Font/font8.inc
                             src/Display/AdvancedFont_d.cpp)
file(GLOB FDRIVE             src/FlashDrive/*)
file(GLOB HARDWARE           src/Hardware/*.h src/Hardware/*.cpp)
file(GLOB HARDWARE_CMSIS     src/Hardware/CMSIS/*)
file(GLOB HARDWARE_FATFS     src/Hardware/FatFS/*)
file(GLOB HARDWARE_HAL       src/Hardware/HAL/*)
file(GLOB HARDWARE_STM32     src/Hardware/stm32/*)
file(GLOB HARDWARE_USBH      src/Hardware/USBH/*)
file(GLOB KEYBOARD           src/Keyboard/*)
file(GLOB UTILS              src/Utils/*)

set_source_files_properties(${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBH} PROPERTIES HEADER_FILE_ONLY TRUE)

source_group(libs\\FatFS     FILES ${LIBS_FATFS})
source_group(libs\\HAL       FILES ${LIBS_HAL})
source_group(libs\\USBH      FILES ${LIBS_USBH})
source_group(src             FILES ${SRC})
source_group(commmon         FILES ${COMMON})
source_group(Display         FILES ${DISPLAY})
source_group(Display\\Font   FILES ${DISPLAY_FONT})
source_group(FDrive          FILES ${FDRIVE})
source_group(Hardware        FILES ${HARDWARE})
source_group(Hardware\\CMSIS FILES ${HARDWARE_CMSIS})
source_group(Hardware\\FatFS FILES ${HARDWARE_FATFS})
source_group(Hardware\\HAL   FILES ${HARDWARE_HAL})
source_group(Hardware\\stm32 FILES ${HARDWARE_STM32})
source_group(Hardware\\USBH  FILES ${HARDWARE_USBH})
source_group(Keyboard        FILES ${KEYBOARD})
source_group(Utils           FILES ${UTILS})

add_library(DLoader ${LIBS_FATFS} ${LIBS_HAL} ${LIBS_USBH} ${SRC} ${COMMON} ${DISPLAY} ${DISPLAY_FONT} ${FDRIVE} ${HARDWARE} ${HARDWARE_CMSIS}
                    ${HARDWARE_FATFS} ${HARDWARE_HAL} ${HARDWARE_STM32} ${HARDWARE_USBH} ${KEYBOARD} ${UTILS})
