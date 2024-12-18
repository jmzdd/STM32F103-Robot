/*
 * ssd1306.c
 *
 *  Created on: 2021. 6. 30.
 *      Author: mokhwasomssi
 *
 *      设置显示模式：set_display_on 和 set_display_off 分别开启和关闭显示器。
 *      内存地址模式配置：set_memory_addressing_mode 设置内存寻址模式。
 *      硬件配置：如set_multiplex_ratio、set_com_output_scan_direction等函数用于配置显示器的硬件参数。
 *      初始化：ssd1306_init 初始化显示器并设置默认参数。
 *      屏幕更新：ssd1306_update_screen 更新屏幕显示内容。
 *      清屏操作：ssd1306_black_screen 和 ssd1306_white_screen 分别将屏幕设为全黑或全白。
 *      像素操作：ssd1306_black_pixel 和 ssd1306_white_pixel 分别设置指定位置的像素为黑色或白色。
 *      文本输出：ssd1306_write_char 和 ssd1306_write_string 在屏幕上输出字符和字符串。
 *      光标操作：ssd1306_set_cursor 设置光标位置；ssd1306_enter 和 ssd1306_space 移动光标。
 * 
 */
#include "ssd1306.h"
#include <string.h>
#include "SR04.h"

/* SSD1306 Variable */
static uint8_t ssd1306_buffer[SSD1306_BUFFER_SIZE];
static SSD1306_CURSOR cursor;
SSD1306_FONT current_font;

/* I2C Write Function */
void ssd1306_write_command(uint8_t command)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_COMMAND, 1, &command, 1, 1000);
}

void ssd1306_write_data(uint8_t* buffer, uint16_t size)
{
    HAL_I2C_Mem_Write(SSD1306_I2C, SSD1306_I2C_SA_WRITE, SSD1306_CONTROL_BYTE_DATA, 1, buffer, size, 1000);
}


/* Charge Bump Setting */
void charge_bump_setting(uint8_t charge_bump)
{
    ssd1306_write_command(CHARGE_BUMP_SETTING);
    ssd1306_write_command(charge_bump);
}

/* Fundamental */
void set_contrast_control(uint8_t value)
{
    ssd1306_write_command(SET_CONTRAST_CONTROL);
    ssd1306_write_command(value);
}

void entire_display_off()
{
    ssd1306_write_command(ENTIRE_DISPLAY_OFF);
}

void entire_display_on()
{
    ssd1306_write_command(ENTIRE_DISPLAY_ON);
}

void set_normal_display()
{
    ssd1306_write_command(SET_NORMAL_DISPLAY);
}

void set_inverse_display()
{
    ssd1306_write_command(SET_INVERSE_DISPLAY);
}

void set_display_on()
{
    ssd1306_write_command(SET_DISPLAY_ON);
}

void set_display_off()
{
    ssd1306_write_command(SET_DISPLAY_OFF);
}


/* Scrolling Command */
// 안써용~


/* Addressing Setting */
void set_lower_column_start_address_for_page_addressing_mode(uint8_t addr)
{
    ssd1306_write_command(addr);
}

void set_higher_column_start_address_for_page_addressing_mode(uint8_t addr)
{
    ssd1306_write_command(addr);
}

void set_memory_addressing_mode(uint8_t mode)
{
    ssd1306_write_command(SET_MEMORY_ADDRESSING_MODE);
    ssd1306_write_command(mode);
}

void set_column_address(uint8_t start, uint8_t end)
{
    ssd1306_write_command(SET_COLUMN_ADDRESS);
    ssd1306_write_command(start);
    ssd1306_write_command(end);
}

void set_page_address(uint8_t start, uint8_t end)
{
    ssd1306_write_command(SET_PAGE_ADDRESS);
    ssd1306_write_command(start);
    ssd1306_write_command(end);
}

void set_page_start_address_for_page_addressing_mode(uint8_t page)  // 0xB0(page0) ~ 0xB7(page7)
{
    ssd1306_write_command(page);
}


/* Hardware Configuration */
void set_display_start_line(uint8_t start_line)
{
    ssd1306_write_command(start_line);
}

void set_segment_remap(uint8_t mapping)
{
    ssd1306_write_command(mapping);
}

void set_multiplex_ratio(uint8_t mux)
{
    ssd1306_write_command(SET_MULTIPLEX_RATIO);
    ssd1306_write_command(mux);
}

void set_com_output_scan_direction(uint8_t mode)
{
    ssd1306_write_command(mode);
}

