cmake_minimum_required(VERSION 3.0)

project (Panel56)

include_directories("..\\common\\"
                    "..\\common\\_VS\\"
                    "src"
                    "..\\common\\_VS\\HAL\\"
                    "src\\libs\\HAL\\"
                    "..\\common\\_VS\\CMSIS\\")
                    
add_definitions(-DSTM32F437xx)

SET(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -MP -Wall")

add_compile_options(/wd4710)
add_compile_options(/wd4996)

source_group(common                    REGULAR_EXPRESSION "../common/*.*")
source_group(_VS                       REGULAR_EXPRESSION "../common/_VS/*.*")
source_group(_VS\\CMSIS                REGULAR_EXPRESSION "../common/_VS/CMSIS/*.*")
source_group(_VS\\HAL                  REGULAR_EXPRESSION "../common/_VS/HAL/*.*")
source_group(src                       REGULAR_EXPRESSION "src/*.*")
source_group(libs\\HAL                 REGULAR_EXPRESSION "src/libs/HAL/*.h*")
source_group(Display                   REGULAR_EXPRESSION "src/Display/*.*")
source_group(Display\\Font             REGULAR_EXPRESSION "src/Display/Font/*.*")
source_group(FPGA                      REGULAR_EXPRESSION "src/FPGA/*.*")
source_group(Hardware                  REGULAR_EXPRESSION "src/Hardware/*.*")
source_group(Hardware\\stm32           REGULAR_EXPRESSION "src/Hardware/stm32/*.*")
source_group(Settings                  REGULAR_EXPRESSION "src/Settings/*.*")
source_group(Utils                     REGULAR_EXPRESSION "src/Utils/*.*")


file(GLOB SOURCES   ../common/_VS/*
                    ../common/_VS/CMSIS/*
                    ../common/_VS/HAL/*
                    ../common/*.*
                    src/Data/*.*
                    src/Display/*.*
                    src/Display/Font/*.*
                    src/FPGA/*.*
                    src/Hardware/*.*
                    src/Hardware/stm32/*.*
                    src/Settings/*
                    src/Utils/*
                    src/*.*
                    src/libs/HAL/*.h*)

add_library(Panel56 ${SOURCES})


