#include <cstdio>
#include "driver_i2c.h"

extern "C" void app_main(void)
{
    const gpio_num_t I2C_SDA_PIN = GPIO_NUM_6;  // Пин данных (SDA)
    const gpio_num_t I2C_SCL_PIN = GPIO_NUM_7;  // Пин тактирования (SCL)
    const int I2C_DELAY_US = 5;                 // Задержка в микросекундах (~100 кГц)
    i2c_gpio_init();    // инициализация пинов для работы I2C
    start_i2c();
    
    ssd1306_init();

}
