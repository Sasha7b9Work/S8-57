cmake_minimum_required(VERSION 3.0)

project (Device57)

include_directories(
                    "..\\..\\VS\\Device\\src\\"
                    "..\\..\\Device\\src\\"
                    "..\\..\\Device\\src\\Hardware\\FatFS\\"
                    "..\\..\\Device\\src\\Hardware\\HAL\\"
                    "..\\..\\Device\\src\\Hardware\\USBD\\"
                    "..\\..\\Device\\src\\Hardware\\USBH\\"
                    "..\\..\\common\\"
#                    "..\\..\\common\\_VS\\"
                    "..\\..\\common\\_VS\\CMSIS\\"
                    "..\\..\\common\\_VS\\FatFS\\"
                    "..\\..\\common\\_VS\\HAL\\"
                    "..\\..\\common\\_VS\\USBD\\"
                    "..\\..\\common\\_VS\\USBH\\"
                    "..\\..\\ThirdParty\\wxWidgets\\include\\"
                    "..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\mswud\\"
                    "..\\..\\ThirdParty\\SDL2\\SDL2-2.0.10\\include\\"
                    )
                    
add_definitions(-DSTM32F437xx -DOLD_RECORDER -DWXUSINGDLL -DUSE_SDL2 -DGUI)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4355)
add_compile_options(/wd4459)
add_compile_options(/wd4464) # relative include path contains '..'
add_compile_options(/wd4505)
add_compile_options(/wd4548)
add_compile_options(/wd4571)
add_compile_options(/wd4625)
add_compile_options(/wd4626)
add_compile_options(/wd4668)
add_compile_options(/wd4710)
add_compile_options(/wd4820)
add_compile_options(/wd4996)
add_compile_options(/wd5026)
add_compile_options(/wd5039)

source_group(common                 FILES ../../common/Command.h
                                          ../../common/Message.cpp
                                          ../../common/Message.h
                                          ../../common/Transceiver.cpp
                                          ../../common/Transceiver.h
                                          ../common/Transceiver_win.cpp)
source_group(_VS                                        REGULAR_EXPRESSION "../common/_VS/*.*")
source_group(_VS\\CMSIS                                 REGULAR_EXPRESSION "../common/_VS/CMSIS/*.*")
source_group(_VS\\FatFS                                 REGULAR_EXPRESSION "../common/_VS/FatFS/*.*")
source_group(_VS\\HAL                                   REGULAR_EXPRESSION "../common/_VS/HAL/*.*")
source_group(_VS\\HAL\\Legacy                           REGULAR_EXPRESSION "../common/_VS/HAL/Legacy/*.*")
source_group(_VS\\USBD                                  REGULAR_EXPRESSION "../common/_VS/USBD/*.*")
source_group(_VS\\USBH                                  REGULAR_EXPRESSION "../common/_VS/USBH/*.*")
source_group(Data                                       REGULAR_EXPRESSION "src/Data/*.*")
source_group(Display\\Font                              REGULAR_EXPRESSION "../common/Display/Font/*.*")
source_group(FlashDrive                                 REGULAR_EXPRESSION "src/FlashDrive/*.*")
source_group(Hardware\\HAL                              REGULAR_EXPRESSION "src/Hardware/HAL/*.*")
source_group(Keyboard                                   REGULAR_EXPRESSION "src/Keyboard/*.*")
source_group(Menu                                       REGULAR_EXPRESSION "src/Menu/*.*")
source_group(Menu\\Pages                                REGULAR_EXPRESSION "src/Menu/Pages/*.*")
source_group(Menu\\Pages\\Include                       REGULAR_EXPRESSION "src/Menu/Pages/Include/*.*")
source_group(Menu\\Pages\\PageDebug                     REGULAR_EXPRESSION "src/Menu/Pages/PageDebug/*.*")
source_group(Menu\\Pages\\PageDisplay                   REGULAR_EXPRESSION "src/Menu/Pages/PageDisplay/*.*")
source_group(Menu\\Pages\\PageFunction                  REGULAR_EXPRESSION "src/Menu/Pages/PageFunction/*.*")
source_group(Menu\\Pages\\PageFunction\\PageRecorder    REGULAR_EXPRESSION "src/Menu/Pages/PageFunction/PageRecorder/*.*")
source_group(Menu\\Pages\\PageHelp                      REGULAR_EXPRESSION "src/Menu/Pages/PageHelp/*.*")
source_group(Menu\\Pages\\PageMeasures                  REGULAR_EXPRESSION "src/Menu/Pages/PageMeasures/*.*")
source_group(Menu\\Pages\\PageMemory                    REGULAR_EXPRESSION "src/Menu/Pages/PageMemory/*.*")
source_group(Menu\\Pages\\PageService                   REGULAR_EXPRESSION "src/Menu/Pages/PageService/*.*")
source_group(Menu\\Pages\\PageService\\PageDebug        REGULAR_EXPRESSION "src/Menu/Pages/PageService/PageDebug/*.*")
source_group(Menu\\Pages\\PageTrig                      REGULAR_EXPRESSION "src/Menu/Pages/PageTrig/*.*")
source_group(Multimeter                                 REGULAR_EXPRESSION "src/Multimeter/*.*")
source_group(Osci                                       REGULAR_EXPRESSION "src/Osci/*.*")
source_group(Osci\\Display                              REGULAR_EXPRESSION "src/Osci/Display/*.*")
source_group(Osci\\Measurements\\                       REGULAR_EXPRESSION "src/Osci/Measurements/*.*")
source_group(Recorder                                   REGULAR_EXPRESSION "src/Recorder/*.*")
source_group(SCPI                                       REGULAR_EXPRESSION "src/SCPI/*.*")
source_group(Tester                                     REGULAR_EXPRESSION "src/Tester/*.*")
source_group(Utils                                      REGULAR_EXPRESSION "src/Utils/*.*")
source_group(src                FILES                src/Application.cpp
                                                     src/Application.h
                                                     src/main_win.cpp
                                        ../../Device/src/defines.cpp
                                        ../../Device/src/defines.h
                                        ../../Device/src/device.cpp
                                        ../../Device/src/device.h
                                        ../../Device/src/log.cpp
                                        ../../Device/src/log.h
                                        ../../Device/src/tables.cpp
                                        ../../Device/src/tables.h
                                        ../../Device/src/MessageMgr.cpp
                                        ../../Device/src/MessageMgr.h)
