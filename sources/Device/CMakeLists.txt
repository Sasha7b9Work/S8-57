cmake_minimum_required(VERSION 3.0)

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
                    "src"
                    "src\\libs\\HAL\\"
                    "src\\libs\\FatFS\\"
                    "src\\libs\\USBH\\"
                    "src\\libs\\USBD\\")
                    
add_definitions(-DSTM32F437xx -DOLD_RECORDER)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4820)
add_compile_options(/wd5039)

source_group(common                    REGULAR_EXPRESSION "../common/*.*")
source_group(_VS                       REGULAR_EXPRESSION "../common/_VS/*.*")
source_group(src                       REGULAR_EXPRESSION "src/*.*")
source_group(Data                      REGULAR_EXPRESSION "src/Data/*.*")
source_group(Display                   REGULAR_EXPRESSION "src/Display/*.*")
source_group(Display\\Font             REGULAR_EXPRESSION "src/Display/Font/*.*")
source_group(FlashDrive                REGULAR_EXPRESSION "src/FlashDrive/*.*")
source_group(FPGA                      REGULAR_EXPRESSION "src/FPGA/*.*")
source_group(Hardware                  REGULAR_EXPRESSION "src/Hardware/*.*")
source_group(Hardware\\stm32           REGULAR_EXPRESSION "src/Hardware/stm32/*.*")
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
source_group(Osci\\Processing\\        REGULAR_EXPRESSION "src/Osci/Processing/*.*")
source_group(Recorder                  REGULAR_EXPRESSION "src/Recorder/*.*")
source_group(SCPI                      REGULAR_EXPRESSION "src/SCPI/*.*")
source_group(Settings                  REGULAR_EXPRESSION "src/Settings/*.*")
source_group(Tester                    REGULAR_EXPRESSION "src/Tester/*.*")
source_group(Utils                     REGULAR_EXPRESSION "src/Utils/*.*")


file(GLOB SOURCES   ../common/_VS/*
                    ../common/*.*
                    src/Data/*.*
                    src/Display/*.*
                    src/Display/Font/*.*
                    src/FlashDrive/*.*
                    src/FPGA/*.*
                    src/Hardware/*.*
                    src/Hardware/stm32/*.*
                    src/Keyboard/*
                    src/Menu/*
                    src/Menu/Pages/*
                    src/Menu/Pages/Include/*
                    src/Menu/Pages/PageDebug/*
                    src/Menu/Pages/PageDisplay/*
                    src/Menu/Pages/PageFunction/*
                    src/Menu/Pages/PageFunction/PageRecorder/*
                    src/Menu/Pages/PageHelp/*
                    src/Menu/Pages/PageMeasures/*
                    src/Menu/Pages/PageMemory/*
                    src/Menu/Pages/PageService/*
                    src/Menu/Pages/PageTrig/*
                    src/Multimeter/*
                    src/Osci/*
                    src/Osci/Display/*
                    src/Osci/Processing/*
                    src/Recorder/*
                    src/Settings/*
                    src/Tester/*
                    src/Utils/*
                    src/*.*)

add_library(Device56 ${SOURCES})


