cmake_minimum_required(VERSION 3.0)

project (Device56)

include_directories("..\\..\\Device\\src\\"
                    "..\\..\\Device\\src\\Hardware\\FatFS\\"
                    "..\\..\\Device\\src\\Hardware\\HAL\\"
                    "..\\..\\Device\\src\\Hardware\\USBD\\"
                    "..\\..\\Device\\src\\Hardware\\USBH\\"
                    "..\\..\\common\\"
                    "..\\..\\common\\_VS\\"
                    "..\\..\\common\\_VS\\CMSIS\\"
                    "..\\..\\common\\_VS\\FatFS\\"
                    "..\\..\\common\\_VS\\HAL\\"
                    "..\\..\\common\\_VS\\USBD\\"
                    "..\\..\\common\\_VS\\USBH\\"
                    "..\\..\\ThirdParty\\wxWidgets\\include\\"
                    "..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\mswud\\")
                    
add_definitions(-DSTM32F437xx -DOLD_RECORDER)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4459)
add_compile_options(/wd4548)
add_compile_options(/wd4571)
add_compile_options(/wd4625)
add_compile_options(/wd4626)
add_compile_options(/wd4710)
add_compile_options(/wd4820)
add_compile_options(/wd4996)
add_compile_options(/wd5026)
add_compile_options(/wd5039)

source_group(common                    REGULAR_EXPRESSION "../common/*.*")
source_group(_VS                       REGULAR_EXPRESSION "../../common/_VS/*.*")
source_group(_VS\\CMSIS                REGULAR_EXPRESSION "../common/_VS/CMSIS/*.*")
source_group(_VS\\FatFS                REGULAR_EXPRESSION "../common/_VS/FatFS/*.*")
source_group(_VS\\HAL                  REGULAR_EXPRESSION "../common/_VS/HAL/*.*")
source_group(_VS\\HAL\\Legacy          REGULAR_EXPRESSION "../common/_VS/HAL/Legacy/*.*")
source_group(_VS\\USBD                 REGULAR_EXPRESSION "../common/_VS/USBD/*.*")
source_group(_VS\\USBH                 REGULAR_EXPRESSION "../common/_VS/USBH/*.*")
source_group(src                       FILES src/Application.cpp
                                             src/Application.h
                                             src/main_win.cpp
                                             ../../Device/src/defines.cpp
                                             ../../Device/src/defines.h
                                             ../../Device/src/device.cpp
                                             ../../Device/src/device.h
                                             ../../Device/src/log.cpp
                                             ../../Device/src/log.h
                                             ../../Device/src/tables.cpp
                                             ../../Device/src/tables.h)
