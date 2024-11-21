#include <avr/io.h>
#include <util/delay.h>
#include "thuvienrieng.h"
void LCD_DISPLAY_HELLO_WORLD() {
    LCD4_CLR();                 // Xóa n?i dung trên LCD
    LCD4_CUR_GOTO(1, 0);        // ??a con tr? ??n dòng 1, c?t 0
    LCD4_OUT_STR("Hello,");     // Hi?n th? "Hello,"
    LCD4_CUR_GOTO(2, 0);        // ??a con tr? ??n dòng 2, c?t 0
    LCD4_OUT_STR("World!");     // Hi?n th? "World!"
}
int main() {
    LCD4_INIT(1, 0);          // Kh?i t?o LCD
    LCD_DISPLAY_HELLO_WORLD(); // Hi?n th? "Hello, World!" lên LCD

    while (1) {
    }

    return 0;
}
