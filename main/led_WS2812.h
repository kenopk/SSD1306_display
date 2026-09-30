#pragma once

#include <stdint.h>

// Инициализация пина светодиода (GPIO8 -> выход, LOW)
void led_init();

// Мигнуть error_code раз, потом длинная пауза
void blink_error(int8_t error_code);