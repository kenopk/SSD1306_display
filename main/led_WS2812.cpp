#include "led_WS2812.h"
#include "sdkconfig.h"
#include "driver/gpio.h"
#include "esp_cpu.h"
#include "soc/gpio_reg.h"
#include "esp_rom_sys.h"
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"

#define LED_PIN  8


//такты = время_в_нс × частота_в_МГц / 1000 
#define T1H ((900*CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ)/1000) // 0,9 мкс = (900 * 160) / 1000 = 144 такта
#define T0H ((350*CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ)/1000) // 0,35 мкс = (350 * 160) / 1000 = 56 тактов
#define T_BIT ((1250*CONFIG_ESP_DEFAULT_CPU_FREQ_MHZ)/1000) // 1,25 мкс = (1250 * 160) / 1000 = 200 тактов
#define T_RESET_US 80 // 80 мкс 

constexpr uint32_t LED_MASK = 1u << LED_PIN;

static void ws_send_bit(bool bit) {
    uint32_t start_cycle = esp_cpu_get_cycle_count();
    uint32_t t_high = bit ? T1H : T0H;
    REG_WRITE(GPIO_OUT_W1TS_REG, LED_MASK);
    while ((esp_cpu_get_cycle_count() - start_cycle) < t_high) {
    // busy wait
    }
    REG_WRITE(GPIO_OUT_W1TC_REG, LED_MASK);
    while ((esp_cpu_get_cycle_count() - start_cycle) < T_BIT) {
    // busy wait
    }
}

static void led_set_color(uint8_t r, uint8_t g, uint8_t b) {
    // TODO
}

// ---- публичная часть (объявлена в .h) ----

void led_init() {
    gpio_reset_pin(gpio_num_t(LED_PIN));
    gpio_set_direction(gpio_num_t(LED_PIN), GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num_t(LED_PIN), 0);
}

void blink_error(int8_t error_code) {
    // TODO
}