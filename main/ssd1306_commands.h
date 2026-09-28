#pragma once
// ============================================================================
// SSD1306 Commands and Constants
// Все значения взяты из даташита SSD1306 (раздел 9 COMMAND TABLE)
// ============================================================================

// ============================================================================
// CONTROL BYTE (управляющий байт для I2C)
// Формат: [Co] [D/C#] [0] [0] [0] [0] [0] [0]
// Co = 0 (передача продолжается), D/C# = 0 (команда) или 1 (данные)
// ============================================================================
#define SSD1306_CONTROL_COMMAND     0x00  // Co=0, D/C#=0 → следующий байт = КОМАНДА
#define SSD1306_CONTROL_DATA        0x40  // Co=0, D/C#=1 → следующий байт = ДАННЫЕ (пиксели)

// ============================================================================
// I2C ADDRESS (адрес устройства на шине I2C)
// Формат: [0] [1] [1] [1] [1] [0] [SA0] [R/W#]
// SA0 = 0 (D/C# пин подключен к GND), R/W# = 0 (запись)
// ============================================================================
#define SSD1306_I2C_ADDRESS         0x78  // 01111000b = адрес для ЗАПИСИ

// ============================================================================
// FUNDAMENTAL COMMANDS (базовые команды)
// Раздел 10.1 даташита
// ============================================================================

// Display ON/OFF (раздел 10.1.12)
#define SSD1306_DISPLAY_OFF         0xAE  // Display OFF (sleep mode)   1010 1110
#define SSD1306_DISPLAY_ON          0xAF  // Display ON (normal mode)   1010 1111

// Entire Display ON/OFF (раздел 10.1.9)
#define SSD1306_NORMAL_DISPLAY      0xA4  // Resume to RAM content display  1010 0100 
#define SSD1306_ENTIRE_DISPLAY_ON   0xA5  // Entire display ON (ignore RAM) используем при запуске, что бы проверить наличие битых пикселей 1010 0101

// Normal/Inverse Display (раздел 10.1.10)
#define SSD1306_NORMAL_COLOR        0xA6  // Normal:    1=ON, 0=OFF    1010 0110
#define SSD1306_INVERSE_COLOR       0xA7  // Inverse:   0=ON, 1=OFF   1010 0111

// Set Contrast Control (раздел 10.1.7) — ДВУХБАЙТОВАЯ команда
// Формат: 0x81, затем значение контраста (0x00-0xFF)
#define SSD1306_SET_CONTRAST        0x81  // Аргумент: 0x00 (мин) - 0xFF (макс) 1000 0001 яркость

// ============================================================================
// HARDWARE CONFIGURATION COMMANDS (настройка железа)
// Раздел 10.1, подраздел "Hardware Configuration"
// ============================================================================

// Set Multiplex Ratio (раздел 10.1.11) — ДВУХБАЙТОВАЯ команда
// Формат: 0xA8, затем значение N (16-63), реальное количество строк = N+1
// Сколько физических строк у нашего дисплея? Настрой сканер на это количество
#define SSD1306_SET_MULTIPLEX       0xA8  // Аргумент: 0x1F = 32 MUX (для 128x32)

// Set Display Offset (раздел 10.1.15) — ДВУХБАЙТОВАЯ команда
// Формат: 0xD3, затем смещение (0x00-0x3F)
#define SSD1306_SET_DISPLAY_OFFSET  0xD3  // Аргумент: 0x00 = без смещения 

// Set Display Start Line (раздел 10.1.6)
// Формат: 0x40 + значение (0-32), определяет начальную строку RAM
#define SSD1306_SET_START_LINE      0x40  // 0x40 + 0x00 = начать с строки 0 0x20(макс) - 32

// Set Segment Re-map (раздел 10.1.8)    отражение по горизонтали
#define SSD1306_SEG_REMAP_NORMAL    0xA0  // Column 0 mapped to SEG0
#define SSD1306_SEG_REMAP_FLIP      0xA1  // Column 127 mapped to SEG0 (зеркало)

// Set COM Output Scan Direction (раздел 10.1.14)   отражение по вертикали
#define SSD1306_COM_NORMAL          0xC0  // Scan from COM0 to COM[N-1]
#define SSD1306_COM_REMAP           0xC8  // Scan from COM[N-1] to COM0 (зеркало)

// Set COM Pins Hardware Configuration (раздел 10.1.18) — ДВУХБАЙТОВАЯ команда
// Формат: 0xDA, затем конфигурация
// вот схема того, как именно строки (COM) подключены к этому конкретному куску стекла. Используй эту схему".
// 0 - Строки идут по порядку 0 - Disable (Отключено). Стандартное направление.
// 0 - Alternative (Альтернативная). Строки идут вперемешку: 0, 63, 1, 62... (часто используется в дисплеях 128x64). 0 - Enable (Включено). Зеркальное отражение строк по горизонтали.
#define SSD1306_SET_COM_PINS        0xDA  // Аргумент: 0x02 - последовательность (Alternative, no remap - 0x12)

// ============================================================================
// TIMING & DRIVING SCHEME COMMANDS (тайминги и управление питанием)
// Раздел 10.1, подраздел "Timing & Driving Scheme"
// ============================================================================

