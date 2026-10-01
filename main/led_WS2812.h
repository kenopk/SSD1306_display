#pragma once

#include <stdint.h>

// Драйвер адресного светодиода WS2812B (GPIO8), bit-bang по даташиту DOC001554925.pdf
//
// Публичный API. Внутренние функции (ws_send_bit, led_set_color, blink_brightness)
// объявлены static в led_WS2812.cpp и сюда не входят.

// Инициализация пина светодиода (GPIO8 -> выход, LOW)
void led_init();

// Мигнуть цветом error_code раз, потом длинная пауза (BLINK_PAUSE_MS).
// error_code <= 0 -> ничего не делает.
void blink_error(int8_t error_code);