source_group(Display            FILES   ../../Device/src/Display/Colors.h
                                        ../../Device/src/Display/Colors.cpp
                                                     src/Display/Colors_win.cpp
                                        ../../Device/src/Display/Console.h
                                        ../../Device/src/Display/Console.cpp
                                        ../../Device/src/Display/Display.h
                                        ../../Device/src/Display/Display.cpp
                                        ../../Device/src/Display/Display_Primitives.h
                                        ../../Device/src/Display/Display_Primitives.cpp
                                                     src/Display/Display_Primitives_win.cpp
                                        ../../Device/src/Display/Display_Types.h
                                        ../../Device/src/Display/Display_Warnings.h
                                        ../../Device/src/Display/Grid.h
                                        ../../Device/src/Display/Grid.cpp
                                        ../../Device/src/Display/Grid_Types.h
                                        ../../Device/src/Display/Painter.h
                                                     src/Display/Painter_win.cpp
                                        ../../Device/src/Display/PainterMem.h
                                        ../../Device/src/Display/PainterMem.cpp
                                        ../../Device/src/Display/Symbols.h)
source_group(FPGA               FILES   ../../Device/src/FPGA/AD9286.h
                                        ../../Device/src/FPGA/AD9286.cpp
                                        ../../Device/src/FPGA/FPGA.h
                                        ../../Device/src/FPGA/FPGA.cpp
                                                     src/FPGA/FPGA_win.cpp
                                        ../../Device/src/FPGA/FPGA_HAL.h
                                        ../../Device/src/FPGA/FPGA_HAL.cpp
                                                     src/FPGA/FPGA_HAL_win.cpp
                                        ../../Device/src/FPGA/FPGA_Math.h
                                        ../../Device/src/FPGA/FPGA_Math.cpp
                                        ../../Device/src/FPGA/FPGA_Settings.h
                                        ../../Device/src/FPGA/FPGA_Settings.cpp)
source_group(Hardware           FILES   ../../Device/src/Hardware/Battery.h
                                        ../../Device/src/Hardware/Battery.cpp
                                        ../../Device/src/Hardware/Beeper.h
                                        ../../Device/src/Hardware/Beeper.cpp
                                        ../../Device/src/Hardware/Clock.h
                                        ../../Device/src/Hardware/Clock.cpp
                                        ../../Device/src/Hardware/Memory.h
                                        ../../Device/src/Hardware/Memory.cpp
                                        ../../Device/src/Hardware/RAM.h
                                        ../../Device/src/Hardware/RAM.cpp
                                        ../../Device/src/Hardware/Timer.h
                                        ../../Device/src/Hardware/Timer.cpp
                                        ../../Device/src/Hardware/VCP.h
                                                     src/Hardware/Transceiver_win.cpp
                                        ../../Device/src/Hardware/VCP.cpp)
source_group(Settings           FILES   ../../Device/src/Settings/Settings.h
                                        ../../Device/src/Settings/Settings.cpp
                                                     src/Settings/Settings_win.cpp
                                        ../../Device/src/Settings/Settings_default.cpp
                                        ../../Device/src/Settings/SettingsChannel.h
                                        ../../Device/src/Settings/SettingsChannel.cpp
                                        ../../Device/src/Settings/SettingsTransformation.cpp)

