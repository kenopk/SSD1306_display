#include "SSD1306.h"
#include "driver_i2c.h"
#include "ssd1306_commands.h"
#include "led_WS2812.h"
#include "esp_rom_sys.h"
#include <string.h>  // memset

static void ssd1306_clear_ram();
static void ssd1306_send_command(uint8_t command);
static void ssd1306_send_data(uint8_t data);
static bool check_ack(int8_t error_code);
static void ssd1306_set_window(uint8_t col_start, uint8_t col_end, uint8_t page_start, uint8_t page_end);
static int abs_int(int a);

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
    ssd1306_send_command(0x02);     // для большинства 128*32 дисплеев

    // Set Contrast Control
    ssd1306_send_command(SSD1306_SET_CONTRAST);
    ssd1306_send_command(0x7F);
   
    // Disable entire display on
    //ssd1306_send_command(SSD1306_ENTIRE_DISPLAY_ON);
    //esp_rom_delay_us(3000 * 1000); // 3 секунды
    ssd1306_send_command(SSD1306_NORMAL_DISPLAY);

    // Set normal color display
    ssd1306_send_command(SSD1306_NORMAL_COLOR);

    // Set Osc Frequency
    ssd1306_send_command(SSD1306_SET_CLOCK_DIV);    // oscillator
    ssd1306_send_command(0x80);                     // datasheet

    // Enable charge pump
    ssd1306_send_command(SSD1306_CHARGE_PUMP);
    ssd1306_send_command(0x14);

    // Clear RAM
    ssd1306_send_command(SSD1306_SET_ADDR_MODE);
    ssd1306_send_command(SSD1306_ADDR_HORIZONTAL);
    ssd1306_clear_ram();

    // Display on
    ssd1306_send_command(SSD1306_DISPLAY_ON);

    // test color
    ssd1306_send_command(SSD1306_ENTIRE_DISPLAY_ON);
    esp_rom_delay_us(500 * 1000); // 0.5 секунды
    ssd1306_send_command(SSD1306_NORMAL_DISPLAY);

}

void static ssd1306_send_command(uint8_t command) {
    start_i2c();  // Начало передачи I2C
    i2c_write_byte(0x78);  // Адрес дисплея SSD1306 с записью (0x3C << 1)
    if (!check_ack(1)){
        //printf("Send command error: 0x%02X\n", command);
        return; // Если ACK не получен, выходим из функции, не отправляя команду
    }
    i2c_write_byte(0x00);  // следующий байт будет командой (0x00)
    if (!check_ack(2)){
        //printf("Send command error: 0x%02X\n", command);
        return; // Если ACK не получен, выходим из функции, не отправляя команду
    }
    i2c_write_byte(command);  // Отправка команды
    if (!check_ack(3)){
        //printf("Send command error: 0x%02X\n", command);
        return; // Если ACK не получен, выходим из функции, не отправляя команду
    }
    stop_i2c();
}

void static ssd1306_send_data(uint8_t data) {
    start_i2c();  // Начало передачи I2C
    i2c_write_byte(0x78);  // Адрес дисплея SSD1306 с записью (0x3C << 1)
    if (!check_ack(4)){
        //printf("Send data error: 0x%02X\n", data);
        return;
    }
    i2c_write_byte(0x40);  // следующий байт будет данными (0x40)
    if (!check_ack(5)){
        //printf("Send data error: 0x%02X\n", data);
        return;
    }
    i2c_write_byte(data);  // Отправка данных
    if (!check_ack(6)){
        //printf("Send data error: 0x%02X\n", data);
        return;
    }
    stop_i2c();
}

bool static check_ack(int8_t error_code) {
    if (!i2c_read_ack()) { 
        blink_error(error_code); // Мигаем нужное количество раз
        stop_i2c();              // Обязательно закрываем шину при ошибке!
        return false;            // Сообщаем вызывающей функции: "БЫЛА ОШИБКА"
    }
    return true; // Всё хорошо, можно продолжать
}

uint8_t static screen_buffer[SSD1306_WIDTH * SSD1306_PAGES]; // Буфер для хранения данных экрана (128*32/8 = 512 байт)

static void ssd1306_clear_ram(){
    ssd1306_set_window(0, SSD1306_WIDTH - 1, 0, SSD1306_PAGES - 1);
    for (uint16_t i = 0; i < SSD1306_WIDTH * SSD1306_PAGES; ++i) {
        ssd1306_send_data(0x00); // Заполняем нулями
    }
}

void ssd1306_clear_buffer(){
    memset(screen_buffer, 0, sizeof(screen_buffer));
}

