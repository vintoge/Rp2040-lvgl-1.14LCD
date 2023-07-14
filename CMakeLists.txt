cmake_minimum_required(VERSION 3.25)

########################################
# Pull in SDK (must be before project)
########################################
include(cmake/pico_sdk_import.cmake)
include(cmake/lvgl_import.cmake)

########################################
# Project and SDK setup
########################################
project(ToyDemo C CXX ASM)

# set(PICO_BOARD pico CACHE STRING "Board type")

set(CMAKE_C_COMPILER "/usr/bin/arm-none-eabi-gcc")
set(CMAKE_CXX_COMPILER "/usr/bin/arm-none-eabi-g++")

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED ON)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)
set(CMAKE_CXX_EXTENSIONS OFF)

# set(CMAKE_BUILD_TYPE "Release")
if (NOT CMAKE_BUILD_TYPE)
    set(CMAKE_BUILD_TYPE Debug)  # for DAP-LINK downloading the code to pico
endif()

########################################
# Initialize the SDK
########################################
if (PICO_SDK_VERSION_STRING VERSION_LESS "1.5.0")
    message(FATAL_ERROR "Raspberry Pi Pico SDK version 1.5.0 (or later) required. Your version is ${PICO_SDK_VERSION_STRING}")
endif()

pico_sdk_init()


########################################
# For TinyUSB support (pico as device)
########################################

# set(FAMILY rp2040)
# set(BOARD pico_sdk)
# set(TINYUSB_FAMILY_PROJECT_NAME_PREFIX "tinyusb_dev_")
# add_subdirectory(${PICO_TINYUSB_PATH}/examples/device tinyusb_device_examples)


########################################
# Adding submodule
########################################
if (TARGET tinyusb_device)
    add_subdirectory(ToyMain)
elseif(PICO_ON_DEVICE)
    message(WARNING "Not building target due to that TinyUSB submodule is not initialized in the SDK")
endif()


add_subdirectory(Module/ToyINA)
add_subdirectory(Module/ToyLCD)


