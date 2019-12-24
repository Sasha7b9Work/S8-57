cmake_minimum_required(VERSION 3.0)

project (Device)

include_directories(../common/_VS
                    ../common/_VS/std_includes
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc
                    ../common/libs/STM32Cube_F4/Drivers/CMSIS/Device/ST/STM32F4xx/Include
                    ../common/libs/STM32Cube_F4/Middlewares/Third_Party/FatFs/src
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Host_Library/Class/MSC/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Core/Inc
                    ../common/libs/STM32Cube_F4/Middlewares/ST/STM32_USB_Device_Library/Class/CDC/Inc
                    ../
                    src
                    src/Hardware/HAL
                    src/Hardware/FatFS
                    src/Hardware/USBH
                    src/Hardware/USBD)
                    
add_definitions(-DSTM32F437xx -DDEVICE -DVS_KEIL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4159)    # #pragma pack(pop,...): has popped previously pushed identifier xxx
add_compile_options(/wd4201)    # nonstandard extension used: nameless struct/union
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4820)    # '<anonymous-tag>': 'N' bytes padding added after data member
add_compile_options(/wd5027)    # 'type': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd26812)   #The enum type %type% is unscoped. Prefer 'enum class' over 'enum' (Enum.3)


source_group(common                                  FILES ../common/Command.h
                                                           ../common/Decoder_d.h
                                                           ../common/Decoder_d.cpp
                                                           ../common/Message.h
                                                           ../common/Message.cpp
                                                           ../common/Transceiver.h
                                                           ../common/Transceiver.cpp
                                                           ../common/Transceiver_d.cpp)
source_group(_VS                                     REGULAR_EXPRESSION ../common/_VS/*)
source_group(_VS\\std_includes                       REGULAR_EXPRESSION ../common/_VS/std_includes/*)
source_group(src                                     REGULAR_EXPRESSION src/*)
source_group(Data                                    REGULAR_EXPRESSION src/Data/*)
source_group(Display                                 REGULAR_EXPRESSION src/Display/*)
source_group(Display\\Font                           REGULAR_EXPRESSION ../common/Display/Font/*)
source_group(FlashDrive                              REGULAR_EXPRESSION src/FlashDrive/*)
source_group(FPGA                                    REGULAR_EXPRESSION src/FPGA/*)
source_group(Hardware                                REGULAR_EXPRESSION src/Hardware/*)
source_group(Hardware\\stm32                         REGULAR_EXPRESSION src/Hardware/stm32/*)
source_group(Hardware\\HAL                           REGULAR_EXPRESSION src/Hardware/HAL/*)
source_group(Hardware\\Memory                        REGULAR_EXPRESSION src/Hardware/Memory/*)
source_group(Keyboard                                REGULAR_EXPRESSION src/Keyboard/*)
source_group(Menu                                    REGULAR_EXPRESSION src/Menu/*)
source_group(Menu\\Pages                             REGULAR_EXPRESSION src/Menu/Pages/*)
source_group(Menu\\Pages\\Include                    REGULAR_EXPRESSION src/Menu/Pages/Include/*)
source_group(Menu\\Pages\\PageDisplay                REGULAR_EXPRESSION src/Menu/Pages/PageDisplay/*)
source_group(Menu\\Pages\\PageFunction               REGULAR_EXPRESSION src/Menu/Pages/PageFunction/*)
source_group(Menu\\Pages\\PageRecorder               REGULAR_EXPRESSION src/Menu/Pages/PageRecorder/*)
source_group(Menu\\Pages\\PageHelp                   REGULAR_EXPRESSION src/Menu/Pages/PageHelp/*)
source_group(Menu\\Pages\\PageMeasures               REGULAR_EXPRESSION src/Menu/Pages/PageMeasures/*)
source_group(Menu\\Pages\\PageMemory                 REGULAR_EXPRESSION src/Menu/Pages/PageMemory/*)
source_group(Menu\\Pages\\PageService                REGULAR_EXPRESSION src/Menu/Pages/PageService/*)
source_group(Menu\\Pages\\PageService\\PageDebug     REGULAR_EXPRESSION src/Menu/Pages/PageService/PageDebug/*)
source_group(Menu\\Pages\\PageTrig                   REGULAR_EXPRESSION src/Menu/Pages/PageTrig/*)
source_group(Multimeter                              REGULAR_EXPRESSION src/Multimeter/*)
source_group(Osci                                    REGULAR_EXPRESSION src/Osci/*)
source_group(Osci\\Display                           REGULAR_EXPRESSION src/Osci/Display/*)
source_group(Osci\\Measurements\\                    REGULAR_EXPRESSION src/Osci/Measurements/*)
source_group(Recorder                                REGULAR_EXPRESSION src/Recorder/*)
source_group(SCPI                                    REGULAR_EXPRESSION src/SCPI/*)
source_group(Settings                                REGULAR_EXPRESSION src/Settings/*)
source_group(Test                                    REGULAR_EXPRESSION src/Test/*)
source_group(Tester                                  REGULAR_EXPRESSION src/Tester/*)
source_group(ThirdParty\\HAL                         FILES ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc.h
                                                           ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc_ex.h)
source_group(Utils                                   REGULAR_EXPRESSION src/Utils/*)


file(GLOB SOURCES   ../common/Command.h
                    ../common/Decoder_d.h
                    ../common/Decoder_d.cpp
                    ../common/Message.h
                    ../common/Message.cpp
                    ../common/Transceiver.h
                    ../common/Transceiver.cpp
                    ../common/Transceiver_d.cpp
                    
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc.h
                    ../common/libs/STM32Cube_F4/Drivers/STM32F4xx_HAL_Driver/Inc/stm32f4xx_hal_rtc_ex.h
                    
                    src/Data/*
                    src/Display/*
                    ../common/Display/Font/*
                    src/FlashDrive/*
                    src/FPGA/*
                    src/Hardware/*
                    src/Hardware/HAL/*
                    src/Hardware/Memory/*
                    src/Hardware/stm32/*
                    src/Keyboard/*
                    src/Menu/*
                    src/Menu/Pages/*
                    src/Menu/Pages/Include/*
                    src/Menu/Pages/PageDisplay/*
                    src/Menu/Pages/PageFunction/*
                    src/Menu/Pages/PageRecorder/*
                    src/Menu/Pages/PageHelp/*
                    src/Menu/Pages/PageMeasures/*
                    src/Menu/Pages/PageMemory/*
                    src/Menu/Pages/PageService/*
                    src/Menu/Pages/PageService/PageDebug/*
                    src/Menu/Pages/PageTrig/*
                    src/Multimeter/*
                    src/Osci/*
                    src/Osci/Display/*
                    src/Osci/Measurements/*
                    src/Recorder/*
                    src/SCPI/*
                    src/Settings/*
					src/Test/*
                    src/Tester/*
                    src/Utils/*
                    src/*
                    )

add_library(Device ${SOURCES})