// Set Display Clock Divide Ratio/Oscillator Frequency (раздел 10.1.16) — ДВУХБАЙТОВАЯ
// Формат: 0xD5, затем значение [A7:A4 = частота осциллятора] [A3:A0 = делитель]
// 1111 - самое медленное 15+1 0000 - самое быстрое 0+1 - делитель частоты
// 
#define SSD1306_SET_CLOCK_DIV       0xD5  // Аргумент: 0x80 (default: div=1, freq=1000b)
// Set Pre-charge Period (раздел 10.1.17) — ДВУХБАЙТОВАЯ команда
// Формат: 0xD9, затем значение [A7:A4 = phase 2] [A3:A0 = phase 1]
// Говорит контроллеру дисплея: "Сколько тактов внутренних часов (DCLK) должна длиться Фаза 2 (предварительная зарядка)
#define SSD1306_SET_PRECHARGE       0xD9  // Аргумент: 0x22 (recommended)   1101 1001 и 0010 0010

// Set VCOMH Deselect Level (раздел 10.1.19) — ДВУХБАЙТОВАЯ команда
// Формат: 0xDB, затем значение [A6:A4]
#define SSD1306_SET_VCOMH           0xDB  // Аргумент: 0x40 (~0.77 x VCC)

// Charge Pump Setting (раздел 2.1 App Note) — ДВУХБАЙТОВАЯ команда
// КРИТИЧЕСКИ ВАЖНО: без включения charge pump дисплей не засветится!
// Формат: 0x8D, затем 0x14 (enable) или 0x10 (disable)
#define SSD1306_CHARGE_PUMP         0x8D  // Аргумент: 0x14 = ВКЛЮЧИТЬ

// ============================================================================
// MEMORY ADDRESSING MODE COMMANDS (режимы адресации памяти)
// Раздел 10.1.3
// ============================================================================

// Set Memory Addressing Mode — ДВУХБАЙТОВАЯ команда
// Формат: 0x20, затем режим адресации
#define SSD1306_SET_ADDR_MODE       0x20

// Аргументы для команды 0x20:
#define SSD1306_ADDR_HORIZONTAL     0x00  // Horizontal Addressing Mode
#define SSD1306_ADDR_VERTICAL       0x01  // Vertical Addressing Mode
#define SSD1306_ADDR_PAGE           0x02  // Page Addressing Mode (default)

// ============================================================================
// COLUMN AND PAGE ADDRESS COMMANDS (адресация столбцов и страниц)
// Раздел 10.1.1, 10.1.2, 10.1.4, 10.1.5, 10.1.13
// ============================================================================

// Set Lower Column Start Address (Page Addressing Mode)
// Формат: 0x00 + lower nibble (0x00-0x0F)
#define SSD1306_SET_COL_LOW         0x00  // + nibble 0-15

// Set Higher Column Start Address (Page Addressing Mode)
// Формат: 0x10 + higher nibble (0x00-0x0F)
#define SSD1306_SET_COL_HIGH        0x10  // + nibble 0-15

// Set Column Address (Horizontal/Vertical Addressing Mode) — ТРЁХБАЙТОВАЯ команда
// Формат: 0x21, start_col (0-127), end_col (0-127)
#define SSD1306_SET_COLUMN_ADDR     0x21

// Set Page Address (Horizontal/Vertical Addressing Mode) — ТРЁХБАЙТОВАЯ команда
// Формат: 0x22, start_page (0-7), end_page (0-7)
#define SSD1306_SET_PAGE_ADDR       0x22

// Set Page Start Address (Page Addressing Mode)
// Формат: 0xB0 + page number (0-7)
#define SSD1306_SET_PAGE            0xB0  // + page 0-7

// ============================================================================
// SCROLLING COMMANDS (команды прокрутки)
// Раздел 10.2
// ============================================================================

#define SSD1306_SCROLL_RIGHT        0x26  // Right Horizontal Scroll
#define SSD1306_SCROLL_LEFT         0x27  // Left Horizontal Scroll
#define SSD1306_SCROLL_VRIGHT       0x29  // Vertical and Right Horizontal Scroll
#define SSD1306_SCROLL_VLEFT        0x2A  // Vertical and Left Horizontal Scroll
#define SSD1306_SCROLL_DEACTIVATE   0x2E  // Stop scrolling
#define SSD1306_SCROLL_ACTIVATE     0x2F  // Start scrolling
#define SSD1306_SET_SCROLL_AREA     0xA3  // Set Vertical Scroll Area

// ============================================================================
// OTHER COMMANDS (другие команды)
// ============================================================================

#define SSD1306_NOP                 0xE3  // No Operation (пустышка)

// ============================================================================
// DISPLAY DIMENSIONS (размеры дисплея)
// ============================================================================

#define SSD1306_WIDTH               128   // Ширина дисплея в пикселях
#define SSD1306_HEIGHT              64    // Высота дисплея в пикселях
#define SSD1306_PAGES               8     // Количество страниц (64 / 8 = 8)
#define SSD1306_PAGE_HEIGHT         8     // Высота одной страницы в битах