source_group(Data                      REGULAR_EXPRESSION "src/Data/*.*")
source_group(Display                   REGULAR_EXPRESSION "src/Display/*.*")
source_group(Display\\Font             REGULAR_EXPRESSION "src/Display/Font/*.*")
source_group(FlashDrive                REGULAR_EXPRESSION "src/FlashDrive/*.*")
source_group(FPGA                      REGULAR_EXPRESSION "src/FPGA/*.*")
source_group(Hardware                  REGULAR_EXPRESSION "src/Hardware/*.*")
source_group(Hardware\\HAL             REGULAR_EXPRESSION "src/Hardware/HAL/*.*")
source_group(Keyboard                  REGULAR_EXPRESSION "src/Keyboard/*.*")
source_group(Menu                      REGULAR_EXPRESSION "src/Menu/*.*")
source_group(Menu\\Pages               REGULAR_EXPRESSION "src/Menu/Pages/*.*")
source_group(Menu\\Pages\\Include      REGULAR_EXPRESSION "src/Menu/Pages/Include/*.*")
source_group(Menu\\Pages\\PageDebug    REGULAR_EXPRESSION "src/Menu/Pages/PageDebug/*.*")
source_group(Menu\\Pages\\PageDisplay  REGULAR_EXPRESSION "src/Menu/Pages/PageDisplay/*.*")
source_group(Menu\\Pages\\PageFunction REGULAR_EXPRESSION "src/Menu/Pages/PageFunction/*.*")
source_group(Menu\\Pages\\PageFunction\\PageRecorder REGULAR_EXPRESSION "src/Menu/Pages/PageFunction/PageRecorder/*.*")
source_group(Menu\\Pages\\PageHelp     REGULAR_EXPRESSION "src/Menu/Pages/PageHelp/*.*")
source_group(Menu\\Pages\\PageMeasures REGULAR_EXPRESSION "src/Menu/Pages/PageMeasures/*.*")
source_group(Menu\\Pages\\PageMemory   REGULAR_EXPRESSION "src/Menu/Pages/PageMemory/*.*")
source_group(Menu\\Pages\\PageService  REGULAR_EXPRESSION "src/Menu/Pages/PageService/*.*")
source_group(Menu\\Pages\\PageTrig     REGULAR_EXPRESSION "src/Menu/Pages/PageTrig/*.*")
source_group(Multimeter                REGULAR_EXPRESSION "src/Multimeter/*.*")
source_group(Osci                      REGULAR_EXPRESSION "src/Osci/*.*")
source_group(Osci\\Display             REGULAR_EXPRESSION "src/Osci/Display/*.*")
source_group(Osci\\Measurements\\      REGULAR_EXPRESSION "src/Osci/Measurements/*.*")
source_group(Recorder                  REGULAR_EXPRESSION "src/Recorder/*.*")
source_group(SCPI                      REGULAR_EXPRESSION "src/SCPI/*.*")
source_group(Settings                  REGULAR_EXPRESSION "src/Settings/*.*")
source_group(Tester                    REGULAR_EXPRESSION "src/Tester/*.*")
source_group(Utils                     REGULAR_EXPRESSION "src/Utils/*.*")


file(GLOB SOURCES   ../../common/_VS/*
                    ../../common/_VS/CMSIS/*
                    ../../common/_VS/FatFS/*
                    ../../common/_VS/HAL/*
                    ../../common/_VS/HAL/Legacy/*
                    ../../common/_VS/USBD/*
                    ../../common/_VS/USBH/*
                    ../../common/*.*
                    ../../Device/src/Data/*.*
                    ../../Device/src/Display/*.*
                    ../../Device/src/Display/Font/*.*
                    ../../Device/src/FlashDrive/*.*
                    ../../Device/src/FPGA/*.*
                    ../../Device/src/Hardware/*.*
                    ../../Device/src/Hardware/HAL/*
                    ../../Device/src/Keyboard/*
                    ../../Device/src/Menu/*
                    ../../Device/src/Menu/Pages/*
                    ../../Device/src/Menu/Pages/Include/*
                    ../../Device/src/Menu/Pages/PageDebug/*
                    ../../Device/src/Menu/Pages/PageDisplay/*
                    ../../Device/src/Menu/Pages/PageFunction/*
                    ../../Device/src/Menu/Pages/PageFunction/PageRecorder/*
                    ../../Device/src/Menu/Pages/PageHelp/*
                    ../../Device/src/Menu/Pages/PageMeasures/*
                    ../../Device/src/Menu/Pages/PageMemory/*
                    ../../Device/src/Menu/Pages/PageService/*
                    ../../Device/src/Menu/Pages/PageTrig/*
                    ../../Device/src/Multimeter/*
                    ../../Device/src/Osci/*
                    ../../Device/src/Osci/Display/*
                    ../../Device/src/Osci/Measurements/*
                    ../../Device/src/Recorder/*
                    ../../Device/src/Settings/*
                    ../../Device/src/Tester/*
                    ../../Device/src/Utils/*
                    
                    src/Application.cpp
                    src/Application.h
                    src/main_win.cpp
                    ../../Device/src/defines.cpp
                    ../../Device/src/defines.h
                    ../../Device/src/device.cpp
                    ../../Device/src/device.h
                    ../../Device/src/log.cpp
                    ../../Device/src/log.h
                    ../../Device/src/tables.cpp
                    ../../Device/src/tables.h)

add_executable(Device56 ${SOURCES})


