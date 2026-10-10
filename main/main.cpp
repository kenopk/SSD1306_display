#include <cstdio>
#include "driver_i2c.h"
#include "SSD1306.h"
#include "led_WS2812.h"
#include "esp_rom_sys.h"
#include "ssd1306_commands.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"


extern "C" void app_main(void)
{
    i2c_gpio_init();    // инициализация пинов для работы I2C
    led_init();         // инициализация пина светодиода (GPIO8 -> выход, LOW)
    blink_error(3); // мигаем красным 3 раза, потом длинная пауза
    esp_rom_delay_us(500 * 1000); // 0.5 секунды    
    ssd1306_init();

    ssd1306_clear_buffer(); // очищаем буфер экрана

    // пиксели — по углам экрана, проверка границ и ориентации
    ssd1306_draw_pixel(0, 0, 1);
    ssd1306_draw_pixel(SSD1306_WIDTH-1, 0, 1);
    ssd1306_draw_pixel(0, SSD1306_HEIGHT-1, 1);
    ssd1306_draw_pixel(SSD1306_WIDTH-1, SSD1306_HEIGHT-1, 1);

    // залитый прямоугольник с чёрным окошком внутри (проверка color = 0)
    ssd1306_draw_area(4, 4, 30, 27, 1);
    ssd1306_draw_area(12, 11, 22, 20, 0);

    // две линии крест-накрест: пологие диагонали в разные стороны
    ssd1306_draw_line(36, 2, 74, 29);
    ssd1306_draw_line(36, 29, 74, 2);

    // окружность справа, r = 14 почти во всю высоту экрана
    ssd1306_draw_circle(98, 16, 14);

    ssd1306_update(); // отправляем готовый кадр на дисплей одним разом

    ssd1306_clear_buffer(); // очищаем буфер экрана
    const char *rest = ssd1306_draw_text(0, 0, "Очень длинный текст, который не помещается на один экран...");
    ssd1306_update();

    while (rest) {                              // пока есть непоказанный остаток
        vTaskDelay(pdMS_TO_TICKS(2000));        // даём прочитать страницу
        ssd1306_clear_buffer();
        rest = ssd1306_draw_text(0, 0, rest);   // следующая страница с самого верха
        ssd1306_update();
    }
}
