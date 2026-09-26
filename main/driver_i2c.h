#pragma once  // Защита от двойного включения этого файла

#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include <stdint.h> // Для типа uint8_t
#include <stdbool.h> // Для типа bool

// Константы для работы I2C
static const gpio_num_t I2C_SDA_PIN = GPIO_NUM_6;  // Пин данных (SDA)
static const gpio_num_t I2C_SCL_PIN = GPIO_NUM_7;  // Пин тактирования (SCL)
static const int I2C_DELAY_US = 5;                 // Задержка в микросекундах (~100 кГц)

// Объявления функций (прототипы)
// Эти объявления говорят компилятору: "функции существуют, их тела в driver_i2c.cpp"
void i2c_gpio_init(void);
void start_i2c(void);
void stop_i2c(void);
void i2c_write_bit(bool bit);  // Передача одного бита
void i2c_write_byte(uint8_t byte);  // Передача одного байта