void set_display_offset(uint8_t vertical_shift)
{
    ssd1306_write_command(SET_DISPLAY_OFFSET);
    ssd1306_write_command(vertical_shift);
}

void set_com_pins_hardware_config(uint8_t com_pin_config, uint8_t com_left_right_remap)
{
    uint8_t buffer = 0x02 | (com_pin_config << 4) | (com_left_right_remap << 5);

    ssd1306_write_command(SET_COM_PINS_HARDWARE_CONFIG);
    ssd1306_write_command(buffer);
}


/* Timing & Driving Scheme Setting */
void set_display_clock_divide_ratio_and_osc_freq(uint8_t divide_ratio, uint8_t osc_freq)
{
    uint8_t buffer = (osc_freq << 4) | divide_ratio;

    ssd1306_write_command(SET_DISPLAY_CLOCK_DIVIDE_RATIO_AND_OSC_FREQ);
    ssd1306_write_command(buffer);
}

void set_pre_charge_period(uint8_t phase_1, uint8_t phase_2)
{
    uint8_t buffer = (phase_2 << 4) | phase_1;

    ssd1306_write_command(SET_PRE_CHARGE_PERIOD);
    ssd1306_write_command(buffer);
}

void set_v_comh_deselect_level(uint8_t deselect_level)
{
    ssd1306_write_command(SET_V_COMH_DESELECT_LEVEL);
    ssd1306_write_command(deselect_level);
}


/* SSD1306 Function */
void ssd1306_init()
{
    // SSD1306 App Note 5p
    set_multiplex_ratio(63);

    set_display_offset(0);

    set_display_start_line(0x40);

    set_segment_remap(0xA1);

    set_com_output_scan_direction(0xC8);

    set_com_pins_hardware_config(1, 0);

    set_contrast_control(0x7F);

    entire_display_off();

    set_normal_display();

    set_display_clock_divide_ratio_and_osc_freq(0, 8);

    charge_bump_setting(0x14);

    set_display_on();

    // Clear Ram Data
    ssd1306_black_screen();

    // Set cursor 0, 0
    ssd1306_set_cursor(0, 0);
}

void ssd1306_update_screen()
{
    for(int i = 0; i < SSD1306_PAGE; i ++)
    {
        set_page_start_address_for_page_addressing_mode(0xB0 + i);
        set_lower_column_start_address_for_page_addressing_mode(0x00);
        set_higher_column_start_address_for_page_addressing_mode(0x10);

        ssd1306_write_data(&ssd1306_buffer[SSD1306_WIDTH * i], SSD1306_WIDTH);
    }
}

void ssd1306_black_screen()
{
    for(int i = 0; i < SSD1306_BUFFER_SIZE; i++)
    {
        ssd1306_buffer[i] = 0x00;
    }

    ssd1306_update_screen();
}

void ssd1306_white_screen()
{
    for(int i = 0; i < SSD1306_BUFFER_SIZE; i++)
    {
        ssd1306_buffer[i] = 0xFF;
    }

    ssd1306_update_screen();
}

void ssd1306_black_pixel(uint8_t x, uint8_t y)
{
    // (y / 8) * SSD1306_WIDTH : page
    // y % 8 : data bit D0 - D7
    ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] &= ~(1 << (y % 8));
}

void ssd1306_white_pixel(uint8_t x, uint8_t y)
{
    // (y / 8) * SSD1306_WIDTH : page
    // y % 8 : data bit D0 - D7
    ssd1306_buffer[x + (y / 8) * SSD1306_WIDTH] |= 1 << (y % 8);
}

char ssd1306_write_char(SSD1306_FONT font, char ch)
{
    uint32_t b;

    // Printable Characters : 32 - 126
    if(ch < 32 || ch > 126)
        return 0;

    // Check remaining space on current line
    if (SSD1306_WIDTH < (cursor.x + font.width) ||  SSD1306_HEIGHT < (cursor.y + font.height))
    {
        // Not enough space on current line
        return 0;
    }

    // Use the font to write
    for(int i = 0; i < font.height; i++)
    {
        b = font.data[(ch - 32) * font.height + i];
        
        for(int j = 0; j < font.width; j++)
        {
            if((b << j) & 0x8000)
            {
                ssd1306_white_pixel(cursor.x + j, cursor.y + i);
            }
            else
            {
                ssd1306_black_pixel(cursor.x + j, cursor.y + i);
            }
        }
    }

    // The current space is now taken
    cursor.x += font.width;

    // Return written char for validation
    return ch;
}


