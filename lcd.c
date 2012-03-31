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
lcd_putchar(int i, char c) {
    static const uint8_t map[] = {1, 2, 3, 5, 11, 10, 9, 8, 7};

    if(i > 8)
        return;

    c -= '0';

    uint8_t j = map[i];

    if(j > 5)
        lcd[j] = (lcd_font[c] << 4) | (lcd_font[c] >> 4);
    else
        lcd[j] = lcd_font[c];
}

void
lcd_putint(int line, int i) {
    if(line == 0) {
        lcd_putchar(0, '0' + i / 1000);
        i %= 1000;

        lcd_putchar(1, '0' + i / 100);
        i %= 100;

        lcd_putchar(2, '0' + i / 10);
        i %= 10;

        lcd_putchar(3, '0' + i);
    } else if(line == 1) {
        lcd_putchar(4, '0' + i / 10000);
        i %= 10000;

        lcd_putchar(5, '0' + i / 1000);
        i %= 1000;

        lcd_putchar(6, '0' + i / 100);
        i %= 100;

        lcd_putchar(7, '0' + i / 10);
        i %= 10;

        lcd_putchar(8, '0' + i);
    }
}
