set(LIB_NAME ToyLCD)

file(GLOB_RECURSE SOURCE_FILE CONFIGURE_DEPENDS src/*.cpp src/*.c include/${LIB_NAME}/*.h)  # here *.h for VS-project manage, only *.c/cpp is needed, GLOB_RECURSE will get all file, please set a dir for srcfiles, avoiding getting the src from build.  
add_library(${LIB_NAME} ${SOURCE_FILE})
target_include_directories(${LIB_NAME} PUBLIC include)

pico_generate_pio_header(${LIB_NAME} ${CMAKE_CURRENT_LIST_DIR}/src/st7789_lcd.pio OUTPUT_DIR ${CMAKE_CURRENT_LIST_DIR}/include/${LIB_NAME})

########################################################################################
###################################### PICO-LIB ########################################
########################################################################################

target_link_libraries(${LIB_NAME} PUBLIC lvgl::lvgl lvgl::examples lvgl::demos)

target_link_libraries(${LIB_NAME} PUBLIC
        pico_stdlib

        hardware_adc
        hardware_clocks
        hardware_flash
        hardware_dma
        hardware_pio
        hardware_vreg
        hardware_i2c
        hardware_spi
        hardware_pwm
        hardware_interp
        hardware_divider


        # tinyusb_device 
        # tinyusb_board
)

########################################################################################
###################################### PICO-LIB ########################################
########################################################################################

target_compile_options(${LIB_NAME} PUBLIC
        -Wall
        -Wextra
        -Wno-format          # int != int32_t as far as the compiler is concerned because gcc has int32_t as long int
        -Wno-unused-function # we have some for the docs that aren't called
        -Wno-maybe-uninitialized
)



