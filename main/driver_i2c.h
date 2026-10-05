#pragma once  // Защита от двойного включения этого файла

#include "driver/gpio.h"
#include "esp_rom_sys.h"
#include <stdint.h> // Для типа uint8_t
#include <stdbool.h> // Для типа bool

// Константы для работы I2C
const gpio_num_t I2C_SDA_PIN = GPIO_NUM_6;  // Пин данных (SDA)
const gpio_num_t I2C_SCL_PIN = GPIO_NUM_7;  // Пин тактирования (SCL)
const int I2C_DELAY_US = 20;                 // Задержка в микросекундах (~16 кГц)

// Объявления функций (прототипы)
// Эти объявления говорят компилятору: "функции существуют, их тела в driver_i2c.cpp"
void i2c_gpio_init(void);
void start_i2c(void);
void stop_i2c(void);
void i2c_write_byte(uint8_t byte);  // Передача одного байта
bool i2c_read_ack(void);