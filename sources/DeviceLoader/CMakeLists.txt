cmake_minimum_required(VERSION 3.0)

#if (COMMAND cmake_policy)
#    cmake_policy(SET CMP0026 OLD)
#endif()

project (Device56)

include_directories("..\\common\\libs\\STM32Cube_F4\\Drivers\\STM32F4xx_HAL_Driver\\Inc\\"
                    "..\\common\\libs\\STM32Cube_F4\\Drivers\\CMSIS\\Device\\ST\\STM32F4xx\\Include\\"
                    "..\\common\\libs\\STM32Cube_F4\\Middlewares\\Third_Party\\FatFs\\src\\"
                    "..\\common\\libs\\STM32Cube_F4\\Middlewares\\ST\\STM32_USB_Host_Library\\Core\\Inc\\"
                    "..\\common\\libs\\STM32Cube_F4\\Middlewares\\ST\\STM32_USB_Host_Library\\Class\\MSC\\Inc\\"
                    "..\\common\\libs\\STM32Cube_F4\\Middlewares\\ST\\STM32_USB_Device_Library\\Core\\Inc\\"
                    "..\\common\\libs\\STM32Cube_F4\\Middlewares\\ST\\STM32_USB_Device_Library\\Class\\CDC\\Inc\\"
                    "..\\common\\"
                    "..\\common\\_VS\\"
                    "..\\common\\_VS\\std_includes\\"
                    "src"
                    "src\\libs\\HAL\\"
                    "src\\libs\\FatFS\\"
                    "src\\libs\\USBH\\")
                    
add_definitions(-DSTM32F437xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4100)
add_compile_options(/wd4505)
add_compile_options(/wd4514)
add_compile_options(/wd4820)
add_compile_options(/wd5039)
add_compile_options(/wd5045)

source_group(src                    FILES src/main.h
                                          src/main.cpp
                                          src/defines.h
                                          src/log.h
                                          src/log.cpp)
source_group(libs\\CMSIS            REGULAR_EXPRESSION "src/libs/CMSIS/*.*")
source_group(libs\\FatFS            REGULAR_EXPRESSION "src/libs/FatFS/*.*")
source_group(libs\\HAL              REGULAR_EXPRESSION "src/libs/HAL/*.*")
source_group(libs\\USBH             REGULAR_EXPRESSION "src/libs/USBH/*.*")
source_group(common                 FILES ../common/Transceiver.h
                                          ../common/Transceiver.cpp)
source_group(Display                REGULAR_EXPRESSION "src/Display/*.*")
source_group(Display\\Font          REGULAR_EXPRESSION "../common/Display/Font/*.*")
source_group(Hardware               REGULAR_EXPRESSION "src/Hardware/*.*")
source_group(Hardware\\HAL          REGULAR_EXPRESSION "src/Hardware/HAL/*.*")
source_group(Hardware\\stm32        REGULAR_EXPRESSION "src/Hardware/stm32/*.*")
source_group(Hardware\\stm32\\4XX   REGULAR_EXPRESSION "src/Hardware/stm32/4XX/*.*")
source_group(Settings               REGULAR_EXPRESSION "src/Settings/*.*")
source_group(Utils                  REGULAR_EXPRESSION "src/Utils/*.*")

file(GLOB SOURCES   ../common/Transceiver.h
                    ../common/Transceiver.cpp
                    src/main.h
                    src/main.cpp
                    src/defines.h
                    src/log.h
                    src/log.cpp
                    src/libs/CMSIS/*.*
                    src/libs/FatFS/*.*
                    src/libs/HAL/*.*
                    src/libs/USBH/*.*
                    src/Display/*.*
                    ../common/Display/Font/*.*
                    src/Hardware/*.*
                    src/Hardware/HAL/*.*
                    src/Hardware/stm32/*.*
                    src/Hardware/stm32/4XX/*.*
                    src/Settings/*.*
                    src/Utils/*.*)

add_library(DeviceLoader ${SOURCES})
