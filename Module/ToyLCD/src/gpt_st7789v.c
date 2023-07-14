#include <ToyLCD/gpt_st7789v.h>

static int channel = 0; 

static const uint8_t st7789_init_seq[] = {
        1, 20, 0x01,                        // Software reset
        1, 10, 0x11,                        // Exit sleep mode
        2, 2, 0x3a, 0x05,                   // Set colour mode to 16 bit (65K colors, RGB565)
        2, 0, 0x36, 0x70,                   // Set MADCTL: row then column, refresh is bottom to top
        2, 0, 0xc6, 0x02,                   // Frame Rate Control in Normal Mode. {0x1E:40Hz 0x0F:60Hz 0x05:90Hz 0x02:105Hz}
        5, 0, 0x2a, 0x00, X_OFFSET, (SCREEN_WIDTH - 1 + X_OFFSET) >> 8, (SCREEN_WIDTH - 1 + X_OFFSET) & 0xff,   // CASET: column addresses (0 to SCREEN_WIDTH - 1)
        5, 0, 0x2b, 0x00, Y_OFFSET, (SCREEN_HEIGHT - 1 + Y_OFFSET) >> 8, (SCREEN_HEIGHT - 1 + Y_OFFSET) & 0xff, // RASET: row addresses (0 to SCREEN_HEIGHT - 1)
        1, 2, 0x21,                         // Inversion on, then 10 ms delay (supposedly a hack?)
        1, 2, 0x13,                         // Normal display on, then 10 ms delay
        1, 2, 0x29,                         // Main screen turn on, then wait 500 ms
        0                                   // Terminate list
};


static inline void st7789v_lcd_set_dc_cs(bool dc, bool cs)
{
    sleep_us(1);
    gpio_put_masked((1u << PIN_DC) | (1u << PIN_CS), !!dc << PIN_DC | !!cs << PIN_CS);
    sleep_us(1);
}

static inline void st7789v_lcd_send_command(uint8_t cmd)
{
    st7789v_lcd_set_dc_cs(0, 0);
    dma_channel_set_read_addr(channel, &cmd, false);
    dma_channel_set_trans_count(channel, 1, true);
    dma_channel_wait_for_finish_blocking(channel);
    st7789v_lcd_set_dc_cs(0, 1);
}

static inline void st7789v_lcd_send_data(uint8_t data)
{
    st7789v_lcd_set_dc_cs(1, 0);
    dma_channel_set_read_addr(channel, &data, false);
    dma_channel_set_trans_count(channel, 1, true);
    dma_channel_wait_for_finish_blocking(channel);
    st7789v_lcd_set_dc_cs(1, 1);
}

static inline void st7789v_lcd_send_frame(const uint8_t* pixel_base, uint32_t len)
{
    st7789v_lcd_set_dc_cs(1, 0);
    dma_channel_set_read_addr(channel, pixel_base, false);
    dma_channel_set_trans_count(channel, len, true);
    dma_channel_wait_for_finish_blocking(channel);
    st7789v_lcd_set_dc_cs(1, 1);
}


static void st7789v_lcd_write_reg(const uint8_t *cmd, size_t count) 
{
    st7789v_lcd_send_command(*cmd++);
    if (count >= 2) 
    {
        for (size_t i = 0; i < count - 1; ++i)
        {
            st7789v_lcd_send_data(*cmd++);
        }
    }
}


static void st7789v_lcd_reg_init(const uint8_t *init_seq) 
{
    const uint8_t *cmd = init_seq;
    while (*cmd) 
    {
        st7789v_lcd_write_reg(cmd + 2, *cmd);
        sleep_ms(*(cmd + 1) * 5);
        cmd += *cmd + 2;
    }
}


static void rp2040_init(PIO pio)
{
    /*
        PIO Config 
    */
    uint sm = pio_claim_unused_sm(pio, true);
    uint offset = pio_add_program(pio, &st7789_lcd_program);
    st7789_lcd_program_init(pio, sm, offset, PIN_DIN, PIN_CLK, SERIAL_CLK_DIV);

    /*
        DMA Config
    */
    // static int channel = 0;  // static init always 0, the "=" is not necessary
    channel = dma_claim_unused_channel(true);  // don't like the "channel" param here, need rectify
    dma_channel_config cfg = dma_channel_get_default_config(channel);
    channel_config_set_transfer_data_size(&cfg, DMA_SIZE_8);
    channel_config_set_read_increment(&cfg, true);
    channel_config_set_write_increment(&cfg, false);
    channel_config_set_dreq(&cfg, pio_get_dreq(pio, sm, true));

    dma_channel_configure(
        channel,        // Channel to be configured
        &cfg,               // The configuration we just created
        &pio->txf[sm],      // The initial write address
        NULL,               // The initial read address
        0,                  // Number of transfers; in this case each is 1 byte.
        false               // Start immediately.
    );

    /*
        GPIO Config
    */
    gpio_init(PIN_CS);
    gpio_init(PIN_DC);
    gpio_init(PIN_RESET);
    gpio_init(PIN_BL);
    gpio_set_dir(PIN_CS, GPIO_OUT);
    gpio_set_dir(PIN_DC, GPIO_OUT);
    gpio_set_dir(PIN_RESET, GPIO_OUT);
    gpio_set_dir(PIN_BL, GPIO_OUT);

}

