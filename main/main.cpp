#include <cstdio>
#include "driver_i2c.h"
#include "SSD1306.h"
#include "led_WS2812.h"




extern "C" void app_main(void)
{
    i2c_gpio_init();    // инициализация пинов для работы I2C
    ssd1306_init();

}
