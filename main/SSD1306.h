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

// заливка прямоугольной области на дисплее (черный или белый)
void ssd1306_draw_area(uint8_t start_coord_x, uint8_t start_coord_y, uint8_t end_coord_x, uint8_t end_coord_y, bool color);

// линия от (start) до (end), алгоритм Брезенхэма
void ssd1306_draw_line(int start_coord_x, int start_coord_y, int end_coord_x, int end_coord_y);

// окружность с центром (center_x, center_y) и радиусом radius
void ssd1306_draw_circle(int center_x, int center_y, int radius);

// Отрисовка символов и текста (использует шрифт из font_8x10.h)
const char *ssd1306_draw_text(uint8_t x, uint8_t y, const char *text);

// Функции для работы с битмапами (например, для иконок)
void ssd1306_draw_bitmap(uint8_t x, uint8_t y, const uint8_t *bitmap, uint8_t width, uint8_t height);

