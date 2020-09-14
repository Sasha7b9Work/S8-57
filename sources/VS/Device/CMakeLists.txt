project (S8-57)

include_directories(../../VS/Device/src
                    ../../Device/src
                    ../../Device/src/Hardware/FatFS
                    ../../Device/src/Hardware/HAL
                    ../../Device/src/Hardware/USBD
                    ../../Device/src/Hardware/USBH
                    ../..
                    ../../common/_VS/CMSIS
                    ../../common/_VS/FatFS
                    ../../common/_VS/HAL
                    ../../common/_VS/USBD
                    ../../common/_VS/USBH
                    ../../common/Display/Font
                    ../../ThirdParty/wxWidgets/include
                    ../../ThirdParty/wxWidgets/generated/lib/vc_dll/mswud)
                    
add_definitions(-DSTM32F437xx -DWXUSINGDLL -DGUI -DDEVICE)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4201)    # nonstandard extension used: nameless struct/union
add_compile_options(/wd4464)    # relative include path contains '..'
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4571)    # Informational: catch(...) semantics changed since Visual C++ 7.1; structured exceptions (SEH) are no longer caught
add_compile_options(/wd4623)    # 'derived class' : default constructor was implicitly defined as deleted because a base class default constructor is inaccessible or deleted
add_compile_options(/wd4625)    # 'xxx': copy constructor was implicitly defined as deleted 
add_compile_options(/wd4626)    # 'xxx': assignment operator was implicitly defined as deleted
add_compile_options(/wd4668)    # 'xxx' is not defined as a preprocessor macro, replacing with '0' for '#if/#elif'
add_compile_options(/wd4710)    # 'xxx': function not inlined
add_compile_options(/wd4774)    # 'string' : format string expected in argument number is not a string literal
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe.
add_compile_options(/wd5026)    # 'xxx': move constructor was implicitly defined as deleted
add_compile_options(/wd5027)    # 'type': move assignment operator was implicitly defined as deleted
add_compile_options(/wd5039)    # 'xxx': pointer or reference to potentially throwing function passed to extern C function under -EHc.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified
add_compile_options(/wd5219)
add_compile_options(/wd26812)   # Prefer 'enum class' over 'enum' (Enum.3)

file(GLOB COMMON                            ../../common/Command.h
                                            ../../common/Decoder_d.*
                                            ../../common/Decoder_p.h
                                            ../../common/Message.*
                                            ../../common/common_defines.h)
