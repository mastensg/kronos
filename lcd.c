#include <inttypes.h>

#include "cc430x613x.h"

#include "lcd.h"

unsigned char * const lcd = (unsigned char *)0x0A20;

//   A
// F   B
//   G
// E   C
//   D
const unsigned char lcd_font[] = {
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F      ,
          SEG_B|SEG_C                        ,
    SEG_A|SEG_B|      SEG_D|SEG_E|      SEG_G,
    SEG_A|SEG_B|SEG_C|SEG_D|            SEG_G,
          SEG_B|SEG_C|            SEG_F|SEG_G,
    SEG_A|      SEG_C|SEG_D|      SEG_F|SEG_G,
    SEG_A|      SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
    SEG_A|SEG_B|SEG_C                        ,
    SEG_A|SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
    SEG_A|SEG_B|SEG_C|SEG_D|      SEG_F|SEG_G,
    0                                        ,
    0                                        ,
    SEG_A|                        SEG_F|SEG_G,
                      SEG_D|            SEG_G,
    0                                        ,
    SEG_A|SEG_B|            SEG_E|      SEG_G,
    0                                        ,
    SEG_A|SEG_B|SEG_C|      SEG_E|SEG_F|SEG_G,
                SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
                      SEG_D|SEG_E|      SEG_G,
          SEG_B|SEG_C|SEG_D|SEG_E|      SEG_G,
    SEG_A|            SEG_D|SEG_E|SEG_F|SEG_G,
    SEG_A|                  SEG_E|SEG_F|SEG_G,
    SEG_A|SEG_B|SEG_C|SEG_D|      SEG_F|SEG_G,
                SEG_C|      SEG_E|SEG_F|SEG_G,
                            SEG_E            ,
    SEG_A|SEG_B|SEG_C|SEG_D                  ,
                      SEG_D|      SEG_F|SEG_G,
                      SEG_D|SEG_E|SEG_F      ,
    SEG_A|SEG_B|SEG_C|      SEG_E|SEG_F      ,
                SEG_C|      SEG_E|      SEG_G,
                SEG_C|SEG_D|SEG_E|      SEG_G,
    SEG_A|SEG_B|            SEG_E|SEG_F|SEG_G,
    SEG_A|SEG_B|SEG_C|            SEG_F|SEG_G,
                            SEG_E|      SEG_G,
    SEG_A|      SEG_C|SEG_D|      SEG_F|SEG_G,
                      SEG_D|SEG_E|SEG_F|SEG_G,
                SEG_C|SEG_D|SEG_E            ,
                SEG_C|SEG_D|SEG_E            ,
          SEG_B|SEG_C|SEG_D|SEG_E|SEG_F|SEG_G,
          SEG_B|SEG_C|      SEG_E|SEG_F|SEG_G,
          SEG_B|SEG_C|SEG_D|      SEG_F|SEG_G,
    SEG_A|SEG_B|      SEG_D|SEG_E|      SEG_G,
};

void
lcd_clear(void) {
    LCDBMEMCTL |= (LCDCLRBM | LCDCLRM);
}

void
lcd_putchar(uint8_t i, char c) {
    lcd_putdigit(i, c - '0');
}

void
lcd_putdigit(uint8_t i, uint8_t d) {
    static const uint8_t map[] = {1, 2, 3, 5, 11, 10, 9, 8, 7};

    if(i > 8)
        return;

    uint8_t j = map[i];

    if(j > 5)
        lcd[j] = (lcd_font[d] << 4) | (lcd_font[d] >> 4);
    else
        lcd[j] = lcd_font[d];
}

void
lcd_putint(uint8_t line, int i) {
    if(line == 0) {
        lcd_putdigit(0, i / 1000);
        i %= 1000;

        lcd_putdigit(1, i / 100);
        i %= 100;

        lcd_putdigit(2, i / 10);
        i %= 10;

        lcd_putdigit(3, i);
    } else if(line == 1) {
        lcd_putdigit(4, i / 10000);
        i %= 10000;

        lcd_putdigit(5, i / 1000);
        i %= 1000;

        lcd_putdigit(6, i / 100);
        i %= 100;

        lcd_putdigit(7, i / 10);
        i %= 10;

        lcd_putdigit(8, i);
    }
}
