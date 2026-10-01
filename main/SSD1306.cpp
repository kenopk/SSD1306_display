#include "SSD1306.h"
#include "driver_i2c.h"
#include "ssd1306_commands.h"
#include "led_WS2812.h"
#include "esp_rom_sys.h"

void ssd1306_init(){
    // выключение хороший тон
    ssd1306_send_command(SSD1306_DISPLAY_OFF);
    
    // Set Mux
    ssd1306_send_command(SSD1306_SET_MULTIPLEX);    // set MUX 
    ssd1306_send_command(0x1F);                     // 32 string

    // Set Display offset
    ssd1306_send_command(SSD1306_SET_DISPLAY_OFFSET);       // Set display offset
    ssd1306_send_command(0x00);       // без смещения
    
    // Set Display start line
    ssd1306_send_command(SSD1306_SET_START_LINE);   // точка начала строки

    // Set Segment re-map
    //ssd1306_send_command(SSD1306_SEG_REMAP_NORMAL); // нормальный режим, но есть и зеркальный
    ssd1306_send_command(SSD1306_SEG_REMAP_FLIP);

    // Set COM output scan direction
    //ssd1306_send_command(SSD1306_COM_NORMAL);   // нормальный режим, но есть и зеркальный
    ssd1306_send_command(SSD1306_COM_REMAP);

    // Set COM pins
    ssd1306_send_command(SSD1306_SET_COM_PINS);  // назначение реальных пинов
    ssd1306_send_command(0x02);     // для болшинства 128*32 дисплеев

    // Set Contrast Control
    ssd1306_send_command(SSD1306_SET_CONTRAST);
    ssd1306_send_command(0x7F);
   
    // Disably entire / display on
    //ssd1306_send_command(SSD1306_ENTIRE_DISPLAY_ON);
    //esp_rom_delay_us(3000 * 1000); // 3 секунды
    ssd1306_send_command(SSD1306_NORMAL_DISPLAY);

    // Set normal colar display
    ssd1306_send_command(SSD1306_NORMAL_COLOR);

    // Set Osc Frequency
    ssd1306_send_command(SSD1306_SET_CLOCK_DIV);    // oscillator
    ssd1306_send_command(0x80);                     // datasheet

    // Enable charge pump
    ssd1306_send_command(SSD1306_CHARGE_PUMP);
    ssd1306_send_command(0x14);

    // Display on
    ssd1306_send_command(SSD1306_DISPLAY_ON);

    // test color
    ssd1306_send_command(SSD1306_ENTIRE_DISPLAY_ON);
    esp_rom_delay_us(1500 * 1000); // 1,5 секунды
    ssd1306_send_command(SSD1306_NORMAL_DISPLAY);

}

void ssd1306_send_command(uint8_t command) {
    start_i2c();  // Начало передачи I2C
    i2c_write_byte(0x78);  // Адрес дисплея SSD1306 с записью (0x3C << 1)
    if (!check_ack(1)) return;
    i2c_write_byte(0x00);  // следующий байт будет командой (0x00)
    if (!check_ack(2)) return;
    i2c_write_byte(command);  // Отправка команды
    if (!check_ack(3)) return;
    stop_i2c();
}

void ssd1306_send_data(uint8_t data) {
    start_i2c();  // Начало передачи I2C
    i2c_write_byte(0x78);  // Адрес дисплея SSD1306 с записью (0x3C << 1)
    if (!check_ack(4)) return;
    i2c_write_byte(0x40);  // следующий байт будет данными (0x40)
    if (!check_ack(5)) return;
    i2c_write_byte(data);  // Отправка данных
    if (!check_ack(6)) return; 
    stop_i2c();
}

bool check_ack(int8_t error_code) {
    if (!i2c_read_ack()) { 
        blink_error(error_code); // Мигаем нужное количество раз
        stop_i2c();              // Обязательно закрываем шину при ошибке!
        return false;            // Сообщаем вызывающей функции: "БЫЛА ОШИБКА"
    }
    return true; // Всё хорошо, можно продолжать
}