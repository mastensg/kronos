#include "cc430x613x.h"

#include "lcd.h"

#define BUTTONS_IN              P2IN
#define BUTTONS_OUT             P2OUT
#define BUTTONS_DIR             P2DIR
#define BUTTONS_REN             P2REN
#define BUTTONS_IE              P2IE
#define BUTTONS_IES             P2IES
#define BUTTONS_IFG             P2IFG
#define BUTTONS_IRQ_VECT2       PORT2_VECTOR

#define BUTTON_STAR_PIN         (1 << 2)
#define BUTTON_NUM_PIN          (1 << 1)
#define BUTTON_UP_PIN           (1 << 4)
#define BUTTON_DOWN_PIN         (1 << 0)
#define BUTTON_BACKLIGHT_PIN    (1 << 3)
#define ALL_BUTTONS             (BUTTON_STAR_PIN + BUTTON_NUM_PIN + BUTTON_UP_PIN + BUTTON_DOWN_PIN + BUTTON_BACKLIGHT_PIN)

int
main(void) {
    WDTCTL = WDTPW + WDTHOLD;

    // LCD_FREQ = ACLK/16/8 = 256Hz
    // Frame frequency = 256Hz/4 = 64Hz, LCD mux 4, LCD on
    LCDBCTL0 = (LCDDIV0 + LCDDIV1 + LCDDIV2 + LCDDIV3) | (LCDPRE0 + LCDPRE1) | LCD4MUX | LCDON;

    // LCB_BLK_FREQ = ACLK/8/4096 = 1Hz
    LCDBBLKCTL = LCDBLKPRE0 | LCDBLKPRE1 | LCDBLKDIV0 | LCDBLKDIV1 | LCDBLKDIV2 | LCDBLKMOD0;

    P5SEL |= BIT5 | BIT6 | BIT7;
    P5DIR |= BIT5 | BIT6 | BIT7;

    LCDBPCTL0 = 0xFFFF;
    LCDBPCTL1 = 0x00FF;

    RTCCTL1 &= ~(RTCHOLD >> 8);
    RTCCTL1 |= (RTCMODE >> 8);

    BUTTONS_OUT &= ~ALL_BUTTONS;
    BUTTONS_REN |= ALL_BUTTONS;

    RTCHOUR = 23;
    RTCMIN = 11;
    RTCSEC = 32;

    lcd_clear();

    int x = 0;
    for(;;) {
        __delay_cycles(62500);

        int h = RTCHOUR;
        int m = RTCMIN;
        int s = RTCSEC;

        lcd_putchar(0, '0' + h / 10);
        lcd_putchar(1, '0' + h % 10);

        lcd_putchar(2, '0' + m / 10);
        lcd_putchar(3, '0' + m % 10);

        lcd_putchar(4, '0' + s / 10);
        lcd_putchar(5, '0' + s % 10);

        int b = BUTTONS_IN;

        lcd_putchar(7, '0' + b / 16);
        lcd_putchar(8, '0' + b % 16);

        if(BUTTONS_IN & BUTTON_NUM_PIN)
            ((void (*)())0x1000)();
    }

    for(;;);

    return 0;
}
