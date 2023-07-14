set(TARGET_NAME ToyMain)

file(GLOB_RECURSE SOURCE_FILE CONFIGURE_DEPENDS src/*.cpp src/*.c include/${TARGET_NAME}/*.h)  # here *.h for VS-project manage, only *.c/cpp is needed, GLOB_RECURSE will get all file, please set a dir for srcfiles, avoiding getting the src from build.  
add_executable(${TARGET_NAME} ${SOURCE_FILE})
target_include_directories(${TARGET_NAME} PUBLIC include)

##########################################################################################################################
##########################################################################################################################
target_link_libraries(${TARGET_NAME} PUBLIC lvgl::lvgl lvgl::examples lvgl::demos)

target_link_libraries(${TARGET_NAME} PUBLIC
        pico_stdlib
        pico_util

        hardware_adc
        hardware_clocks
        hardware_flash
        hardware_dma
        hardware_pio
        hardware_vreg
        hardware_i2c
        hardware_pwm
        hardware_interp
        hardware_divider

        # tinyusb_device 
        # tinyusb_board

        ToyINA  # MY-LIB
        ToyLCD
)

# enable/disable usb output, uart output
pico_enable_stdio_usb(${TARGET_NAME} 0)
pico_enable_stdio_uart(${TARGET_NAME} 1)

# create map/bin/hex/uf2 file etc.
pico_add_extra_outputs(${TARGET_NAME})

# setup the floating point implementation.
pico_set_float_implementation(${TARGET_NAME} default)   # Use the fast/compact SDK/bootrom implementations
pico_set_double_implementation(${TARGET_NAME} default)  # Use the fast/compact SDK/bootrom implementations

# It doesn't make sense to run this program from flash. Always build a RAM-only binary.
# pico_set_binary_type(test no_flash)

##########################################################################################################################
##########################################################################################################################

# Print memory usage
target_link_options(${TARGET_NAME} PUBLIC -Xlinker --print-memory-usage)  # -Xlinker used for passing the param


target_compile_definitions(${TARGET_NAME} PUBLIC
        # PICO_DEFAULT_UART_TX_PIN=16
        # PICO_DEFAULT_UART_RX_PIN=17
        # PICO_FLOAT_PROPAGATE_NANS=1
        # PICO_DOUBLE_PROPAGATE_NANS=1
        
        PICO_FLOAT_SUPPORT_ROM_V1=0     # the chip version is B2, thus the v1 is discarded
        PICO_DOUBLE_SUPPORT_ROM_V1=0    # details can be found in "Raspberry Pi Pico C/C++ SDK" p.26

        # PICO_STACK_SIZE=4096
        # PICO_STDIO_STACK_BUFFER_SIZE=64 # use a small printf on stack buffer
        # PICO_STDIO_DEFAULT_CRLF=0
        # PICO_DEFAULT_UART=1
        # PICO_DEFAULT_BINARY_TYPE=no_flash        
        
        # PICO_CXX_ENABLE_EXCEPTIONS=1  # for c++ exception handling
)


target_compile_options(${TARGET_NAME} PUBLIC
        -Wall
        -Wextra
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)