//////////////////////////////////////////////////////////////////////////////////////////////////////////////////


void st7789v_lcd_init(int channel)
{
    /*
        MCU reset
    */
    PIO pio = pio0;
    rp2040_init(pio);

    /*
        LCD reset 
    */
    gpio_put(PIN_RESET, 0);
    sleep_ms(10);
    gpio_put(PIN_RESET, 1);
    sleep_ms(10);

    /*
        LCD init
    */
    gpio_put(PIN_BL, 1);
    gpio_put(PIN_CS, 1);
    st7789v_lcd_reg_init(&st7789_init_seq[0]); 
}


void st7789v_lcd_clear(uint16_t color)
{
    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    uint8_t buff[BUFF_SIZE];
    for (size_t i = 0; i < sizeof(buff); i+=2) 
    {
        buff[i] = hi;
        buff[i + 1] = lo;
    }
    
    st7789v_lcd_send_command(0x2c); // Memory Write
    st7789v_lcd_send_frame(&buff[0], sizeof(buff));
}

void st7789v_lcd_frame_refresh_slow(const uint8_t *frame, bool MSB)
{        
    if(MSB)
    {
        st7789v_lcd_send_command(0x2c); // Memory Write
        st7789v_lcd_send_frame(&frame[0], BUFF_SIZE);
    }
    else
    {
        uint8_t buff[BUFF_SIZE];
        uint8_t temp = 0;
        for (size_t i = 0; i < sizeof(buff); i+=2) 
        {
            buff[i] = *frame++;
            buff[i + 1] = *frame++;

            temp = buff[i];
            buff[i] = buff[i + 1];
            buff[i + 1] = temp;
        }    
        st7789v_lcd_send_command(0x2c); // Memory Write
        st7789v_lcd_send_frame(&buff[0], BUFF_SIZE);
    }
}

static void st7789v_lcd_set_window(uint16_t x0, uint16_t y0, uint16_t x1, uint16_t y1)
{
    st7789v_lcd_send_command(0x2a); // Column Address Set
    st7789v_lcd_send_data((X_OFFSET + x0) >> 8);
    st7789v_lcd_send_data((X_OFFSET + x0) & 0xff);
    st7789v_lcd_send_data((SCREEN_WIDTH - 1 + X_OFFSET + x1) >> 8);
    st7789v_lcd_send_data((SCREEN_WIDTH - 1 + X_OFFSET + x1) & 0xff);

    st7789v_lcd_send_command(0x2b); // Row Address Set
    st7789v_lcd_send_data((Y_OFFSET + y0) >> 8);
    st7789v_lcd_send_data((Y_OFFSET + y0) & 0xff);
    st7789v_lcd_send_data((SCREEN_HEIGHT - 1 + Y_OFFSET + y1) >> 8);
    st7789v_lcd_send_data((SCREEN_HEIGHT - 1 + Y_OFFSET + y1) & 0xff);

    // st7789v_lcd_send_command(0x2c, channel); // Memory Write
}

void st7789v_lcd_frame_refresh(const uint8_t *frame)
{   

    st7789v_lcd_send_command(0x2c); // Memory Write
    st7789v_lcd_send_frame(&frame[0], BUFF_SIZE);
}

void st7789v_lcd_pixel_refresh(uint16_t x, uint16_t y, uint16_t color)
{
    st7789v_lcd_set_window(x, y, x+1, y+1);

    uint8_t hi = color >> 8;
    uint8_t lo = color & 0xFF;

    st7789v_lcd_send_command(0x2c); // Memory Write
    st7789v_lcd_send_data(hi);
    st7789v_lcd_send_data(lo);
}


// void st7789v_lcd_set_backlight(uint8_t brightness)
// {
//     uint16_t duty_cycle = (uint16_t)((brightness / 255.0f) * PWM_FREQ);
//     pwm_set_chan_level(lcd->slice_num, lcd->channel_num, duty_cycle);
// }


