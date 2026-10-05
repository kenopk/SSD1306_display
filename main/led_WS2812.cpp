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

#define BLINK_ON_MS 200
#define BLINK_OFF_MS 200
#define BLINK_PAUSE_MS 1000

constexpr uint32_t LED_MASK = 1u << LED_PIN;
static portMUX_TYPE ws_mux = portMUX_INITIALIZER_UNLOCKED;

static void ws_send_bit(bool bit);
static void led_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness);
static uint8_t blink_brightness(uint8_t color, uint8_t brightness);

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

static void led_set_color(uint8_t r, uint8_t g, uint8_t b, uint8_t brightness) {
    
    // WS2812 GRB with brightness adjustment
    uint32_t r_bright = blink_brightness(r, brightness);
    uint32_t g_bright = blink_brightness(g, brightness);
    uint32_t b_bright = blink_brightness(b, brightness);

    g_bright = g_bright << 16;        // зелёный сдвинули на 16 бит влево (то же, что × 65536)
    r_bright = r_bright << 8;         // красный на 8 бит влево (то же, что × 256)

    uint32_t grb = g_bright | r_bright | b_bright;   // склеили всё в одно число

    portENTER_CRITICAL(&ws_mux);
    for (int i = 23; i >= 0; --i) {
        ws_send_bit((grb >> i) & 1);
    }
    portEXIT_CRITICAL(&ws_mux);

    esp_rom_delay_us(T_RESET_US);          // RES ≥ 50 мкс: светодиод защёлкивает цвет
}

void led_init() {
    gpio_reset_pin(gpio_num_t(LED_PIN));
    gpio_set_direction(gpio_num_t(LED_PIN), GPIO_MODE_OUTPUT);
    gpio_set_level(gpio_num_t(LED_PIN), 0);
}

void blink_error(int8_t error_code) {
    uint8_t brightness = 125; // яркость мигания (0-255)
    for(int i = 0; i < error_code; ++i) {
        led_set_color(255, 100, 20, brightness);
        vTaskDelay(pdMS_TO_TICKS(BLINK_ON_MS));
        led_set_color(0, 0, 0, 0);   // выключить
        vTaskDelay(pdMS_TO_TICKS(BLINK_OFF_MS));
    }
    vTaskDelay(pdMS_TO_TICKS(BLINK_PAUSE_MS));
}

static uint8_t blink_brightness(uint8_t color, uint8_t brightness) {
    return (color * brightness) / 255;
}