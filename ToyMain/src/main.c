/**
 * Copyright (c) 2020 Raspberry Pi (Trading) Ltd.
 *
 * SPDX-License-Identifier: BSD-3-Clause
 */

#include <time.h>
#include <math.h>
#include <stdio.h>
#include <stdint.h>
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/pio.h"
#include "hardware/vreg.h"
#include "hardware/gpio.h"
#include "hardware/interp.h"

#include <ToyLCD/gpt_st7789v.h>
#include <ToyLCD/lv_port_disp.h>
#include <ToyLCD/st7789_lcd.pio.h>

#include <ToyMain/ImageData.h>

#include "lvgl.h"
#include "demos/lv_demos.h"

#define PLL_SYS_KHZ (133 * 1000) // 260 MHz is not stable

/*
    LVGL calllback setup
*/
bool repeating_timer_callback(struct repeating_timer *t)
{
    (void)t;

    // gpio_xor_mask(1 << PICO_DEFAULT_LED_PIN);
    lv_tick_inc(5);
    printf("Repeat at %lld\n", time_us_64());
    return true;
}


void lv_example_spinner_1(void)
{
    /*Create a spinner*/
    lv_obj_t * spinner = lv_spinner_create(lv_scr_act(), 1000, 60);
    lv_obj_set_size(spinner, 100, 100);
    lv_obj_center(spinner);
}


int main()
{
    set_sys_clock_khz(PLL_SYS_KHZ, true);
    vreg_set_voltage(VREG_VOLTAGE_1_10);

    stdio_init_all();

    /*
        LVGL timer setup
    */
    struct repeating_timer timer;
    add_repeating_timer_ms(-5, repeating_timer_callback, NULL, &timer);


    /*
        LVGL start
    */
    lv_init();
    lv_port_disp_init();

    // lv_example_spinner_1();
    lv_demo_benchmark();  // color is wrong due to bit reverse

    while (true)
    {
        lv_timer_handler();
        // sleep_ms(5);
    }

    return 0;
}
