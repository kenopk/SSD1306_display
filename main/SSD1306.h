#pragma once  // Защита от двойного включения этого файла

#include <stdint.h>
#include <stdbool.h>

// Публичный API драйвера SSD1306.
// Внутренние функции (send_command, send_data, check_ack, set_window, clear_ram)
// объявлены static в SSD1306.cpp и сюда не входят.

void ssd1306_init();

// Работа с буфером кадра (в RAM ESP32, без I2C)
void ssd1306_clear_buffer();
void ssd1306_draw_pixel(uint8_t x, uint8_t y, bool color);

// Отправка буфера на дисплей
void ssd1306_update();

// Пишет напрямую в дисплей, в обход буфера (координаты в страницах)
void ssd1306_draw_rect(uint8_t col_start, uint8_t col_end, uint8_t page_start, uint8_t page_end, uint8_t color);