static void ssd1306_set_window(uint8_t col_start, uint8_t col_end, uint8_t page_start, uint8_t page_end){
    ssd1306_send_command(SSD1306_SET_COLUMN_ADDR);
    ssd1306_send_command(col_start); // Начальный столбец
    ssd1306_send_command(col_end); // Конечный столбец

    ssd1306_send_command(SSD1306_SET_PAGE_ADDR);
    ssd1306_send_command(page_start); // Начальная страница
    ssd1306_send_command(page_end); // Конечная страница
}

void ssd1306_draw_pixel(uint8_t x, uint8_t y, bool color) {
if (x >= SSD1306_WIDTH || y >= SSD1306_HEIGHT) {
        return; // Выход, если координаты за пределами экрана
    }
    uint16_t index_byte = x+(y/8)*SSD1306_WIDTH;
    uint8_t index_bit = y % 8;
    if(color) {
        screen_buffer[index_byte] |= (1 << index_bit); // Устанавливаем бит
    } else {
        screen_buffer[index_byte] &= ~(1 << index_bit); // Сбрасываем бит
    }
}

void ssd1306_draw_area(uint8_t start_coord_x, uint8_t start_coord_y, uint8_t end_coord_x, uint8_t end_coord_y, bool color) {
    if (start_coord_x >= SSD1306_WIDTH  || start_coord_y >= SSD1306_HEIGHT || end_coord_x >= SSD1306_WIDTH || end_coord_y >= SSD1306_HEIGHT) {
        return; // Выход, если координаты за пределами экрана
    }
    if (start_coord_x > end_coord_x ) {
        start_coord_x = start_coord_x + end_coord_x;
        end_coord_x = start_coord_x - end_coord_x;
        start_coord_x = start_coord_x - end_coord_x;
    }
    if (start_coord_y > end_coord_y ) {
        start_coord_y = start_coord_y + end_coord_y;
        end_coord_y = start_coord_y - end_coord_y;
        start_coord_y = start_coord_y - end_coord_y;
    }
    for(uint8_t y = start_coord_y; y <= end_coord_y; ++y) {
        for(uint8_t x = start_coord_x; x <= end_coord_x; ++x) {
            ssd1306_draw_pixel(x, y, color);
        }
    }
}

static int abs_int(int a) {
    if (a < 0) {
        a = -a;
    }
    return a;
}

void ssd1306_draw_line( int start_coord_x, int start_coord_y, int end_coord_x, int end_coord_y){
    int dx =abs_int(end_coord_x - start_coord_x); // расстояние по x
    int dy =-abs_int(end_coord_y - start_coord_y); // расстояние по y
    int sx = (start_coord_x < end_coord_x) ? 1 : -1;  // в какую сторону шагать по X
    int sy = (start_coord_y < end_coord_y) ? 1 : -1;  // в какую сторону шагать по Y
    int err = dx + dy; 
    int e2;
    while (true) {  
        ssd1306_draw_pixel(start_coord_x, start_coord_y, 1); // рисуем пиксель
        if (start_coord_x == end_coord_x && start_coord_y == end_coord_y) break;
        e2 = 2 * err;
        if (e2 >= dy) { 
            err += dy; 
            start_coord_x += sx; 
        }
        if (e2 <= dx) { 
            err += dx; 
            start_coord_y += sy; 
        }
    }
}

void ssd1306_draw_circle(int center_x, int center_y, int radius) {
    int x = radius;        // начинаем справа от центра
    int y = 0;             // на уровне центра
    int err = 1 - radius;  // весы: внутри мы круга или снаружи

    while (x >= y) {       // пока не дошли до диагонали (45°)
        // одна точка → 8 отражений
        ssd1306_draw_pixel(center_x + x, center_y + y, 1);
        ssd1306_draw_pixel(center_x + y, center_y + x, 1);
        ssd1306_draw_pixel(center_x - y, center_y + x, 1);
        ssd1306_draw_pixel(center_x - x, center_y + y, 1);
        ssd1306_draw_pixel(center_x - x, center_y - y, 1);
        ssd1306_draw_pixel(center_x - y, center_y - x, 1);
        ssd1306_draw_pixel(center_x + y, center_y - x, 1);
        ssd1306_draw_pixel(center_x + x, center_y - y, 1);

        y++;                           // по Y шагаем ВСЕГДА
        if (err < 0) {                 // точка ещё внутри круга
            err += 2 * y + 1;          // x не трогаем
        } else {                       // вылезли наружу
            x--;                       // шаг по X внутрь
            err += 2 * (y - x) + 1;
        }
    }
}

void ssd1306_update(){
    ssd1306_set_window(0, SSD1306_WIDTH - 1, 0, SSD1306_PAGES - 1);
    for (uint16_t i = 0; i < SSD1306_WIDTH * SSD1306_PAGES; ++i) {
        ssd1306_send_data(screen_buffer[i]);
    }
}

