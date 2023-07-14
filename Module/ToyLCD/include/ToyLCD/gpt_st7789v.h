#ifndef __GPT_ST7789V_H__
#define __GPT_ST7789V_H__

#include <stdio.h>
#include <stdint.h>
#include <stdlib.h> //itoa()
#include <stdbool.h>

#include "pico/stdlib.h"
#include "hardware/spi.h"
#include "hardware/pwm.h"
#include "hardware/dma.h"
#include "hardware/pio.h"
#include "hardware/gpio.h"

#include <ToyLCD/st7789_lcd.pio.h>

/*
    Usual color RGB(565)
*/
#define WHITE   0xFFFF
#define BLACK   0x0000
#define BLUE    0x001F
#define BRED    0XF81F
#define GRED    0XFFE0
#define GBLUE   0X07FF
#define RED     0xF800
#define MAGENTA 0xF81F
#define GREEN   0x07E0
#define CYAN    0x7FFF
#define YELLOW  0xFFE0
#define BROWN   0XBC40
#define BRRED   0XFC07
#define GRAY    0X8430


/*
    Pin map 
*/
#define PIN_DC 8
#define PIN_CS 9
#define PIN_CLK 10
#define PIN_DIN 11
#define PIN_RESET 12
#define PIN_BL 13


/*
    1.14inch 240x135 ips screen
*/
#define SCREEN_WIDTH 240
#define SCREEN_HEIGHT 135
#define BUFF_SIZE 64800    // 240 * 135 * 2 Byte

#define PWM_FREQ 1000 // PWM frequency in Hz
#define SERIAL_CLK_DIV 2.f

#define X_OFFSET 0x28  // 40 pixels
#define Y_OFFSET 0x35  // 53 pixels

// #define x_offset_vertical 0x34  // 52 pixels
// #define y_offset_vertical 0x28  // 40 pixels


/*
    st7789v drive 
*/
void st7789v_lcd_init();
void st7789v_lcd_clear(uint16_t color);
void st7789v_lcd_frame_refresh(const uint8_t *frame);
void st7789v_lcd_frame_refresh_slow(const uint8_t *frame, bool MSB);
// void st7789v_lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1, int channel);
void st7789v_lcd_pixel_refresh(uint16_t x, uint16_t y, uint16_t color);
// void st7789v_lcd_set_backlight(uint8_t brightness);


// void st7789v_lcd_send_command(uint8_t cmd, int channel);
// void st7789v_lcd_send_data(uint8_t data, int channel);

#endif
