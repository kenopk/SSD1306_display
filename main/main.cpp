#include <cstdio>
#include "driver_i2c.h"
#include "SSD1306.h"
#include "led_WS2812.h"
#include "esp_rom_sys.h"



extern "C" void app_main(void)
{
    i2c_gpio_init();    // инициализация пинов для работы I2C
    led_init();         // инициализация пина светодиода (GPIO8 -> выход, LOW)
    blink_error(3); // мигаем красным 3 раза, потом длинная пауза
    esp_rom_delay_us(1000 * 1000); // 1 секунды    
    ssd1306_init();
}
