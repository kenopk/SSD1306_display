#include "SSD1306.h"
#include "driver_i2c.h"

void ssd1306_init() {
    // Инициализация дисплея SSD1306
    // Здесь можно добавить команды инициализации дисплея, если необходимо
}

void ssd1306_send_command(uint8_t command) {
    start_i2c();  // Начало передачи I2C
    i2c_write_byte(0x78);  // Адрес дисплея SSD1306 с записью (0x3C << 1)
    i2c_write_byte(0x00);  // следующий байт будет командой (0x00)
    i2c_write_byte(command);  // Отправка команды
    // Здесь можно добавить проверку на ACK, если необходимо
}

void ssd1306_send_data(uint8_t data) {
    start_i2c();  // Начало передачи I2C
    i2c_write_byte(0x78);  // Адрес дисплея SSD1306 с записью (0x3C << 1)
    i2c_write_byte(0x40);  // следующий байт будет данными (0x40)
    i2c_write_byte(data);  // Отправка данных
    // Здесь можно добавить проверку на ACK, если необходимо
}

void ssd1306_init()