file(GLOB SOURCES   ../../common/_VS/*
                    ../../common/_VS/CMSIS/*
                    ../../common/_VS/FatFS/*
                    ../../common/_VS/HAL/*
                    ../../common/_VS/HAL/Legacy/*
                    ../../common/_VS/USBD/*
                    ../../common/_VS/USBH/*
                    
                    ../../common/Command.h
                    ../../common/Message.cpp
                    ../../common/Message.h
                    ../../common/Transceiver.cpp
                    ../../common/Transceiver.h
                    ../common/Transceiver_win.cpp
                    
                    ../../Device/src/Data/*.*
                    
                    ../../Device/src/Display/Colors.h
                    ../../Device/src/Display/Colors.cpp
                                 src/Display/Colors_win.cpp
                    ../../Device/src/Display/Console.h
                    ../../Device/src/Display/Console.cpp
                    ../../Device/src/Display/Display.h
                    ../../Device/src/Display/Display.cpp
                    ../../Device/src/Display/Display_Primitives.h
                    ../../Device/src/Display/Display_Primitives.cpp
                                 src/Display/Display_Primitives_win.cpp
                    ../../Device/src/Display/Display_Types.h
                    ../../Device/src/Display/Display_Warnings.h
                    ../../Device/src/Display/Grid.h
                    ../../Device/src/Display/Grid.cpp
                    ../../Device/src/Display/Grid_Types.h
                    ../../Device/src/Display/Painter.h
                                 src/Display/Painter_win.cpp
                    ../../Device/src/Display/PainterMem.h
                    ../../Device/src/Display/PainterMem.cpp
                    ../../Device/src/Display/Symbols.h
                    
                    ../../common/Display/Font/*.*
                    ../../Device/src/FlashDrive/*.*
                    
                    ../../Device/src/FPGA/AD9286.h
                    ../../Device/src/FPGA/AD9286.cpp
                    ../../Device/src/FPGA/FPGA.h
                    ../../Device/src/FPGA/FPGA.cpp
                                 src/FPGA/FPGA_win.cpp
                    ../../Device/src/FPGA/FPGA_HAL.h
                    ../../Device/src/FPGA/FPGA_HAL.cpp
                                 src/FPGA/FPGA_HAL_win.cpp
                    ../../Device/src/FPGA/FPGA_Math.h
                    ../../Device/src/FPGA/FPGA_Math.cpp
                    ../../Device/src/FPGA/FPGA_Settings.h
                    ../../Device/src/FPGA/FPGA_Settings.cpp
                    
                    ../../Device/src/Hardware/Battery.h
                    ../../Device/src/Hardware/Battery.cpp
                    ../../Device/src/Hardware/Beeper.h
                    ../../Device/src/Hardware/Beeper.cpp
                    ../../Device/src/Hardware/Clock.h
                    ../../Device/src/Hardware/Clock.cpp
                    ../../Device/src/Hardware/Memory.h
                    ../../Device/src/Hardware/Memory.cpp
                    ../../Device/src/Hardware/RAM.h
                    ../../Device/src/Hardware/RAM.cpp
                    ../../Device/src/Hardware/Timer.h
                    ../../Device/src/Hardware/Timer.cpp
                    ../../Device/src/Hardware/VCP.h
                    ../../Device/src/Hardware/VCP.cpp
                                 src/Hardware/Transceiver_win.cpp
                    
                                 src/Hardware/HAL/*
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
                    ../../Device/src/Menu/Pages/PageService/PageDebug/*
                    ../../Device/src/Menu/Pages/PageTrig/*
                    ../../Device/src/Multimeter/*
                    ../../Device/src/Osci/*
                    ../../Device/src/Osci/Display/*
                    ../../Device/src/Osci/Measurements/*
                    ../../Device/src/Recorder/*
                    ../../Device/src/SCPI/*
                    
                    ../../Device/src/Settings/Settings.h
                    ../../Device/src/Settings/Settings.cpp
                                 src/Settings/Settings_win.cpp
                    ../../Device/src/Settings/Settings_default.cpp
                    ../../Device/src/Settings/SettingsChannel.h
                    ../../Device/src/Settings/SettingsChannel.cpp
                    ../../Device/src/Settings/SettingsTransformation.cpp
                    
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
                    ../../Device/src/tables.h
                    ../../Device/src/MessageMgr.cpp
                    ../../Device/src/MessageMgr.h
                    )
                    
add_executable(Device57 ${SOURCES})

target_link_libraries(Device57 "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxbase31ud.lib")
target_link_libraries(Device57 "..\\..\\..\\ThirdParty\\wxWidgets\\generated\\lib\\vc_dll\\wxmsw31ud_core.lib")

target_link_libraries(Device57 "..\\..\\..\\ThirdParty\\SDL2\\SDL2-2.0.10\\generated\\Debug\\SDL2-staticd.lib")

target_link_libraries(Device57 WinMM.lib)
target_link_libraries(Device57 version.lib)
target_link_libraries(Device57 Imm32.Lib)
target_link_libraries(Device57 Setupapi.lib)
