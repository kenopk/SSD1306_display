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
    esp_rom_delay_us(500 * 1000); // 0.5 секунды    
    ssd1306_init();

    /*ssd1306_draw_rect(0, 127, 0, 3, 0xFF); // Рисуем белый прямоугольник на весь экран
    esp_rom_delay_us(1000 * 1000); // 1 секунда
    ssd1306_clear_ram(); // очищаем экран
    ssd1306_draw_rect(0, 127, 0, 3, 0xFF); // Рисуем черный прямоугольник на весь экран
    esp_rom_delay_us(1000 * 1000); // 1 секунда*/

    ssd1306_clear_buffer(); // очищаем буфер экрана
    //ssd1306_draw_rect(10, 5, 30, 20, true); // Рисуем белый прямоугольник на весь экран
    //ssd1306_update(); // обновляем экран из буфера
}