file(GLOB VS                                ../../common/_VS/*.h)
file(GLOB VS_CMSIS                          ../../common/_VS/CMSIS/*)
file(GLOB VS_FATFS                          ../../common/_VS/FatFS/*)
file(GLOB VS_HAL                            ../../common/_VS/HAL/*)
file(GLOB VS_HAL_LEGACY                     ../../common/_VS/HAL/Legacy/*)
file(GLOB VS_USBD                           ../../common/_VS/USBD/*)
file(GLOB VS_USBH                           ../../common/_VS/USBH/*)
file(GLOB FDRIVE                            ../../Device/src/FlashDrive/*)
file(GLOB FREQMETER                         ../../Device/src/FreqMeter/*)
file(GLOB _GUI                              src/GUI/*.h src/GUI/*.cpp src/GUI/*.c)
file(GLOB GUI_DIALOGS                       src/GUI/Dialogs/*)
file(GLOB KEYBOARD                          ../../Device/src/Keyboard/*)
file(GLOB MENU                              ../../Device/src/Menu/*.h ../../Device/src/Menu/*.cpp)
file(GLOB MENU_PAGES                        ../../Device/src/Menu/Pages/*.h ../../Device/src/Menu/Pages/*.cpp)
file(GLOB MENU_PAGES_INCLUDE                ../../Device/src/Menu/Pages/Include/*)
file(GLOB MENU_PAGES_PAGEDISPLAY            ../../Device/src/Menu/Pages/PageDisplay/*)
file(GLOB MENU_PAGES_PAGEFUNCTION           ../../Device/src/Menu/Pages/PageFunction/*)
file(GLOB MENU_PAGES_PAGERECORDER           ../../Device/src/Menu/Pages/PageRecorder/*)
file(GLOB MENU_PAGES_PAGEHELP               ../../Device/src/Menu/Pages/PageHelp/*)
file(GLOB MENU_PAGES_PAGEMEASURES           ../../Device/src/Menu/Pages/PageMeasures/*)
file(GLOB MENU_PAGES_PAGEMEMORY             ../../Device/src/Menu/Pages/PageMemory/*)
file(GLOB MENU_PAGES_PAGESERVICE            ../../Device/src/Menu/Pages/PageService/*.h ../../Device/src/Menu/Pages/PageService/*.cpp)
file(GLOB MENU_PAGES_PAGESERVICE_PAGE_DEBUG ../../Device/src/Menu/Pages/PageService/PageDebug/*)
file(GLOB MENU_PAGES_PAGETRIG               ../../Device/src/Menu/Pages/PageTrig/*)
file(GLOB MULTIMETER                        ../../Device/src/Multimeter/*)
file(GLOB OSCI_DISPLAY                      ../../Device/src/Osci/Display/*)
file(GLOB OSCI_MATH                         ../../Device/src/Osci/Math/*)
file(GLOB OSCI_MEASUREMENTS                 ../../Device/src/Osci/Measurements/*)
file(GLOB _SCPI                             ../../Device/src/SCPI/*)
file(GLOB TEST                              ../../Device/src/Test/*)
file(GLOB TESTER                            ../../Device/src/Tester/*)
file(GLOB UTILS                             ../../Device/src/Utils/*)
file(GLOB SRC                               ../../Device/src/defines.*
                                            ../../Device/src/device.*
                                            ../../Device/src/globals.*
                                            ../../Device/src/log.h
                                                         src/log_win.cpp
                                                         src/main_win.cpp
                                            ../../Device/src/MessageMgr.*
                                            ../../Device/src/tables.*)
file(GLOB DISPLAY                           ../../Device/src/Display/Colors.*
                                            ../../Device/src/Display/Console.*
                                            ../../Device/src/Display/Display.*
                                            ../../Device/src/Display/Primitives.*
                                                         src/Display/Primitives_win.cpp
                                            ../../Device/src/Display/Grid.*
                                            ../../Device/src/Display/Painter.h
                                                         src/Display/Painter_win.cpp
                                            ../../Device/src/Display/PainterMem.*
                                            ../../Device/src/Display/Symbols.h
                                            ../../Device/src/Display/Warnings.*)
file(GLOB DISPLAY_FONT                      ../../common/Display/Font/AdvancedFont_d.*
                                            ../../common/Display/Font/font5.inc
                                            ../../common/Display/Font/font8.inc
                                            ../../common/Display/Font/Font_d.h
                                            ../../common/Display/Font/Font_d.cpp
                                            ../../Panel/src/Display/Font/Font_p.h
                                                        src/Display/Font/Font_p_win.cpp)
file(GLOB _FPGA                             ../../Device/src/FPGA/AD9286.*
                                            ../../Device/src/FPGA/Calibrator.*
                                            ../../Device/src/FPGA/Context.*
                                            ../../Device/src/FPGA/ContextFreqMeter.*
                                            ../../Device/src/FPGA/ContextRecorder.*
                                            ../../Device/src/FPGA/ContextTester.*
                                            ../../Device/src/FPGA/FPGA.*
                                                         src/FPGA/FPGA_win.cpp
                                            ../../Device/src/FPGA/FPGA_HAL.*
                                                         src/FPGA/FPGA_HAL_win.cpp
                                            ../../Device/src/FPGA/MathFPGA.*
                                            ../../Device/src/FPGA/SettingsFPGA.cpp
                                            ../../Device/src/FPGA/TypesFPGA.h)
file(GLOB HARDWARE                          ../../Device/src/Hardware/Battery.*
                                            ../../Device/src/Hardware/Beeper.*
                                            ../../Device/src/Hardware/Clock.*
                                            ../../Device/src/Hardware/PowerBreaker.*
                                            ../../Device/src/Hardware/Timer.*
                                            ../../Device/src/Hardware/VCP.h
                                                         src/Hardware/VCP_win.cpp)
file(GLOB HARDWARE_HAL                      ../../Device/src/Hardware/HAL/HAL.h
                                            ../../Device/src/Hardware/HAL/HAL_ADC1.cpp
                                            ../../Device/src/Hardware/HAL/HAL_ADC3.cpp
                                            ../../Device/src/Hardware/HAL/HAL_CRC32.cpp
                                            ../../Device/src/Hardware/HAL/HAL_DAC1.cpp
                                                         src/Hardware/HAL/HAL_DAC2_win.cpp
                                                         src/Hardware/HAL/HAL_FSMC_win.cpp
                                                         src/Hardware/HAL/HAL_FSMC_PAN_win.cpp
                                                         src/Hardware/HAL/HAL_IWDG_win.cpp
                                            ../../Device/src/Hardware/HAL/HAL_MSP.cpp
                                            ../../Device/src/Hardware/HAL/HAL_NVIC.cpp
                                            ../../Device/src/Hardware/HAL/HAL_OTP.cpp
                                            ../../Device/src/Hardware/HAL/HAL_PIO.cpp
                                            ../../Device/src/Hardware/HAL/HAL_PIO.h
                                                         src/Hardware/HAL/HAL_ROM_win.cpp
                                            ../../Device/src/Hardware/HAL/HAL_RTC.cpp
                                                         src/Hardware/HAL/HAL_TIM2_win.cpp
                                                         src/Hardware/HAL/HAL_TIM3_win.cpp
                                            ../../Device/src/Hardware/HAL/HAL_USART3.cpp
                                                         src/Hardware/HAL/HAL_win.cpp)
file(GLOB HARDWARE_MEMORY                   ../../Device/src/Hardware/Memory/ExtRAM.*
                                                         src/Hardware/Memory/Memory_win.cpp
                                            ../../Device/src/Hardware/Memory/IntRAM.*
                                            ../../Device/src/Hardware/Memory/RAM.*
                                            ../../Device/src/Hardware/Memory/ROM.*
                                            ../../Device/src/Hardware/Memory/ROM_Data.cpp
                                            ../../Device/src/Hardware/Memory/ROM_NRST.cpp
                                            ../../Device/src/Hardware/Memory/Sector.*
                                                         src/Hardware/Memory/Sector_win.cpp)
file(GLOB OSCI                              ../../Device/src/Osci/AutoSearcher.cpp
                                            ../../Device/src/Osci/DeviceSettings.*
                                            ../../Device/src/Osci/Osci.*
                                            ../../Device/src/Osci/ParametersOsci.*
                                            ../../Device/src/Osci/Reader.*
                                            ../../Device/src/Osci/Roller.cpp
                                                         src/Osci/Osci_win.*)
file(GLOB RECORDER                          ../../Device/src/Recorder/DisplayRecorder.*
                                            ../../Device/src/Recorder/Recorder.*
                                                         src/Recorder/Recorder_win.*
                                            ../../Device/src/Recorder/Sensor.*
                                            ../../Device/src/Recorder/StorageRecorder.*)

file(GLOB SETTINGS                          ../../Device/src/Settings/Settings.*
                                            ../../Device/src/Settings/Settings_default.cpp
                                            ../../Device/src/Settings/SettingsNRST.*
                                                         src/Settings/Settings_win.cpp
                                            ../../Device/src/Settings/ParametersOsci.*)

source_group(common                              FILES ${COMMON})
source_group(_VS                                 FILES ${VS})
source_group(_VS\\CMSIS                          FILES ${VS_CMSIS})
source_group(_VS\\FatFS                          FILES ${VS_FATFS})
source_group(_VS\\HAL                            FILES ${VS_HAL})
source_group(_VS\\HAL\\Legacy                    FILES ${VS_HAL_LEGACY})
source_group(_VS\\USBD                           FILES ${VS_USBD})
source_group(_VS\\USBH                           FILES ${VS_USBH})
source_group(FDrive                              FILES ${FDRIVE})
source_group(FreqMeter                           FILES ${FREQMETER})
source_group(GUI                                 FILES ${_GUI})
source_group(GUI\\Dialogs                        FILES ${GUI_DIALOGS})
source_group(Keyboard                            FILES ${KEYBOARD})
source_group(Menu                                FILES ${MENU})
source_group(Menu\\Pages                         FILES ${MENU_PAGES})
source_group(Menu\\Pages\\Include                FILES ${MENU_PAGES_INCLUDE})
source_group(Menu\\Pages\\PageDisplay            FILES ${MENU_PAGES_PAGEDISPLAY})
source_group(Menu\\Pages\\PageFunction           FILES ${MENU_PAGES_PAGEFUNCTION})
source_group(Menu\\Pages\\PageRecorder           FILES ${MENU_PAGES_PAGERECORDER})
source_group(Menu\\Pages\\PageHelp               FILES ${MENU_PAGES_PAGEHELP})
source_group(Menu\\Pages\\PageMeasures           FILES ${MENU_PAGES_PAGEMEASURES})
source_group(Menu\\Pages\\PageMemory             FILES ${MENU_PAGES_PAGEMEMORY})
source_group(Menu\\Pages\\PageService            FILES ${MENU_PAGES_PAGESERVICE})
source_group(Menu\\Pages\\PageService\\PageDebug FILES ${MENU_PAGES_PAGESERVICE_PAGE_DEBUG})
source_group(Menu\\Pages\\PageTrig               FILES ${MENU_PAGES_PAGETRIG})
source_group(Multimeter                          FILES ${MULTIMETER})
source_group(Osci\\Display                       FILES ${OSCI_DISPLAY})
source_group(Osci\\Math                          FILES ${OSCI_MATH})
source_group(Osci\\Measurements                  FILES ${OSCI_MEASUREMENTS})
source_group(SCPI                                FILES ${_SCPI})
source_group(Test                                FILES ${TEST})
source_group(Tester                              FILES ${TESTER})
source_group(Utils                               FILES ${UTILS})
source_group(src                                 FILES ${SRC})
source_group(Display                             FILES ${DISPLAY})
source_group(Display\\Font                       FILES ${DISPLAY_FONT})
source_group(FPGA                                FILES ${_FPGA})
source_group(Hardware                            FILES ${HARDWARE})
source_group(Hardware\\HAL                       FILES ${HARDWARE_HAL})
source_group(Hardware\\Memory                    FILES ${HARDWARE_MEMORY})
source_group(Osci                                FILES ${OSCI})
source_group(Recorder                            FILES ${RECORDER})
source_group(Settings                            FILES ${SETTINGS})

add_executable(S8-57 ${COMMON} ${VS} ${VS_CMSIS} ${VS_FATFS} ${VS_HAL} ${VS_HAL_LEGACY} ${VS_USBD} ${VS_USBH} ${FDRIVE} ${FREQMETER} ${_GUI}
                     ${GUI_DIALOGS} ${KEYBOARD} ${MENU} ${MENU_PAGES} ${MENU_PAGES_INCLUDE} ${MENU_PAGES_PAGEDISPLAY} ${MENU_PAGES_PAGEFUNCTION}
                     ${MENU_PAGES_PAGERECORDER} ${MENU_PAGES_PAGEHELP} ${MENU_PAGES_PAGEMEASURES} ${MENU_PAGES_PAGEMEMORY} ${MENU_PAGES_PAGESERVICE}
                     ${MENU_PAGES_PAGESERVICE_PAGE_DEBUG} ${MENU_PAGES_PAGETRIG} ${MULTIMETER} ${OSCI_DISPLAY} ${OSCI_MATH} ${OSCI_MEASUREMENTS} ${_SCPI}
                     ${TEST} ${TESTER} ${UTILS} ${SRC} ${DISPLAY} ${DISPLAY_FONT} ${_FPGA} ${HARDWARE} ${HARDWARE_HAL} ${HARDWARE_MEMORY} ${OSCI} ${RECORDER}
                     ${SETTINGS})

target_link_libraries(S8-57 ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxbase31ud)
target_link_libraries(S8-57 ../../../ThirdParty/wxWidgets/generated/lib/vc_dll/wxmsw31ud_core)

target_link_libraries(S8-57 WinMM.lib)
target_link_libraries(S8-57 version.lib)
target_link_libraries(S8-57 Imm32.Lib)
target_link_libraries(S8-57 Setupapi.lib)