// Write full string to screen buffer
char ssd1306_write_string(SSD1306_FONT font, char *str)
{
    current_font = font;

    // Write until null-byte
    while(*str)
    {
        if(ssd1306_write_char(font, *str) != *str)
        {
            // Char could not be written
            return *str;
        }

        // Next char
        str++;
    }

    // Everything ok
    return *str;
}

void ssd1306_set_cursor(uint8_t x, uint8_t y)
{
    cursor.x = x;
    cursor.y = y;
}

void ssd1306_enter()
{
    cursor.x = 0;
    cursor.y += current_font.height;
}

void ssd1306_space()
{
    cursor.x += current_font.width;
}

// DIY Function
void ssd1306_drow_line(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    int dx, dy, p, x, y;
    dx = x2 - x1;
    dy = y2 - y1;
    x = x1;
    y = y1;
    p = 2 * dy - dx;
    while (x < x2)
    {
        if (p >= 0)
        {
            ssd1306_white_pixel(x, y);
            y = y + 1;
            p = p + 2 * dy - 2 * dx;
        }
        else
        {
            ssd1306_white_pixel(x, y);
            p = p + 2 * dy;
        }
        x = x + 1;
    }
}

void ssd1306_drow_boxes(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    for(int i = x1; i < x2; i++)
    {
        ssd1306_white_pixel(i, y1);
        ssd1306_white_pixel(i, y2);
    }

    for(int i = y1; i < y2; i++)
    {
        ssd1306_white_pixel(x1, i);
        ssd1306_white_pixel(x2, i);
    }
}

void ssd1306_clear_area(uint8_t x1, uint8_t y1, uint8_t x2, uint8_t y2)
{
    // 确保坐标在有效范围内
    if (x1 >= SSD1306_WIDTH || x2 >= SSD1306_WIDTH || y1 >= SSD1306_HEIGHT || y2 >= SSD1306_HEIGHT) {
        return;
    }

    // 确保 x1 <= x2 和 y1 <= y2
    if (x1 > x2) {
        uint8_t temp = x1;
        x1 = x2;
        x2 = temp;
    }
    if (y1 > y2) {
        uint8_t temp = y1;
        y1 = y2;
        y2 = temp;
    }

    // 计算清除区域的起始和结束页
    uint8_t start_page = y1 / 8;
    uint8_t end_page = y2 / 8;

    // 清除指定区域内的像素
    for (uint8_t page = start_page; page <= end_page; page++) {
        for (uint8_t x = x1; x <= x2; x++) {
            uint8_t start_bit = (page == start_page) ? y1 % 8 : 0;
            uint8_t end_bit = (page == end_page) ? y2 % 8 : 7;

            // 将整个字节清零
            if (start_bit == 0 && end_bit == 7) {
                ssd1306_buffer[x + (page * SSD1306_WIDTH)] = 0;
            } else {
                // 部分清零
                for (uint8_t bit = start_bit; bit <= end_bit; bit++) {
                    ssd1306_buffer[x + (page * SSD1306_WIDTH)] &= ~(1 << bit);
                }
            }
        }
    }

    // 更新屏幕
    ssd1306_update_screen();
}



/* 功能函数 */
void display_oled(void) {
    // float distance = SR04_GetData();
    // char dis_buffer[32];
    // sprintf(dis_buffer, "%4.2f", distance);
    ssd1306_set_cursor(17 ,1);
    ssd1306_write_string(font11x18, "Robot CMD");
    // ssd1306_set_cursor(1,25);
    // ssd1306_write_string(font6x8, "Error: NULL");
    ssd1306_set_cursor(1,35);
    ssd1306_write_string(font6x8, "Statuses:");
    // ssd1306_set_cursor(1,45);
    // ssd1306_write_string(font6x8, "Distance:");
    // ssd1306_clear_area(55,45,120,55);// no clear show "cm"
    // ssd1306_set_cursor(60,45);
    // ssd1306_write_string(font6x8, dis_buffer);
    // ssd1306_set_cursor(100,45);
    // ssd1306_write_string(font6x8, "cm");

    ssd1306_update_screen();
}

float sensing(void) {
    float distance = SR04_GetData();
    char dis_buffer[32];
    sprintf(dis_buffer, "%4.2f", distance);

    ssd1306_clear_area(55,45,120,55);// no clear show "cm"
    ssd1306_set_cursor(60,45);
    ssd1306_write_string(font6x8, dis_buffer);
    ssd1306_set_cursor(100,45);
    ssd1306_write_string(font6x8, "cm");
    return distance;
}

