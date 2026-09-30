#pragma once  // Защита от двойного включения этого файла

#include <stdint.h>
#include <stdbool.h>

void ssd1306_init();
void ssd1306_send_command(uint8_t command);
void ssd1306_send_data(uint8_t data);
bool check_ack(int8_t error_code);
