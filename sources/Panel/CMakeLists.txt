cmake_minimum_required(VERSION 3.0)

project (Panel56)

include_directories(..
                    ../common/_VS
                    ../common/_VS/std_includes
                    src
                    ../common/_VS/HAL
                    src/libs/HAL
                    ../common/_VS/CMSIS)
                    
add_definitions(-DSTM32F437xx -DPANEL)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4159)    # #pragma pack(pop,...): has popped previously pushed identifier xxx
add_compile_options(/wd4514)    # 'function' : unreferenced inline function has been removed
add_compile_options(/wd4710)    # 'xxx': function not inlined
add_compile_options(/wd4820)    # 'xxx': '4' bytes padding added after data member 'yyy'
add_compile_options(/wd4996)    # 'xxx': This function or variable may be unsafe.
add_compile_options(/wd5045)    # Compiler will insert Spectre mitigation for memory load if /Qspectre switch specified

source_group(common                    FILES ../common/Command.h
                                             ../common/DataBus.cpp
                                             ../common/DataBus.h
                                             ../common/Message.cpp
                                             ../common/Message.h
                                             ../common/Transceiver.cpp
                                             ../common/Transceiver.h
                                             ../common/Transceiver.cpp
                                             ../common/Transceiver_Panel.cpp)
source_group(_VS                       REGULAR_EXPRESSION ../common/_VS/*)
source_group(_VS\\std_includes         REGULAR_EXPRESSION ../common/_VS/std_includes/*)
source_group(_VS\\CMSIS                REGULAR_EXPRESSION ../common/_VS/CMSIS/*)
source_group(_VS\\HAL                  REGULAR_EXPRESSION ../common/_VS/HAL/*)
source_group(src                       REGULAR_EXPRESSION src/*)
source_group(libs\\HAL                 REGULAR_EXPRESSION src/libs/HAL/*.h)
source_group(Display                   REGULAR_EXPRESSION src/Display/*)
source_group(Display\\Font             REGULAR_EXPRESSION ../common/Display/Font/*)
source_group(FPGA                      REGULAR_EXPRESSION src/FPGA/*)
source_group(Hardware                  REGULAR_EXPRESSION src/Hardware/*)
source_group(Hardware\\stm32           REGULAR_EXPRESSION src/Hardware/stm32/*)
source_group(Settings                  REGULAR_EXPRESSION src/Settings/*)
source_group(Utils                     REGULAR_EXPRESSION src/Utils/*)


file(GLOB SOURCES   ../common/_VS/*
                    ../common/_VS/std_includes/*
                    ../common/_VS/CMSIS/*
                    ../common/_VS/HAL/*
                    ../common/Command.h
                    ../common/DataBus.cpp
                    ../common/DataBus.h
                    ../common/Message.cpp
                    ../common/Message.h
                    ../common/Transceiver.cpp
                    ../common/Transceiver.h
                    ../common/Transceiver.cpp
                    ../common/Transceiver_Panel.cpp
                    src/Data/*
                    src/Display/*
                    ../common/Display/Font/*
                    src/FPGA/*
                    src/Hardware/*
                    src/Hardware/stm32/*
                    src/Settings/*
                    src/Utils/*
                    src/*
                    src/libs/HAL/*.h)

add_library(Panel57 ${SOURCES})
